#include <fs/fs.h>
#include <fs/spin.h>
#include <string.h>
#include <stdlib.h>
#include <util/list.h>
#include <util/tree.h>
#include <util/hashmap.h>
#include <util/logging.h>

#define MAX_SYMLINK_DEPTH 8
#define MAX_SYMLINK_SIZE 4096

#ifndef foreach
#define foreach(item, list) \
    for (list_node *(item) = (list); (item); (item) = (item)->next)
#endif

tree_t    * fs_tree = NULL; /* File system mountpoint tree */
fs_node_t * fs_root = NULL; /* Pointer to the root mount fs_node (must be some form of filesystem, even ramdisk) */

hashmap_t * fs_types = NULL;

void vfs_install(void) {
  /* Initialize the mountpoint tree */
  fs_tree = tree_create();

  struct vfs_entry * root = (struct vfs_entry *)malloc(sizeof(struct vfs_entry));

  root->name = (char*)malloc(strlen("[root]")+1);
  strcpy(root->name, "[root]");
  root->file = NULL; /* Nothing mounted as root */

  tree_set_root(fs_tree, root);

  fs_types = hashmap_create(5);
}

//volatile uint8_t tmp_refcount_lock = 0;
static spin_lock_t tmp_refcount_lock = { 0 };

void vfs_lock(fs_node_t * node) {
  spin_lock(tmp_refcount_lock);
  node->refcount = -1;
  spin_unlock(tmp_refcount_lock);
}

int vfs_register(char * name, vfs_mount_callback callback) {
  if (hashmap_get(fs_types, name)) return 1;
  hashmap_set(fs_types, name, (void *)(uintptr_t)callback);
  return 0;
}

/**
 * finddir_fs: Find the requested file in the directory and return an fs_node for it
 *
 * @param node Directory to search
 * @param name File to look for
 * @returns An fs_node that the caller can free
 */
fs_node_t *finddir_fs(fs_node_t *node, char *name) {
  if (!node) return NULL;

  if ((node->flags & FS_DIRECTORY) && node->finddir) {
    fs_node_t *ret = node->finddir(node, name);
    return ret;
  } else {
    debug_print(WARNING, "Node passed to finddir_fs isn't a directory!");
    debug_print(WARNING, "node = 0x%x, name = %s", node, name);
    return (fs_node_t *)NULL;
  }
}

/**
 * read_fs: Read a file system node based on its underlying type.
 *
 * @param node    Node to read
 * @param offset  Offset into the node data to read from
 * @param size    How much data to read (in bytes)
 * @param buffer  A buffer to copy of the read data into
 * @returns Bytes read
 */
uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
  if (!node) return -1;

  if (node->read) {
    uint32_t ret = node->read(node, offset, size, buffer);
    return ret;
  } else {
    return -1;
  }
}


/**
 * write_fs: Write a file system node based on its underlying type.
 *
 * @param node    Node to write to
 * @param offset  Offset into the node data to write to
 * @param size    How much data to write (in bytes)
 * @param buffer  A buffer to copy from
 * @returns Bytes written
 */
uint32_t write_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
  if (!node) return -1;

  if (node->write) {
    uint32_t ret = node->write(node, offset, size, buffer);
    return ret;
  } else {
    return -1;
  }
}



/**
 * open_fs: Open a file system node.
 *
 * @param node  Node to open
 * @param flags Same as open, specifies read/write/append/truncate
 */
void open_fs(fs_node_t *node, unsigned int flags) {

  if (!node) return;

  if (node->refcount >= 0) {
    spin_lock(tmp_refcount_lock);
    node->refcount++;
    spin_unlock(tmp_refcount_lock);
  }

  if (node->open) {
    node->open(node, flags);
  }
}



void close_fs(fs_node_t *node) {
  assert(node != fs_root, "Attempted to close the filesystem root. kablooey");

  if (!node) {
    debug_print(WARNING,"%s", "Double close? This isn't an fs_node.");
    return;
  }

  if (node->refcount == -1) return;

  spin_lock(tmp_refcount_lock);
  node->refcount--;
  if (node->refcount == 0) {
    debug_print(NOTICE,"%s",  "Node refcount [%s] is now 0: %d", node->name, node->refcount);

    if (node->close) {
      node->close(node);
    }

    free(node);
  }
  spin_unlock(tmp_refcount_lock);
}








/**
 * canonicalize_path: Canonicalize a path.
 *
 * @param cwd   Current working directory
 * @param input Path to append or canonicalize on
 * @returns An absolute path string
 */
char *canonicalize_path(char *cwd, char *input) {
  /* This is a stack-based canonicalizer; we use a list as a stack */
  list_t *out = list_create();

  /*
   * If we have a relative path, we need to canonicalize
   * the working directory and insert it into the stack.
   */
  if (strlen(input) && input[0] != PATH_SEPARATOR) {
    /* Make a copy of the working directory */
    char *path = (char*)malloc((strlen(cwd) + 1) * sizeof(char));
    memcpy(path, cwd, strlen(cwd) + 1);

    /* Setup tokenizer */
    char *pch;
    pch = strtok(path,PATH_SEPARATOR_STRING);

    /* Start tokenizing */
    while (pch != NULL) {
      /* Make copies of the path elements */
      char *s = (char*)malloc(sizeof(char) * (strlen(pch) + 1));
      memcpy(s, pch, strlen(pch) + 1);
      /* And push them */
      list_insert(out, s);
      pch = strtok(NULL,PATH_SEPARATOR_STRING);
    }
    free(path);
  }

  /* Similarly, we need to push the elements from the new path */
  char *path = (char*)malloc((strlen(input) + 1) * sizeof(char));
  memcpy(path, input, strlen(input) + 1);

  /* Initialize the tokenizer... */
  char *pch;
  pch = strtok(path,PATH_SEPARATOR_STRING);

  /*
   * Tokenize the path, this time, taking care to properly
   * handle .. and . to represent up (stack pop) and current
   * (do nothing)
   */
  while (pch != NULL) {
    if (!strcmp(pch,PATH_UP)) {
      /*
       * Path = ..
       * Pop the stack to move up a directory
       */
      node_t * n = list_pop(out);
      if (n) {
        free(n->value);
        free(n);
      }
    } else if (!strcmp(pch,PATH_DOT)) {
      /*
       * Path = .
       * Do nothing
       */
    } else {
      /*
       * Regular path, push it
       * XXX: Path elements should be checked for existence!
       */
      char * s = (char*)malloc(sizeof(char) * (strlen(pch) + 1));
      memcpy(s, pch, strlen(pch) + 1);
      list_insert(out, s);
    }
    pch = strtok(NULL, PATH_SEPARATOR_STRING);
  }
  free(path);

  /* Calculate the size of the path string */
  size_t size = 0;
  foreach(item, out) {
    /* Helpful use of our foreach macro. */
    size += strlen((char *)item->value) + 1;
  }

  /* join() the list */
  char *output = (char*)malloc(sizeof(char) * (size + 1));
  char *output_offset = output;
  if (size == 0) {
    /*
     * If the path is empty, we take this to mean the root
     * thus we synthesize a path of "/" to return.
     */
    output = (char*)realloc(output, sizeof(char) * 2);
    output[0] = PATH_SEPARATOR;
    output[1] = '\0';
  } else {
    /* Otherwise, append each element together */
    foreach(item, out) {
      output_offset[0] = PATH_SEPARATOR;
      output_offset++;
      memcpy(output_offset, item->value, strlen((char *)item->value) + 1);
      output_offset += strlen((char *)item->value);
    }
  }

  /* Clean up the various things we used to get here */
  list_destroy(out);
  list_free(out);
  free(out);

  /* And return a working, absolute path */
  return output;
}

/**
 * get_mount_point
 *
 */
fs_node_t *get_mount_point(char * path, unsigned int path_depth, char **outpath, unsigned int * outdepth) {
  size_t depth;

  for (depth = 0; depth <= path_depth; ++depth) {
    path += strlen(path) + 1;
  }

  /* Last available node */
  fs_node_t   * last = fs_root;
  tree_node_t * node = fs_tree->root;

  char * at = *outpath;
  int _depth = 1;
  int _tree_depth = 0;

  while (1) {
    if (at >= path) {
      break;
    }
    int found = 0;
    debug_print(INFO, "Searching for %s", at);
    foreach(child, node->children) {
      tree_node_t * tchild = (tree_node_t *)child->value;
      struct vfs_entry * ent = (struct vfs_entry *)tchild->value;
      if (!strcmp(ent->name, at)) {
        found = 1;
        node = tchild;
        at = at + strlen(at) + 1;
        if (ent->file) {
          _tree_depth = _depth;
          last = ent->file;
          *outpath = at;
        }
        break;
      }
    }
    if (!found) {
      break;
    }
    _depth++;
  }

  *outdepth = _tree_depth;

  if (last) {
    fs_node_t * last_clone = (fs_node_t *)malloc(sizeof(fs_node_t));
    memcpy(last_clone, last, sizeof(fs_node_t));
    return last_clone;
  }
  return last;
}

fs_node_t *kopen_recur(char *filename, uint32_t flags, uint32_t symlink_depth, char *relative_to) {
  /* Simple sanity checks that we actually have a file system */
  if (!filename) {
    return NULL;
  }

  /* Canonicalize the (potentially relative) path... */
  char *path = canonicalize_path(relative_to, filename);
  /* And store the length once to save recalculations */
  size_t path_len = strlen(path);

  /* If strlen(path) == 1, then path = "/"; return root */
  if (path_len == 1) {
    /* Clone the root file system node */
    fs_node_t *root_clone = (fs_node_t *)malloc(sizeof(fs_node_t));
    memcpy(root_clone, fs_root, sizeof(fs_node_t));

    /* Free the path */
    free(path);

    open_fs(root_clone, flags);

    /* And return the clone */
    return root_clone;
  }

  /* Otherwise, we need to break the path up and start searching */
  char *path_offset = path;
  uint32_t path_depth = 0;
  while (path_offset < path + path_len) {
    /* Find each PATH_SEPARATOR */
    if (*path_offset == PATH_SEPARATOR) {
      *path_offset = '\0';
      path_depth++;
    }
    path_offset++;
  }
  /* Clean up */
  path[path_len] = '\0';
  path_offset = path + 1;

  /*
   * At this point, the path is tokenized and path_offset points
   * to the first token (directory) and path_depth is the number
   * of directories in the path
   */

  /*
   * Dig through the (real) tree to find the file
   */
  unsigned int depth = 0;
  /* Find the mountpoint for this file */
  fs_node_t *node_ptr = get_mount_point(path, path_depth, &path_offset, &depth);
  debug_print(INFO, "path_offset: %s", path_offset);
  debug_print(INFO, "depth: %d", depth);

  if (!node_ptr) return NULL;

  if (path_offset >= path+path_len) {
    free(path);
    open_fs(node_ptr, flags);
    return node_ptr;
  }
  fs_node_t *node_next = NULL;
  for (; depth < path_depth; ++depth) {
    /* Search the active directory for the requested directory */
    debug_print(INFO, "... Searching for %s", path_offset);
    node_next = finddir_fs(node_ptr, path_offset);
    free(node_ptr); /* Always a clone or an unopened thing */
    node_ptr = node_next;
    if (!node_ptr) {
      /* We failed to find the requested directory */
      free((void *)path);
      return NULL;
    }
    /* 
     * This test is a little complicated, but we basically always resolve symlinks in the
     * of a path (like /home/symlink/file) even if O_NOFOLLOW and O_PATH are set. If we are
     * on the leaf of the path then we will look at those flags and act accordingly
     */
    if ((node_ptr->flags & FS_SYMLINK) &&
        !((flags & O_NOFOLLOW) && (flags & O_PATH) && depth == path_depth - 1)) {
      /* This ensures we don't return a path when NOFOLLOW is requested but PATH
       * isn't passed.
       */
      debug_print(NOTICE, "resolving symlink at %s", node_ptr->name);
      if ((flags & O_NOFOLLOW) && depth == path_depth - 1) {
        /* TODO(gerow): should probably be setting errno from this */
        debug_print(NOTICE, "Refusing to follow final entry for open with O_NOFOLLOW for %s.", node_ptr->name);
        free((void *)path);
        free(node_ptr);
        return NULL;
      }
      if (symlink_depth >= MAX_SYMLINK_DEPTH) {
        /* TODO(gerow): should probably be setting errno from this */
        debug_print(WARNING, "Reached max symlink depth on %s.", node_ptr->name);
        free((void *)path);
        free(node_ptr);
        return NULL;
      }
      /* 
       * This may actually be big enough that we wouldn't want to allocate it on
       * the stack, especially considering this function is called recursively
       */
      char symlink_buf[MAX_SYMLINK_SIZE];
      int len = node_ptr->readlink(node_ptr, symlink_buf, sizeof(symlink_buf));
      if (len < 0) {
        /* TODO(gerow): should probably be setting errno from this */
        debug_print(WARNING, "Got error %d from symlink for %s.", len, node_ptr->name);
        free((void *)path);
        free(node_ptr);
        return NULL;
      }
      if (symlink_buf[len] != '\0') {
        /* TODO(gerow): should probably be setting errno from this */
        debug_print(WARNING, "readlink for %s doesn't end in a null pointer. That's weird...", node_ptr->name);
        free((void *)path);
        free(node_ptr);
        return NULL;
      }
      fs_node_t * old_node_ptr = node_ptr;
      /* Rebuild our path up to this point. This is hella hacky. */
      char * relpath = (char *)malloc(path_len + 1);
      char * ptr = relpath;
      memcpy(relpath, path, path_len + 1);
      for (unsigned int i = 0; i < depth; i++) {
        while(*ptr != '\0') {
          ptr++;
        }
        *ptr = PATH_SEPARATOR;
      }
      node_ptr = kopen_recur(symlink_buf, 0, symlink_depth + 1, relpath);
      free(relpath);
      free(old_node_ptr);
      if (!node_ptr) {
        /* Dangling symlink? */
        debug_print(WARNING, "Failed to open symlink path %s. Perhaps it's a dangling symlink?", symlink_buf);
        free((void *)path);
        return NULL;
      }
    }
    if (depth == path_depth - 1) {
      /* We found the file and are done, open the node */
      open_fs(node_ptr, flags);
      free((void *)path);
      return node_ptr;
    }
    /* We are still searching... */
    path_offset += strlen(path_offset) + 1;
  }
  debug_print(INFO, "- Not found.");
  /* We failed to find the requested file, but our loop terminated. */
  free((void *)path);
  return NULL;
}

/**
 * kopen: Open a file by name.
 *
 * Explore the file system tree to find the appropriate node for
 * for a given path. The path can be relative to the working directory
 * and will be canonicalized by the kernel.
 *
 * @param filename Filename to open
 * @param flags    Flag bits for read/write mode.
 * @returns A file system node element that the caller can free.
 */
fs_node_t *kopen(char *filename, uint32_t flags, char *wd) {
  debug_print(NOTICE, "kopen(%s)", filename);

  return kopen_recur(filename, flags, 0, wd);
}