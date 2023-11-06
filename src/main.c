#include "../include/header.h"

void init_tabs(struct Directory_struct directory[2], struct Tab_struct tabs[3], int win_tab) {
	strncpy(tabs[LEFT_PANEL].panel_btns[0], "View mode", NLINES);
	strncpy(tabs[LEFT_PANEL].panel_btns[1], "Tree mode", NLINES);
	strncpy(tabs[LEFT_PANEL].panel_btns[2], "List mode", NLINES);

	strcpy(tabs[SETTINGS].panel_btns[0], "option_1");
	strcpy(tabs[SETTINGS].panel_btns[1], "option_2");
	strcpy(tabs[SETTINGS].panel_btns[2], "option_3");

	strcpy(tabs[RIGHT_PANEL].panel_btns[0], tabs[LEFT_PANEL].panel_btns[0]);
	strcpy(tabs[RIGHT_PANEL].panel_btns[1], tabs[LEFT_PANEL].panel_btns[1]);
	strcpy(tabs[RIGHT_PANEL].panel_btns[2], tabs[LEFT_PANEL].panel_btns[2]);

	for (int i = 0; i < 3; i++) {
		tabs[i].win = newwin(NLINES, NCOLS, 2, 1 + (i * TAB_OFFSET));
		wbkgd(tabs[i].win, COLOR_PAIR(3));
		box(tabs[i].win, 0, 0);

		tabs[i].panel = new_panel(tabs[i].win);
		hide_panel(tabs[i].panel);

		tabs[i].highlight = 0;
		tabs[i].linesCounter = countLines(tabs[i].panel_btns, NLINES);
	}

	redrawSubwindow(directory, win_tab);
}

int main() {	
	#if DEBUG
		fdw = fopen("log.log", "w");
	#endif

	int cycle = 1;
	int win_tab = 0;
	struct Directory_struct directory[2];			// 0 - Левое окно; 1 - правое окно
	struct Tab_struct tabs[3];

	ESCDELAY = 0;									// Убирает задержку при нажатии Escape
	panel_state = HIDE;
	
	pthread_mutex_init(&mutex, NULL);
	displayFunc(directory, win_tab);				// Инициализируем интерфейс приложения
	init_tabs(directory, tabs, win_tab);			// Создаем вкладки
	printList(directory, win_tab);					// Выводим список файлов в папке

	while(cycle) {
		switchFunc(directory, tabs, &cycle, &win_tab);
	}

	pthread_mutex_destroy(&mutex);
	endwin();

	#if DEBUG
		fclose(fdw);
	#endif

	return 0;
}
