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

// ##############################
#define DEBUG 1

#if DEBUG
    FILE* fdw;

	#define LOG_NUM(x) fprintf(fdw, "%s: %d\n", #x, x);
	#define LOG_CHAR(x, switcher)   	    \
		( (switcher) == 0 ? 				\
		(fprintf(fdw, "%s\n", x)) :    		\
		(fprintf(fdw, "%s: %s\n", #x, x)) ); 
#else
	#define LOG(x) 
#endif
// ##############################

#define ARR_SIZE 1024
#define BUFF_SIZE 5    // кол-во байт, считываемых за раз 

pthread_t tid1, tid2;
pthread_mutex_t mutex;

struct Arg_struct {
        char *dir_arr[ARR_SIZE];
        char *choices[ARR_SIZE];
        char path[ARR_SIZE];
        int size;
        int dir_size;
        int highlight;
        WINDOW *window;
};

struct pthread_struct {
    char *filename;
    char *pBarName;
}pthreadStruct;

// model.c
void scaner(struct Arg_struct *params);

// view.c
void printList(struct Arg_struct *params);
void boxTitle(WINDOW *wnd, int box_x, int box_y, int line_y, int line_x, int line_w, int lt_x, int rt_x);
void printTitle(WINDOW *win, int starty, int startx, int width, char string[], chtype color);
void createSubwindow(struct Arg_struct *params, int x);
void displayFunc(struct Arg_struct *params);
void reloadWinFunc(struct Arg_struct *params);
void *progressBar(void *param);

// controller.c
void enterFunc(struct Arg_struct *params);
void switchFunc(struct Arg_struct *params, int *cycle, int *win_tab);

// copy.c
void renameFunc(char path_r[], char path_w[]);
void *copyFunc(void *param);


#endif