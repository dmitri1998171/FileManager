#ifndef HEADER_H
#define HEADER_H

#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <errno.h>
#include <fcntl.h> 
#include <pthread.h>
#include <dirent.h> 
#include <sys/types.h> 
#include <sys/param.h> 
#include <sys/stat.h>
#include <sys/wait.h>
#include <ncurses.h>

#define ARR_SIZE 1024
#define BUFF_SIZE 5    // кол-во байт, считываемых за раз 

pthread_t tid1, tid2;
pthread_mutex_t mutex;

WINDOW *windows[2];

struct Arg_struct {
        char dir_arr[ARR_SIZE];
        char path[ARR_SIZE];
        char choices[ARR_SIZE];
        int size;
        int dir_size;
        int highlight;
};

struct pthread_struct {
    char *filename;
    char *pBarName;
}pthreadStruct;

void enterFunc(struct Arg_struct *params);
// void enterFunc(WINDOW *win, char *choices[], char *path, char *dir_arr[], int *highlight, int *size, int *dir_size);

void switchFunc(struct Arg_struct *params, int *cycle, int *win_tab);
// void switchFunc(WINDOW *win, char *path, char *dir_arr[], int *dir_size, char *choices[], int *c, int *highlight, int *size, int *cycle, int *win_tab);

void scaner(WINDOW *window, struct Arg_struct *params);
void boxTitle(WINDOW *wnd, int box_x, int box_y, int line_y, int line_x, int line_w, int lt_x, int rt_x);
void printTitle(WINDOW *win, int starty, int startx, int width, char string[], chtype color);
void interfaceFunc(WINDOW **win, struct Arg_struct *params, int x);
void displayFunc(struct Arg_struct *params);
void reloadWinFunc(WINDOW *win, char *choices[], char *path, char *dir_arr[], int *highlight, int *size, int *dir_size);

void *progressBar(void *param);

void renameFunc(char path_r[], char path_w[]);
void *copyFunc(void *param);


#endif