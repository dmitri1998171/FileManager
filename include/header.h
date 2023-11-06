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

    enum log_level_t {LOG_ERROR, LOG_WARNING, LOG_INFO, LOG_DEBUG};

	#define LOG_NUM(log_level, x)           \
        ( (log_level == LOG_DEBUG) ?        \
            (fprintf(fdw, "%s, %s: %d\n", #log_level, #x, x)) :\
            (fprintf(fdw, "%s, %d\n", #log_level, x)) ); 

	#define LOG_CHAR(log_level, x)   	    \
        ( (log_level == LOG_DEBUG) ? 		\
            (fprintf(fdw, "%s, %s: %s\n", #log_level, #x, x)) :\
            (fprintf(fdw, "%s\n", x)) );		
#else
	#define LOG(x) 
#endif
// ##############################

#define NLINES 10
#define NCOLS 20
#define TAB_OFFSET 13
#define MOD_TIME_SIZE 80
#define STR_SIZE 255
#define ARR_SIZE 1024
#define BUFF_SIZE 5    // Кол-во байт, считываемых за раз 

pthread_t tid1, tid2;
pthread_mutex_t mutex;

struct Directory_struct {
    int size;                       // Обшее кол-во файлов в текущ. директории
    char path[ARR_SIZE];            // Путь к текущ. директории
    struct Entity_struct *entity;
    int highlight;
    WINDOW *window;
    PANEL  *panel;
};

struct Entity_struct {
    unsigned char type;
    int size;
    char modify_time[MOD_TIME_SIZE];
    char name[STR_SIZE];
};

struct Tab_struct {
    int highlight;
    int linesCounter;
    char panel_btns[NLINES][NCOLS];
    WINDOW *win;
    PANEL  *panel;
};

struct pthread_struct {
    char *filename;
    char *pBarName;
} pthreadStruct;

enum panel_states {
    LEFT_PANEL = 0,
    SETTINGS = 1,
    RIGHT_PANEL = 2,
    HIDE = 3
};

int panel_state;

// model.c
void scaner(struct Directory_struct directory[2], int win_tab);
int countLines(char arr[][NCOLS], int totalLines);

// view.c
void init();
void printList(struct Directory_struct directory[2], int win_tab);
void showTabButtons(struct Tab_struct tabs[3]);
void boxTitle(WINDOW *wnd, int box_x, int box_y, int line_y, int line_x, int line_w);
void printTitle(WINDOW *win, int starty, int startx, int width, char string[], chtype color);
void displayFunc(struct Directory_struct directory[2], int win_tab);
void redrawSubwindow(struct Directory_struct directory[2], int win_tab);
void updateSubwindow(struct Directory_struct directory[2], int win_tab);
void *progressBar(void *param);

// controller.c
void enterFunc(struct Directory_struct *directory, int win_tab);
void switchFunc(struct Directory_struct directory[2], struct Tab_struct tabs[3], int *cycle, int *win_tab);
void showTab(struct Directory_struct directory[2], struct Tab_struct tabs[3], int *win_tab);

// copy.c
void renameFunc(char path_r[], char path_w[]);
void *copyFunc(void *param);


#endif