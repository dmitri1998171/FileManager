#include <ncurses.h>
#include <dirent.h> 
#include <sys/types.h> 
#include <sys/param.h> 
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <errno.h>
#include <wait.h>
#include <pthread.h>

#define ARR_SIZE 255
#define BUFF_SIZE 5    // кол-во байт, считываемых за раз 

char *dir_arr_left[ARR_SIZE];
char *dir_arr_right[ARR_SIZE];
char path_left[ARR_SIZE];
char path_right[ARR_SIZE];
char *choices_right[ARR_SIZE];
char *choices_left[ARR_SIZE];
int size_left, size_right;
int dir_size_left, dir_size_right;
int highlight_left = 1, highlight_right = 1;
WINDOW *win_left;
WINDOW *win_right;

pthread_t tid1, tid2;
pthread_mutex_t mutex;
char *buf[BUFF_SIZE];
int fdr, fdw, ret;

void print_menu(WINDOW *menu_win, int highlight, char *choices[], int *size);
void switchFunc(WINDOW *win, char *path, char *dir_arr[], int *dir_size, char *choices[], int *c, int *highlight, 
    int *size, int *cycle, int *win_tab);
void enterFunc(WINDOW *, char **, char *, char **, int *, int *, int *);
// void copyInterfaceFunc(char*);

void scaner(char path[], char *choices[], char *dir_arr[], int *size, int *dir_size){
	int i=0, f=0;
	struct dirent *dir;
    DIR *d;

	d = opendir(path);
	if( d == NULL ){ perror("opendir"); exit(1); }

	while((dir = readdir(d))){
		if(strcmp( dir->d_name, "." ) == 0){
			continue;
	    }
        choices[i] = dir->d_name;

		if(dir->d_type == DT_DIR){
			dir_arr[f] = dir->d_name;
			f++;
		} 
        i++;
	}
    *size = i;
    *dir_size = f;
	closedir(d);
}

void box_title(WINDOW *wnd, int box_x, int box_y, int line_y, int line_x, int line_w, int lt_x, int rt_x){
    box(wnd, box_y, box_x);
	mvwhline(wnd, line_y, line_x, ACS_HLINE, line_w);
}

void print_title(WINDOW *win, int starty, int startx, int width, char string[], chtype color){	
    int length, x, y;
	float temp;

	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}

void interfaceFunc(WINDOW **win, char *path, int *size, int x){
    /* Создаем окно */
    *win = newwin(LINES-4, COLS/2, 3, x);
    keypad(*win, TRUE);

    /* Рисуем границы */
    print_title(*win, 1, 0, COLS/2, path, COLOR_PAIR(1));
    box_title(*win, 0, 0, 2, 1, COLS/2-2, 0, COLS/2-1);

	wrefresh(*win);
}

void displayFunc(){
    initscr();
    curs_set(0);
    start_color();
	noecho();
	cbreak();
    init_color(COLOR_BLACK, 140, 140, 140);
    init_pair(1, COLOR_RED, COLOR_BLACK);

    chdir(".");
    getcwd(path_left, ARR_SIZE);
    getcwd(path_right, ARR_SIZE);

    scaner(path_left, choices_left, dir_arr_left, 
            &size_left, &dir_size_left);
    scaner(path_right, choices_right, dir_arr_right, 
            &size_right, &dir_size_right);
    printf("\n");

    char *title="---------- File Manager ----------";
    print_title(stdscr, 1, 0, COLS, title, COLOR_PAIR(1));
	mvprintw(LINES - 1, 1, "Tab - Switch panel  F1 - Quit  F5 - Copy");

    interfaceFunc(&win_left, path_left, &size_left, 0);
    interfaceFunc(&win_right, path_right, &size_right, COLS/2);
    refresh();

	print_menu(win_left, highlight_left, choices_left, &size_left);
    print_menu(win_right, highlight_right, choices_right, &size_right);
}

int main(){	
	int c, cycle = 1, win_tab = 0;
    pthread_mutex_init(&mutex, NULL);

    displayFunc();
    
	while(cycle)
	{	
        /* переключение между окнами */
        if(win_tab == 0){
            c = wgetch(win_left); 
            switchFunc(win_left, path_left, dir_arr_left, 
                    &dir_size_left, choices_left, &c, &highlight_left, 
                    &size_left, &cycle, &win_tab);

            print_menu(win_left, highlight_left, choices_left, 
                    &size_left);
        }
        if(win_tab == 1){
            c = wgetch(win_right);
            switchFunc(win_right, path_right, dir_arr_right, 
                    &dir_size_right, choices_right, &c, &highlight_right, 
                    &size_right, &cycle, &win_tab);
            
            print_menu(win_right, highlight_right, choices_right, 
                    &size_right);   
        }
	}	
    pthread_mutex_destroy(&mutex);
	endwin();
	return 0;
}

void reloadWinFunc(WINDOW *win, char *choices[], char *path, 
    char *dir_arr[], int *highlight, int *size, int *dir_size){
    
    getcwd(path, ARR_SIZE);
    wclear(win);
    scaner(path, choices, dir_arr, size, dir_size);
    
    print_title(win, 1, 0, COLS/2, path, COLOR_PAIR(1));
    box_title(win, 0, 0, 2, 1, COLS/2-2, 0, COLS/2-1);
}

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

    pthread_mutex_lock(&mutex);
    strcpy(path_r, path);
    pthread_mutex_unlock(&mutex);

    renameFunc(path_r, path_w);
    
    fdr = open(path_r, O_RDONLY);
    fdw = open(path_w, O_CREAT | O_WRONLY, S_IRWXU);

    while((ret = read(fdr, &buf, BUFF_SIZE)) != 0){
        if(ret == -1) perror("Read error");

        write(fdw, &buf, BUFF_SIZE);
    }

    close(fdr);
    close(fdw);
    return NULL;
}

void *copyInterfaceFunc(void *param){
    pthread_mutex_lock(&mutex);
    char *name = (char*) param;
    pthread_mutex_unlock(&mutex);
    int h=6, w=50; 
    WINDOW *mycopywin;
    WINDOW *mysubwin;

    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    
    mycopywin = newwin(h, w, (LINES/2)-(h/2), (COLS/2)-(w/2));
    box(mycopywin, 0, 0);

    wattron(mycopywin, COLOR_PAIR(1));
    mvwprintw(mycopywin, h-5, 2, "Copy: ");
    wattroff(mycopywin, COLOR_PAIR(1));
    mvwprintw(mycopywin, h-5, 8, name);

    mysubwin = derwin(mycopywin, 3, w-2, 2, 1);
    box(mysubwin, 0,0);

    wattroff(mycopywin, COLOR_PAIR(1));
    for(int n = 0; n < w; n++){
        mvwaddch(mycopywin, h-3, n+2, '#');
        mvwprintw(mycopywin, h-5, strlen(name)+10, "%d%%", (n*2)+6);
        wrefresh(mycopywin);
        usleep(100000);
    }
    wgetch(mycopywin);
    delwin(mysubwin);
    delwin(mycopywin);
    return NULL;
}

void switchFunc(WINDOW *win, char *path, char *dir_arr[], 
                int *dir_size, char *choices[], int *c, 
                int *highlight, int *size, int *cycle, 
                int *win_tab){
    switch(*c){
        case KEY_DOWN:
            if(highlight == size)
                *highlight = 1;
            else 
                ++*highlight;  
            break;
        case KEY_UP:
            if(*highlight == 1)
                highlight = size;
            else
                --*highlight;
            break;

        case KEY_F(1):
            *cycle = 0;
            break;
        case KEY_F(5):            
            pthread_create(&tid1, NULL, copyFunc, choices[*highlight-1]);
            pthread_create(&tid2, NULL, copyInterfaceFunc, choices[*highlight-1]);

            pthread_join(tid1, NULL);
            pthread_join(tid2, NULL);

            reloadWinFunc(win, choices, path, dir_arr, 
                        highlight, size, dir_size);
            break;

        case '\t':
            *win_tab += 1;
            if(*win_tab > 1) *win_tab = 0;
            break;
        case 10:
            enterFunc(win, choices, path, 
            dir_arr, highlight, size, dir_size);
            break;

    }
}

void enterFunc(WINDOW *win, char *choices[], char *path, 
    char *dir_arr[], int *highlight, int *size, int *dir_size){   

    int check = 0;
    for(int i=0; i < *dir_size; i++){
        if(strcmp(choices[*highlight-1], dir_arr[i]) == 0){
            chdir(choices[*highlight-1]);
            
            reloadWinFunc(win, choices, path, dir_arr, highlight, size, dir_size);

            *highlight = 1;
            check +=1;   
        }
    }
    if(check == 0){
        pid_t pid = fork();
        if(pid == 0){
            endwin();
            execl(choices[*highlight-1], choices[*highlight-1], NULL);
            exit(0);
        }
        wait(&pid);
        displayFunc();
    }
    refresh();
    wrefresh(win_left);
    wrefresh(win_right);
}

void print_menu(WINDOW *menu_win, int highlight, char *choices[], int *size){
	int i, x = 2, y = 3;

	box(menu_win, 0, 0);
	for(i = 0; i < *size; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}
