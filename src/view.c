#include "../include/header.h"

void enterFunc(struct Arg_struct *params) {   
    int check = 0;
    for(int i=0; i < params->dir_size; i++) {
        if(strcmp(&params->choices[params->highlight-1], &params->dir_arr[i]) == 0) {
            chdir(&params->choices[params->highlight-1]);
            
            // reloadWinFunc(win, choices, path, dir_arr, highlight, size, dir_size);

            params->highlight = 1;
            check +=1;   
        }
    }
    if(check == 0) {
        pid_t pid = fork();
        if(pid == 0) {
            endwin();
            execl(&params->choices[params->highlight-1], &params->choices[params->highlight-1], NULL);
            exit(0);
        }
        wait(&pid);
        displayFunc(params);
    }
    refresh();
    wrefresh(windows[0]);
    wrefresh(windows[1]);
}

void switchFunc(struct Arg_struct *params, int *cycle, int *win_tab) {
    int input = wgetch(windows[1]);

    switch(input) {
        case KEY_DOWN:
            if(params->highlight == params->size)
                params->highlight = 1;
            else 
                ++params->highlight;  
            
            break;

        case KEY_UP:
            if(params->highlight == 1) {
                params->highlight = params->size;
            }
            else
                --params->highlight;
            break;

        case KEY_F(1):
            *cycle = 0;
            break;

        case KEY_F(5):
            strcpy(pthreadStruct.filename, &params->choices[params->highlight-1]); 
            strcpy(pthreadStruct.pBarName, "Copy: "); 

            // pthread_create(&tid1, NULL, copyFunc, &params->choices[params->highlight-1]);
            // pthread_create(&tid2, NULL, progressBar, &pthreadStruct);

            // pthread_join(tid1, NULL);
            // pthread_join(tid2, NULL);

            // reloadWinFunc(windows[0], choices, path, dir_arr, highlight, size, dir_size);
            // reloadWinFunc(windows[1], choices, path, dir_arr, highlight, size, dir_size);

            break;

        case '\t':
            *win_tab += 1;
            if(*win_tab > 1) 
                *win_tab = 0;
            
            break;

        case 10:
            enterFunc(params);
            break;

    }
}

void scaner(WINDOW *window, struct Arg_struct *params) {
	int counter = 0, dir_counter = 0;
	struct dirent *dir_struct_t;
    DIR *dir;

	dir = opendir(params->path);
	if(dir == NULL) { 
        perror("opendir");
        exit(1); 
    }

	int x = 2, y = 3;
	box(window, 0, 0);

	while((dir_struct_t = readdir(dir))) {
		if(strcmp(dir_struct_t->d_name, ".") == 0) {
			continue;
	    }

        strcpy(&params->choices[counter], dir_struct_t->d_name);

		if(dir_struct_t->d_type == DT_DIR) {
			strcpy(&params->dir_arr[dir_counter], dir_struct_t->d_name);
			dir_counter++;
		} 

		// High light the present choice 
		if(params->highlight == counter + 1) { 
            wattron(window, A_REVERSE); 
			mvwprintw(window, y, x, "%s", &params->choices[counter]);
            wattroff(window, A_REVERSE);
		}
		else
			mvwprintw(window, y, x, "%s", &params->choices[counter]);
        
		y++;
        counter++;
	}
    
    params->size = counter;
    params->dir_size = dir_counter;
	closedir(dir);
	wrefresh(window);
}

void boxTitle(WINDOW *wnd, int box_x, int box_y, int line_y, int line_x, int line_w, int lt_x, int rt_x) {
    box(wnd, box_y, box_x);
	mvwhline(wnd, line_y, line_x, ACS_HLINE, line_w);
}

void printTitle(WINDOW *win, int starty, int startx, int width, char string[], chtype color) {	
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
    printf("\n");
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}

void interfaceFunc(WINDOW **win, struct Arg_struct *params, int x) {
    /* Создаем окно */
    *win = newwin(LINES-4, COLS/2, 3, x);
    keypad(*win, TRUE);

    /* Рисуем границы */
    printTitle(*win, 1, 0, COLS/2, params->path, COLOR_PAIR(1));
    boxTitle(*win, 0, 0, 2, 1, COLS/2-2, 0, COLS/2-1);

	wrefresh(*win);
}

void displayFunc(struct Arg_struct *params) {
    initscr();
    curs_set(0);
    start_color();
	noecho();
	cbreak();
    init_color(COLOR_BLACK, 140, 140, 140);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    
    char *title="---------- File Manager ----------";
    printTitle(stdscr, 1, 0, COLS, title, COLOR_PAIR(1));
	mvprintw(LINES - 1, 1, "Tab - Switch panel  F1 - Quit  F5 - Copy");

    interfaceFunc(&windows[0], &params[0], 0);
    interfaceFunc(&windows[1], &params[1], COLS/2);
    refresh();
}

void reloadWinFunc(WINDOW *win, char *choices[], char *path, char *dir_arr[], int *highlight, int *size, int *dir_size) {
    getcwd(path, ARR_SIZE);
    wclear(win);
    // scaner(&params);
    
    printTitle(win, 1, 0, COLS/2, path, COLOR_PAIR(1));
    boxTitle(win, 0, 0, 2, 1, COLS/2-2, 0, COLS/2-1);
}