#include "../include/header.h"

void printList(struct Arg_struct *params) {
	int x = 2, y = 3;
	box(params->window, 0, 0);

    for (int i = 0; i < params->size; i++) {
		// High light the present choice 
		if(params->highlight == i + 1) { 
            wattron(params->window, A_REVERSE); 
			mvwprintw(params->window, y, x, "%s", params->choices[i]);
            wattroff(params->window, A_REVERSE);
		}
		else
			mvwprintw(params->window, y, x, "%s", params->choices[i]);
        
		y++;
    }

	wrefresh(params->window);
}

inline void boxTitle(WINDOW *wnd, int box_x, int box_y, int line_y, int line_x, int line_w, int lt_x, int rt_x) {
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

void createSubwindow(struct Arg_struct *params, int x) {
    /* Создаем подокно */
    params->window = newwin(LINES-4, COLS/2, 3, x);
    keypad(params->window, TRUE);

    LOG_CHAR(params->path, 0);
    
    /* Рисуем границы */
    printTitle(params->window, 1, 0, COLS/2, params->path, COLOR_PAIR(1));
    boxTitle(params->window, 0, 0, 2, 1, COLS/2-2, 0, COLS/2-1);

	wrefresh(params->window);
}

void displayFunc(struct Arg_struct *params) {
    initscr();
    curs_set(0);
    start_color();
	noecho();
	cbreak();
    init_color(COLOR_BLACK, 140, 140, 140);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    
    char *title = "---------- File Manager ----------";
    printTitle(stdscr, 1, 0, COLS, title, COLOR_PAIR(1));
	mvprintw(LINES - 1, 1, "Tab - Switch panel  F1 - Quit  F5 - Copy");

	createSubwindow(&params[0], 0);
    createSubwindow(&params[1], COLS/2);

    for (int i = 0; i < 2; i++) {
		params[i].highlight = 1;
		getcwd(params[i].path, ARR_SIZE);		// Получ. путь
		scaner(&params[i]);						// Сканируем директорию
		printList(&params[i]);					// Выводим на экран
	}
}

void reloadWinFunc(WINDOW *win, char *choices[], char *path, char *dir_arr[], int *highlight, int *size, int *dir_size) {
    getcwd(path, ARR_SIZE);
    wclear(win);
    // scaner(&params);
    
    printTitle(win, 1, 0, COLS/2, path, COLOR_PAIR(1));
    boxTitle(win, 0, 0, 2, 1, COLS/2-2, 0, COLS/2-1);
}

void *progressBar(void *param) {
    int h=6, w=50;
    WINDOW *mycopywin;
    WINDOW *mysubwin;

    struct pthread_struct *threadStruct = (struct pthread_struct *) param;  // ?

    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    
    mycopywin = newwin(h, w, (LINES/2)-(h/2), (COLS/2)-(w/2));
    box(mycopywin, 0, 0);

    wattron(mycopywin, COLOR_PAIR(1));
    mvwprintw(mycopywin, h-5, 2, threadStruct->pBarName);
    wattroff(mycopywin, COLOR_PAIR(1));
    mvwprintw(mycopywin, h-5, 8, threadStruct->filename);

    mysubwin = derwin(mycopywin, 3, w-2, 2, 1);
    box(mysubwin, 0,0);

    wattroff(mycopywin, COLOR_PAIR(1));
    for(int n = 0; n < w; n++) {
        mvwaddch(mycopywin, h-3, n+2, '#');
        mvwprintw(mycopywin, h-5, strlen(threadStruct->filename)+10, "%d%%", (n*2)+6);
        wrefresh(mycopywin);
        usleep(100000);
    }

    delwin(mysubwin);
    delwin(mycopywin);
    
    return NULL;
}

