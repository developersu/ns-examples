#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#include <unistd.h> // rmdir(), unlink()
#include <errno.h>  // DEBUG

// Create file
int touch(const char* path){
    FILE *fd;
    fd = fopen(path, "wb");
    if (fd == NULL)
        return -1;
    return fclose(fd);
}

// Read what's inside dir
void lsDir(const char* dir_name){
    DIR* dir;
    struct dirent* ent;

    dir = opendir(dir_name);//Open current-working-directory.
    if (dir == NULL) {
        printf("Failed to open dir: %s\n", dir_name);
    }
    else {
        printf("ls %s\n", dir_name);

        while ((ent = readdir(dir))) 
            printf("d_name: %s d_type: %u\n", ent->d_name, (unsigned) ent->d_type);
        closedir(dir);
    }
}

// Check is Directory
int isDir(const char* path){
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

// Remove entry; dir_name should not have / on the end
int removeAll(const char* dir_name){
    printf("ra() %i %s\n", isDir(dir_name), dir_name);
    int retVal;

    if (! isDir(dir_name))
        return unlink(dir_name);

    printf("This is dir\n");

    DIR *dir = opendir(dir_name);

    if (dir == NULL)
        return 1;

    printf("No doubts\n");
    
    size_t dSz = strlen(dir_name);
    struct dirent *s_dirent;
    char* full_name;

    while ((s_dirent = readdir(dir)) != NULL){
        if ((strcmp(s_dirent->d_name, ".") == 0) || (strcmp(s_dirent->d_name, "..") == 0))
            continue;
        full_name = malloc(dSz + strlen(s_dirent->d_name) + 2); // '/'+'\0'

        strcpy(full_name, dir_name);
        strcat(full_name, "/");
        strcat(full_name, s_dirent->d_name);    
        
        if (s_dirent->d_type == DT_DIR)
            removeAll(full_name);
        else{
            retVal = unlink(full_name);      // NOTE: Add validation
            printf("I: %s - %i - %s\n", full_name, retVal, strerror(errno)); // DEBUG
        }

        free(full_name);
    }

    closedir(dir);

    retVal = rmdir(dir_name);           // NOTE: Add validation
    printf("O: %s - %i - %s\n", dir_name, retVal, strerror(errno)); // DEBUG
    
    return 0;
}
