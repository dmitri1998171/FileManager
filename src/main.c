#include "../include/header.h"

void init_tabs(struct Arg_struct params[2], struct Tab_struct tabs[3], int win_tab) {
	strncpy(tabs[LEFT_PANEL].panel_btns[0], "View mode", NLINES);
	strncpy(tabs[LEFT_PANEL].panel_btns[1], "Tree mode", NLINES);
	strncpy(tabs[LEFT_PANEL].panel_btns[2], "List mode", NLINES);

	strcpy(tabs[SETTINGS].panel_btns[0], "option_1");
	strcpy(tabs[SETTINGS].panel_btns[1], "option_2");
	strcpy(tabs[SETTINGS].panel_btns[2], "option_3");

	strcpy(tabs[RIGHT_PANEL].panel_btns[0], tabs[0].panel_btns[0]);
	strcpy(tabs[RIGHT_PANEL].panel_btns[1], tabs[0].panel_btns[1]);
	strcpy(tabs[RIGHT_PANEL].panel_btns[2], tabs[0].panel_btns[2]);

	for (int i = 0; i < 3; i++) {
		tabs[i].win = newwin(NLINES, NCOLS, 2, 1 + (i * TAB_OFFSET));
		wbkgd(tabs[i].win, COLOR_PAIR(3));
		box(tabs[i].win, 0, 0);

		tabs[i].panel = new_panel(tabs[i].win);
		hide_panel(tabs[i].panel);

		tabs[i].highlight = 0;
		tabs[i].linesCounter = countLines(tabs[i].panel_btns, NLINES);
	}

	redrawSubwindow(params, win_tab);
}

int main() {	
	#if DEBUG
		fdw = fopen("log.log", "w");
	#endif

	ESCDELAY = 0;								// Убирает задержку при нажатии Escape
	panel_state = HIDE;
	int cycle = 1, win_tab = 0;
	struct Arg_struct params[2];				// 0 - Левое окно; 1 - правое окно
	struct Tab_struct tabs[3];

	pthread_mutex_init(&mutex, NULL);
	displayFunc(params, win_tab);				// Инициализируем интерфейс приложения
	init_tabs(params, tabs, win_tab);			// Создаем вкладки
	printList(params, win_tab);

	while(cycle) {
		switchFunc(params, tabs, &cycle, &win_tab);
	}

	pthread_mutex_destroy(&mutex);
	endwin();

	#if DEBUG
		fclose(fdw);
	#endif

	return 0;
}
