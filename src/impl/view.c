#include "../../include/header.h"

#define BORDER 2

// 0 ... 15
inline int colornum(int fg, int bg) {
    int B, bbb, ffff;

    B = 1 << 7;
    bbb = (7 & bg) << 4;
    ffff = 7 & fg;

    return (B | bbb | ffff);
}

inline short curs_color(int fg) {
    switch (7 & fg) {           /* RGB */
        case 0:                     /* 000 */
            return (COLOR_BLACK);
        case 1:                     /* 001 */
            return (COLOR_BLUE);
        case 2:                     /* 010 */
            return (COLOR_GREEN);
        case 3:                     /* 011 */
            return (COLOR_CYAN);
        case 4:                     /* 100 */
            return (COLOR_RED);
        case 5:                     /* 101 */
            return (COLOR_MAGENTA);
        case 6:                     /* 110 */
            return (COLOR_YELLOW);
        case 7:                     /* 111 */
            return (COLOR_WHITE);
    }

    return 0;
}

inline void init_colorpairs() {
    int fg, bg;
    int colorpair;

    for (bg = 0; bg <= 7; bg++) {
        for (fg = 0; fg <= 7; fg++) {
            colorpair = colornum(fg, bg);
            init_pair(colorpair, curs_color(fg), curs_color(bg));
        }
    }
}

inline void init() {
    initscr();
    curs_set(0);
    start_color();
	noecho();
	cbreak();
    
    init_colorpairs();

    init_color(COLOR_BLACK, 140, 140, 140); 
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_YELLOW, COLOR_RED);
}

void readDir(WINDOW* wnd, char* path) {
    int x = 3, y = 4;
    char buffer[STR_SIZE + 1];
    FILE* fd = fopen(path, "r");

    if(fd < 0) {
        perror("ERROR! Can't open the file!");
        exit(1);
    }

    while( fgets(buffer, STR_SIZE, fd) != NULL ) {
        int size = strlen(buffer);
        buffer[size] = '\0';
        mvwprintw(wnd, y, x, "%s", buffer);
        y++;
    }

    fclose(fd);

    box(wnd, 0, 0);
	wrefresh(wnd);
}

void viewMode(Directory directory[2], int win_tab) {
    char *str;
    redrawSubwindow(directory, win_tab);

    int inactiveTab = win_tab ? 0 : 1;
    if(directory[inactiveTab].entity[ directory[inactiveTab].highlight - 1 ].type == DT_REG) {
        int size = strlen(directory[inactiveTab].path) + strlen(directory[inactiveTab].entity[ directory[inactiveTab].highlight - 1 ].name) + 2;
        str = malloc(size);
        snprintf(str, size, "%s/%s", directory[inactiveTab].path, directory[inactiveTab].entity[ directory[inactiveTab].highlight - 1 ].name);
        readDir(directory[win_tab].window, str);
    }
}

void printList(Directory directory[2], int win_tab) {
	int x = 2, y = 4;
    char str[MOD_TIME_SIZE];

    for (int i = 0; i < directory[win_tab].counter.total; i++) {
		// Highlight the present choice 
		if(directory[win_tab].highlight == i + 1) { 
            wattron(directory[win_tab].window, COLOR_PAIR(2) | A_BOLD); 
			mvwprintw(directory[win_tab].window, y, x, "%s", directory[win_tab].entity[i].name);
            mvwprintw(directory[win_tab].window, y, (COLS/2) - 18, "%d", directory[win_tab].entity[i].size);

            strftime(str, MOD_TIME_SIZE, "%b %d %H:%M", localtime(&(directory[win_tab].entity[i].modify_time)));
            mvwprintw(directory[win_tab].window, y, (COLS/2) - 14, "%s", str);
            wattroff(directory[win_tab].window, COLOR_PAIR(2) | A_BOLD);
		}
		else {
            if(directory[win_tab].entity[i].type == DT_DIR) {
                wattron(directory[win_tab].window, COLOR_PAIR(colornum(11, 8)) | A_BOLD); 
                mvwprintw(directory[win_tab].window, y, x, "%s", directory[win_tab].entity[i].name);
                mvwprintw(directory[win_tab].window, y, (COLS/2) - 18, "%d", directory[win_tab].entity[i].size);

        		strftime(str, MOD_TIME_SIZE, "%b %d %H:%M", localtime(&(directory[win_tab].entity[i].modify_time)));
                mvwprintw(directory[win_tab].window, y, (COLS/2) - 14, "%s", str);
                wattroff(directory[win_tab].window, COLOR_PAIR(colornum(11, 8)) | A_BOLD);
            } else {
			    mvwprintw(directory[win_tab].window, y, x, "%s", directory[win_tab].entity[i].name);
                mvwprintw(directory[win_tab].window, y, (COLS/2) - 18, "%d", directory[win_tab].entity[i].size);
                
                strftime(str, MOD_TIME_SIZE, "%b %d %H:%M", localtime(&(directory[win_tab].entity[i].modify_time)));
                mvwprintw(directory[win_tab].window, y, (COLS/2) - 14, "%s", str);
            }
        }

		y++;
    }

	wrefresh(directory[win_tab].window);
}

void showTabButtons(Tab tabs[3]) {
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

inline void boxTitle(WINDOW *wnd, int box_x, int box_y, int line_y, int line_x, int line_w, int mode) {
    box(wnd, box_y, box_x);
	mvwhline(wnd, line_y, line_x, ACS_HLINE, line_w);

    if(mode == LIST_MODE) {
        for (int i = line_y + 1; i < LINES - 5; i++) {
            mvwhline(wnd, i, (COLS/2) - 15, ACS_VLINE, 1);
            mvwhline(wnd, i, (COLS/2) - 22, ACS_VLINE, 1);
        }

        wattron(wnd, COLOR_PAIR(colornum(6, 8)));
        mvwprintw(wnd, line_y + 1, (((COLS/2) - 22) / 2) - 2, "Name");
        mvwprintw(wnd, line_y + 1, (COLS/2) - 20, "Size");
        mvwprintw(wnd, line_y + 1, (COLS/2) - 12, "Mod time");
        wattroff(wnd, COLOR_PAIR(colornum(6, 8)));
    }
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

void displayFunc(Directory directory[2], int win_tab) {
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

    for (int i = 0; i < 2; i++) {
        directory[i].panel = new_panel(directory[i].window);
        directory[i].mode = LIST_MODE;
        keypad(directory[i].window, TRUE);
        getcwd(directory[i].path, ARR_SIZE);         // Получ. путь
        updateSubwindow(directory, i);
	}
}

void redrawSubwindow(Directory directory[2], int win_tab) {
    wclear(directory[win_tab].window);                 // Очищ. окно
    
    wattron(directory[win_tab].window,  A_BOLD); 
    printTitle(directory[win_tab].window, 1, 0, COLS/2, directory[win_tab].path, COLOR_PAIR(2));
    wattroff(directory[win_tab].window,  A_BOLD); 

    boxTitle(directory[win_tab].window, 0, 0, 2, 1, (COLS/2) - 2, directory[win_tab].mode);
    wrefresh(directory[win_tab].window);
}

inline void updateSubwindow(Directory directory[2], int win_tab) {
    if(directory[win_tab].mode == VIEW_MODE) {
        viewMode(directory, win_tab);
    }
    
    else if(directory[win_tab].mode == TREE_MODE) {
    
    }

    else {
        scaner(directory, win_tab);                        // Сканируем директорию
        sortList(directory, win_tab);                      // Сортировка
        redrawSubwindow(directory, win_tab);               // Отрисовываем подокно
        printList(directory, win_tab);	                   // Выводим на экран список файлов
    }
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

void init_tabs(Directory directory[2], Tab tabs[3], int win_tab) {
    char leftPanelLines[NLINES][NCOLS] = {
        "View mode",
        "Tree mode",
        "List mode",
        "Sort by Alphabet",
        "Sort by Type",
        "Sort by Size min",
        "Sort by Size max",
        "Sort by Mod time min",
        "Sort by Mod time max"
    };

    char optionsPanelLines[OPTNLINES][NCOLS] = {
        "option_1",
        "option_2",
        "option_3"
    };

	for (int i = 0; i < 3; i++) {
		tabs[i].win = newwin(NLINES + 2, NCOLS, 2, 1 + (i * TAB_OFFSET));
		wbkgd(tabs[i].win, COLOR_PAIR(3));
		box(tabs[i].win, 0, 0);

		tabs[i].panel = new_panel(tabs[i].win);
		hide_panel(tabs[i].panel);

		tabs[i].highlight = 0;
		tabs[i].linesCounter = 0;

        if(i == 1) {
            tabs[i].panel_btns = (char**) malloc(sizeof(char*) * OPTNLINES);

            for (int j = 0; j < OPTNLINES; j++) {
                tabs[i].panel_btns[j] = (char*) malloc(NCOLS);
                strcpy(tabs[i].panel_btns[j], optionsPanelLines[j]);
                tabs[i].linesCounter++;
            }
        } else {
            tabs[i].panel_btns = (char**) malloc(sizeof(char*) * NLINES);

            for (int j = 0; j < NLINES; j++) {
                tabs[i].panel_btns[j] = (char*) malloc(NCOLS);
                strcpy(tabs[i].panel_btns[j], leftPanelLines[j]);
                tabs[i].linesCounter++;
            }
        }
	}

	redrawSubwindow(directory, win_tab);
}
