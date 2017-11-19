#ifndef TFS_H
#define TFS_H

#define START_FLAG ":tfs_start:"
#define END_FLAG ":tfs_end:"

#include <stddef.h>

typedef struct {
    size_t start;
    size_t end;
    size_t drive_size;
    //...
} filesystem_t;

void tfs_startup(filesystem_t *fs);
void tfs_uninstall(filesystem_t *fs);

#endif