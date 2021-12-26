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

#define ARR_SIZE 255
#define BUFF_SIZE 5    // кол-во байт, считываемых за раз 

pthread_t tid1, tid2;
pthread_mutex_t mutex;

char *dir_arr_left[ARR_SIZE];
char *dir_arr_right[ARR_SIZE];
char path_left[ARR_SIZE];
char path_right[ARR_SIZE];
char *choices_right[ARR_SIZE];
char *choices_left[ARR_SIZE];
int size_left, size_right;
int dir_size_left, dir_size_right;
int highlight_left, highlight_right;
WINDOW *windows[2];

struct pthread_struct {
    char *filename;
    char *pBarName;
}pthreadStruct;

void print_menu(WINDOW *menu_win, int highlight, char *choices[], int *size);
void enterFunc(WINDOW *win, char *choices[], char *path, char *dir_arr[], int *highlight, int *size, int *dir_size);
void switchFunc(WINDOW *win, char *path, char *dir_arr[], int *dir_size, char *choices[], int *c, int *highlight, int *size, int *cycle, int *win_tab);
void scaner(char path[], char *choices[], char *dir_arr[], int *size, int *dir_size);
void box_title(WINDOW *wnd, int box_x, int box_y, int line_y, int line_x, int line_w, int lt_x, int rt_x);
void print_title(WINDOW *win, int starty, int startx, int width, char string[], chtype color);
void interfaceFunc(WINDOW **win, char *path, int *size, int x);
void displayFunc();
void reloadWinFunc(WINDOW *win, char *choices[], char *path, char *dir_arr[], int *highlight, int *size, int *dir_size);

void *progressBar(void *param);

void renameFunc(char path_r[], char path_w[]);
void* copyFunc(void *param);


#endif