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
#include <panel.h>

// ##############################
#define DEBUG 1

#if DEBUG
    FILE* fdw;

    enum log_level_t {LOG_ERROR, LOG_WARNING, LOG_INFO, LOG_DEBAG};

	#define LOG_NUM(log_level, x)           \
        ( (log_level == LOG_DEBAG) ?        \
            (fprintf(fdw, "%s, %s: %d\n", #log_level, #x, x)) :\
            (fprintf(fdw, "%s, %d\n", #log_level, x)) ); 

	#define LOG_CHAR(log_level, x)   	    \
        ( (log_level == LOG_DEBAG) ? 		\
            (fprintf(fdw, "%s, %s: %s\n", #log_level, #x, x)) :\
            (fprintf(fdw, "%s\n", x)) );		
#else
	#define LOG(x) 
#endif
// ##############################

#define NLINES 10
#define NCOLS 20
#define TAB_OFFSET 13
#define ARR_SIZE 1024
#define BUFF_SIZE 5    // кол-во байт, считываемых за раз 

pthread_t tid1, tid2;
pthread_mutex_t mutex;

struct Arg_struct {
    char *dir_arr[ARR_SIZE];        // Список папок в текущ. директории
    char *choices[ARR_SIZE];
    char path[ARR_SIZE];            // Путь к текущ. директории
    int size;                       // Обшее кол-во файлов в текущ. директории
    int dir_size;                   // Кол-во папок в текущ. директории
    int highlight;
    WINDOW *window;
};

WINDOW *wins[3];
PANEL  *tabs[5];

enum panel_states {
    LEFT_PANEL = 0,
    SETTINGS = 1,
    RIGHT_PANEL = 2,
    HIDE = 3
};

int panel_state;
int tab_btn_highlight;

struct pthread_struct {
    char *filename;
    char *pBarName;
} pthreadStruct;

// model.c
void scaner(struct Arg_struct params[2], int win_tab);
int countLines(char arr[][NCOLS], int totalLines);

// view.c
void init();
void printList(struct Arg_struct params[2], int win_tab);
void showTabButtons(char btns[NLINES][NCOLS], WINDOW* win, int lines);
void boxTitle(WINDOW *wnd, int box_x, int box_y, int line_y, int line_x, int line_w);
void printTitle(WINDOW *win, int starty, int startx, int width, char string[], chtype color);
void displayFunc(struct Arg_struct params[2], int win_tab);
void redrawSubwindow(struct Arg_struct params[2], int win_tab);
void updateSubwindow(struct Arg_struct params[2], int win_tab);
void *progressBar(void *param);

// controller.c
void enterFunc(struct Arg_struct *params, int win_tab);
void switchFunc(struct Arg_struct params[2], int *cycle, int *win_tab);

// copy.c
void renameFunc(char path_r[], char path_w[]);
void *copyFunc(void *param);


#endif