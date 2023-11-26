#ifndef HEADER_H
#define HEADER_H

#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <ctype.h> 
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
            (fprintf(fdw, "%s | %s():%d | %s | %s= %d\n", __FILE__, __FUNCTION__, __LINE__, #log_level, #x, x)) :\
            (fprintf(fdw, "%s, %d\n", #log_level, x)) ); 

	#define LOG_CHAR(log_level, x)   	    \
        ( (log_level == LOG_DEBUG) ? 		\
            (fprintf(fdw, "%s | %s():%d | %s | %s= %c\n", __FILE__, __FUNCTION__, __LINE__, #log_level, #x, x)) :\
            (fprintf(fdw, "%s\n", x)) );	

	#define LOG_STR(log_level, x)   	    \
        ( (log_level == LOG_DEBUG) ? 		\
            (fprintf(fdw, "%s | %s():%d | %s | %s= %s\n", __FILE__, __FUNCTION__, __LINE__, #log_level, #x, x)) :\
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

// Структура для подсчета кол-ва эл-ов в директории
typedef struct EntitiesCounter_struct {
	int fileCounter;
	int dirCounter;
	int linkCounter;
	int total;
} EntitiesCounter;

typedef struct Entity_struct {
    unsigned char type;
    int size;
    char modify_time[MOD_TIME_SIZE];
    char name[STR_SIZE];
}Entity;

typedef struct Directory_struct {
    EntitiesCounter counter;        // Обшее кол-во сушностей в текущ. директории
    char path[ARR_SIZE];            // Путь к текущ. директории
    Entity *entity;                 // Сущность: папка, файл, ссылка
    int highlight;                  // Выделенный элемент
    WINDOW *window;
    PANEL  *panel;
}Directory;

typedef struct Tab_struct {
    int highlight;
    int linesCounter;
    char **panel_btns;
    WINDOW *win;
    PANEL  *panel;
}Tab;

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
void scaner(Directory directory[2], int win_tab);
int countLines(char **arr, int totalLines);
void bubbleSort(Entity list[], int size);
void sortByAlpha(Directory directory[2], int win_tab);
void sortByType(Directory directory[2], int win_tab);
void sortBySize(Directory directory[2], bool direction);
void sortByTime(Directory directory[2]);

// view.c
int colornum(int fg, int bg);
short curs_color(int fg);
void init_colorpairs();
void init();
void printList(Directory directory[2], int win_tab);
void showTabButtons(Tab tabs[3]);
void boxTitle(WINDOW *wnd, int box_x, int box_y, int line_y, int line_x, int line_w);
void printTitle(WINDOW *win, int starty, int startx, int width, char string[], chtype color);
void displayFunc(Directory directory[2], int win_tab);
void redrawSubwindow(Directory directory[2], int win_tab);
void updateSubwindow(Directory directory[2], int win_tab);
void *progressBar(void *param);
void addElem(Tab tabs[3], int panel, char* str);
void init_tabs(Directory directory[2], Tab tabs[3], int win_tab);

// controller.c
void enterFunc(Directory *directory, int win_tab);
void switchFunc(Directory directory[2], Tab tabs[3], int *cycle, int *win_tab);
void showTab(Directory directory[2], Tab tabs[3], int *win_tab);

// copy.c
void renameFunc(char path_r[], char path_w[]);
void *copyFunc(void *param);


#endif