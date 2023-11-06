#include "../../include/header.h"

#define BORDER 2

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

void printList(struct Directory_struct directory[2], int win_tab) {
	int x = 2, y = 3;

    for (int i = 0; i < directory[win_tab].size; i++) {
		// Highlight the present choice 
		if(directory[win_tab].highlight == i + 1) { 
            wattron(directory[win_tab].window, COLOR_PAIR(2) | A_BOLD); 
			mvwprintw(directory[win_tab].window, y, x, "%s", directory[win_tab].entity[i].name);
            wattroff(directory[win_tab].window, COLOR_PAIR(2) | A_BOLD);
		}
		else
			mvwprintw(directory[win_tab].window, y, x, "%s", directory[win_tab].entity[i].name);
        
		y++;
    }

	wrefresh(directory[win_tab].window);
}

void showTabButtons(struct Tab_struct tabs[3]) {
    int x = 1, y = 1;

    for (int i = 0; i < tabs[panel_state].linesCounter; i++) {
		// Highlight the present choice 
		if(i == tabs[panel_state].highlight) { 
            wattron(tabs[panel_state].win, COLOR_PAIR(4) | A_BOLD); 
			mvwprintw(tabs[panel_state].win, y, x, "%s", tabs[panel_state].panel_btns[i]);
            wattroff(tabs[panel_state].win, COLOR_PAIR(4) | A_BOLD);
		}
		else {
            wattron(tabs[panel_state].win, COLOR_PAIR(3)); 
			mvwprintw(tabs[panel_state].win, y, x, "%s", tabs[panel_state].panel_btns[i]);
            wattroff(tabs[panel_state].win, COLOR_PAIR(3));
        }
        
		y++;
    }

	wrefresh(tabs[panel_state].win);
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

void displayFunc(struct Directory_struct directory[2], int win_tab) {
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
    directory[0].window = newwin(LINES-4, COLS/2, 1, 0);         // Левое окно
    directory[1].window = newwin(LINES-4, COLS/2, 1, COLS/2);    // Правое окно

    directory[0].panel = new_panel(directory[0].window);
    directory[1].panel = new_panel(directory[1].window);

    for (int i = 0; i < 2; i++) {
        keypad(directory[i].window, TRUE);
	
        directory[i].size = 0;
    	directory[i].entity = (struct Entity_struct*) malloc(sizeof(struct Entity_struct));
    	directory[i].highlight = 1;

        updateSubwindow(directory, i);
	}
}

void redrawSubwindow(struct Directory_struct directory[2], int win_tab) {
    wclear(directory[win_tab].window);                 // Очищ. окно
    
    wattron(directory[win_tab].window,  A_BOLD); 
    printTitle(directory[win_tab].window, 1, 0, COLS/2, directory[win_tab].path, COLOR_PAIR(2));
    wattroff(directory[win_tab].window,  A_BOLD); 

    boxTitle(directory[win_tab].window, 0, 0, 2, 1, (COLS/2) - 2);
    wrefresh(directory[win_tab].window);
}

inline void updateSubwindow(struct Directory_struct directory[2], int win_tab) {
    getcwd(directory[win_tab].path, ARR_SIZE);         // Получ. путь
    scaner(directory, win_tab);                        // Сканируем директорию
    redrawSubwindow(directory, win_tab);               // Отрисовываем подокно
    printList(directory, win_tab);	                    // Выводим на экран список файлов
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

