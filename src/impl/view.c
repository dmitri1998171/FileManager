#include "../../include/header.h"

#define BORDER 2
#define COLOR_SUPERYELLOW 20324

inline void init() {
    initscr();
    curs_set(0);
    start_color();
	noecho();
	cbreak();
    init_color(COLOR_BLACK, 140, 140, 140); 
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_YELLOW, COLOR_RED);
}

void printList(struct Arg_struct params[2], int win_tab) {
	int x = 2, y = 3;

    for (int i = 0; i < params[win_tab].size; i++) {
		// Highlight the present choice 
		if(params[win_tab].highlight == i + 1) { 
            wattron(params[win_tab].window, COLOR_PAIR(2)); 
			mvwprintw(params[win_tab].window, y, x, "%s", params[win_tab].choices[i]);
            wattroff(params[win_tab].window, COLOR_PAIR(2));
		}
		else
			mvwprintw(params[win_tab].window, y, x, "%s", params[win_tab].choices[i]);
        
		y++;
    }

	wrefresh(params[win_tab].window);
}

int countLines(char arr[][NCOLS], int totalLines) {
    int i = 0, lines = 0;
    
    while(i < totalLines) {
        if(strlen(arr[i]) > 0)
            lines++;

        i++;
    }

    return lines;
}

void showTabButtons(char btns[NLINES][NCOLS], WINDOW* win) {
    int x = 1, y = 1;
    int highlight = 0;

    int lines = countLines(btns, NLINES);

    for (int i = 0; i < lines; i++) {
		// Highlight the present choice 
		if(i == highlight) { 
            wattron(win, COLOR_PAIR(4) | A_BOLD); 
			mvwprintw(win, y, x, "%s", btns[i]);
            wattroff(win, COLOR_PAIR(4) | A_BOLD);
		}
		else {
            wattron(win, COLOR_PAIR(3)); 
			mvwprintw(win, y, x, "%s", btns[i]);
            wattroff(win, COLOR_PAIR(3));
        }
        
		y++;
    }

	wrefresh(win);
}

inline void boxTitle(WINDOW *wnd, int box_x, int box_y, int line_y, int line_x, int line_w) {
    box(wnd, box_y, box_x);
	mvwhline(wnd, line_y, line_x, ACS_HLINE, line_w);
}

inline void printTitle(WINDOW *win, int starty, int startx, int width, char string[], chtype color) {	
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

void displayFunc(struct Arg_struct params[2], int win_tab) {
    init();

	chdir(".");									// Устан. путь

    // Создание кнопок и hotkeys
	mvprintw(0, 1, "Left panel    Settings    Right panel");
    
    attron(A_REVERSE);
	mvprintw(LINES - 2, 1, "Tab");
    mvprintw(LINES - 2, 20, "F1");
    mvprintw(LINES - 2, 30, "F5");
    mvprintw(LINES - 2, 40, "F8");
    attroff(A_REVERSE);

	mvprintw(LINES - 2, 4, " - Switch panel");
    mvprintw(LINES - 2, 22, " - Quit");
    mvprintw(LINES - 2, 32, " - Copy");
    mvprintw(LINES - 2, 42, " - Delete");

    // Создание подокон 
    params[0].window = newwin(LINES-4, COLS/2, 1, 0);         // Левое окно
    params[1].window = newwin(LINES-4, COLS/2, 1, COLS/2);    // Правое окно

    tabs[3] = new_panel(params[0].window);
    tabs[4] = new_panel(params[1].window);


    for (int i = 0; i < 2; i++) {
        keypad(params[i].window, TRUE);
		params[i].highlight = 1;
        updateSubwindow(params, i);
	}
}

void redrawSubwindow(struct Arg_struct params[2], int win_tab) {
    wclear(params[win_tab].window);                 // Очищ. окно
    
    wattron(params[win_tab].window,  A_BOLD); 
    printTitle(params[win_tab].window, 1, 0, COLS/2, params[win_tab].path, COLOR_PAIR(2));
    wattroff(params[win_tab].window,  A_BOLD); 

    boxTitle(params[win_tab].window, 0, 0, 2, 1, (COLS/2) - 2);
    wrefresh(params[win_tab].window);
}

inline void updateSubwindow(struct Arg_struct params[2], int win_tab) {
    getcwd(params[win_tab].path, ARR_SIZE);         // Получ. путь
    LOG_CHAR(LOG_DEBAG, params[win_tab].path)
    scaner(params, win_tab);                        // Сканируем директорию
    redrawSubwindow(params, win_tab);               // Отрисовываем подокно
    printList(params, win_tab);	                    // Выводим на экран список файлов
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

