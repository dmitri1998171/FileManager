#include "../include/header.h"

void renameFunc(char path_r[], char path_w[]){
    char name[255];
    char ext[5];
    
    strcpy(path_w, path_r);
    strcpy(name, strtok(path_w, "."));
    strcpy(ext, strtok(NULL, "."));
    strcat(name, "(1).");
    strcat(name, ext);
    strcpy(path_w, name);
}

void* copyFunc(void *param){
    char *path = (char*) param;
    char path_r[ARR_SIZE];
    char path_w[ARR_SIZE];
    char *buf[BUFF_SIZE];
    int fdr, fdw, ret;

    pthread_mutex_lock(&mutex);
    strcpy(path_r, path);
    pthread_mutex_unlock(&mutex);

    renameFunc(path_r, path_w);
    
    fdr = open(path_r, O_RDONLY);
    fdw = open(path_w, O_CREAT | O_WRONLY, S_IRWXU);

    while((ret = read(fdr, &buf, BUFF_SIZE)) != 0){
        if(ret == -1) 
            perror("Read error");

        write(fdw, &buf, BUFF_SIZE);
    }

    close(fdr);
    close(fdw);
    return NULL;
}