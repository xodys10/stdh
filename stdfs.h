#ifndef STDFS_H
#define STDFS_H

#include <fcntl.h>
#include <include/unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define STDFS_BUF_SIZE 1048576
#define STDFS_MAX_PATH 512

typedef struct {
    int fd;
    void *mapped_memory;
    size_t file_size;
    char path[STDFS_MAX_PATH];
} st_file;

st_file* fOpen(const char *path, int flags) {
    st_file* file = malloc(sizeof(st_file));
    if (!file) {
        return NULL;
    }
    file->fd = open(path, flags | O_DIRECT, S_IRUSR | S_IWUSR);
    if (file->fd == -1) {
        free(file);
        return NULL;
    }
    struct stat statbuf;
    if (fstat(file->fd, &statbuf) == -1) {
        close(file->fd);
        free(file);
        return NULL;
    }
    file->file_size = statbuf.st_size;
    snprintf(file->path, STDFS_MAX_PATH, "%s", path);
    file->mapped_memory = mmap(NULL, file->file_size, PROT_READ | PROT_WRITE, MAP_SHARED, file->fd, 0);
    if (file->mapped_memory == MAP_FAILED) {
        close(file->fd);
        free(file);
        return NULL;
    }
    return file;
}

ssize_t fRead(st_file* file, void *buffer, size_t size, off_t offset) {
    if (!file || !file->mapped_memory) {
        return -1;
    }
    if (offset + size > file->file_size) {
        size = file->file_size - offset;
    }
    memcpy(buffer, (char*)file->mapped_memory + offset, size);
    return size;
}

ssize_t fWrite(st_file* file, const void *buffer, size_t size, off_t offset) {
    if (!file || !file->mapped_memory) {
        return -1;
    }
    if (offset + size > file->file_size) {
        return -1;
    }
    memcpy((char*)file->mapped_memory + offset, buffer, size);
    msync(file->mapped_memory, file->file_size, MS_SYNC);
    return size;
}

void fClose(st_file* file) {
    if (!file) return;
    msync(file->mapped_memory, file->file_size, MS_SYNC);
    munmap(file->mapped_memory, file->file_size);
    close(file->fd);
    free(file);
}

int fExists(const char *path) {
    struct stat statbuf;
    return (stat(path, &statbuf) == 0);
}

#endif // STDFS_H
