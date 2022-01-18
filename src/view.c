#include "../include/header.h"

#define BORDER 2

void printList(struct Arg_struct params[2], int win_tab) {
	int x = 2, y = 3;
	box(params[win_tab].window, 0, 0);

    for (int i = 0; i < params[win_tab].size; i++) {
		// High light the present choice 
		if(params[win_tab].highlight == i + 1) { 
            wattron(params[win_tab].window, A_REVERSE); 
			mvwprintw(params[win_tab].window, y, x, "%s", params[win_tab].choices[i]);
            wattroff(params[win_tab].window, A_REVERSE);
		}
		else
			mvwprintw(params[win_tab].window, y, x, "%s", params[win_tab].choices[i]);
        
		y++;
    }

	wrefresh(params[win_tab].window);
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

    /* Рисуем границы */
    printTitle(params->window, 1, 0, COLS/2, params->path, COLOR_PAIR(1));
    boxTitle(params->window, 0, 0, 2, 1, COLS/2-2, 0, COLS/2-1);

	wrefresh(params->window);
}

void displayFunc(struct Arg_struct params[2], int win_tab) {
    initscr();
    curs_set(0);
    start_color();
	noecho();
	cbreak();
    init_color(COLOR_BLACK, 140, 140, 140);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    
    char *title = "---------- File Manager ----------";
    printTitle(stdscr, 1, 0, COLS, title, COLOR_PAIR(1));
	mvprintw(LINES - 1, 1, "Tab - Switch panel  F1 - Quit  F5 - Copy  F8 - Delete");

    params[0].window = newwin(LINES-4, COLS/2, 3, 0);         // Левое окно
    params[1].window = newwin(LINES-4, COLS/2, 3, COLS/2);    // Правое окно

    for (int i = 0; i < 2; i++) {
        keypad(params[i].window, TRUE);
		params[i].highlight = 1;
        updateSubwindow(params, i);
	}
}

inline void updateSubwindow(struct Arg_struct params[2], int win_tab) {
    getcwd(params[win_tab].path, ARR_SIZE);         // Получ. путь
    LOG_CHAR(LOG_DEBAG, params[win_tab].path)
    wclear(params[win_tab].window);                 // Очищ. окно
    scaner(params);                                 // Сканируем директорию
    printList(params, win_tab);	                    // Выводим на экран
     
    printTitle(params[win_tab].window, 1, 0, COLS/2, params[win_tab].path, COLOR_PAIR(1));
    boxTitle(params[win_tab].window, 0, 0, 2, 1, COLS/2-2, 0, COLS/2-1);

    wrefresh(params[win_tab].window);
}

void *progressBar(void *param) {
    int h = 6, w = 50;
    WINDOW *mycopywin;
    WINDOW *mysubwin;

    struct pthread_struct *threadStruct = (struct pthread_struct *) param;  // ?

    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    
    mycopywin = newwin(h, w, (LINES/2) - (h / 2), (COLS/2) - (w / 2));
    box(mycopywin, 0, 0);

    // Создаем окно, устан. заголовок
    wattron(mycopywin, COLOR_PAIR(1));
    mvwprintw(mycopywin, h - 5, BORDER, threadStruct->pBarName);
    wattroff(mycopywin, COLOR_PAIR(1));
    mvwprintw(mycopywin, h - 5, BORDER * 4, threadStruct->filename);

    mysubwin = derwin(mycopywin, 3, w - BORDER, BORDER, 1);
    box(mysubwin, 0, 0);

    // Полоска загрузки
    wattroff(mycopywin, COLOR_PAIR(1));
    for(int i = 0; i < w; i++) {
        mvwprintw(mycopywin, h - 5, strlen(threadStruct->filename) + 10, "%d%%", (i * 2) + BORDER);  // Проценты
        
        if(i < w - (BORDER * 2))
            mvwaddch(mycopywin, h - 3, i + BORDER, '#');         // Полоска
        
        wrefresh(mycopywin);
        usleep(10000);
    }

    getch();

    delwin(mysubwin);
    delwin(mycopywin);
    
    return NULL;
}

