#ifndef VFS_H
#define VFS_H

struct File {
    char *name;
    char *data;
};

File files[1028];
int files_index = 0;

void make_file(char name[256], char data[1028]) {
    files[files_index].name = name;
    files[files_index].data = data;
    files_index++;
}

File get_file(int index) {
    return files[index];
}

#endif