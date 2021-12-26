#include "../include/header.h"

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

void enterFunc(WINDOW *win, char *choices[], char *path, char *dir_arr[], int *highlight, int *size, int *dir_size){   

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
    wrefresh(windows[0]);
    wrefresh(windows[1]);
}

void switchFunc(WINDOW *win, char *path, char *dir_arr[], int *dir_size, char *choices[], int *c, int *highlight, int *size, int *cycle, int *win_tab){
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
            strcpy(pthreadStruct.filename, choices[*highlight-1]); 
            strcpy(pthreadStruct.pBarName, "Copy: "); 

            pthread_create(&tid1, NULL, copyFunc, choices[*highlight-1]);
            pthread_create(&tid2, NULL, progressBar, &pthreadStruct);

            pthread_join(tid1, NULL);
            pthread_join(tid2, NULL);

            reloadWinFunc(windows[0], choices, path, dir_arr, highlight, size, dir_size);
            // reloadWinFunc(windows[1], choices, path, dir_arr, highlight, size, dir_size);

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
    getcwd(path[0], ARR_SIZE);
    getcwd(path[1], ARR_SIZE);

    scaner(path[0], choices[1], dir_arr[0], 
            &size[0], &dir_size[0]);
    scaner(path[1], choices[0], dir_arr[1], 
            &size[1], &dir_size[1]);
    printf("\n");

    char *title="---------- File Manager ----------";
    print_title(stdscr, 1, 0, COLS, title, COLOR_PAIR(1));
	mvprintw(LINES - 1, 1, "Tab - Switch panel  F1 - Quit  F5 - Copy");

    interfaceFunc(&windows[0], path[0], &size[0], 0);
    interfaceFunc(&windows[1], path[1], &size[1], COLS/2);
    refresh();

	print_menu(windows[0], highlight[0], choices[1], &size[0]);
    print_menu(windows[1], highlight[1], choices[0], &size[1]);
}

void reloadWinFunc(WINDOW *win, char *choices[], char *path, char *dir_arr[], int *highlight, int *size, int *dir_size){
    getcwd(path, ARR_SIZE);
    wclear(win);
    scaner(path, choices, dir_arr, size, dir_size);
    
    print_title(win, 1, 0, COLS/2, path, COLOR_PAIR(1));
    box_title(win, 0, 0, 2, 1, COLS/2-2, 0, COLS/2-1);
}