#include "../include/header.h"

void addElem(struct Tab_struct tabs[3], int panel, char* str) {
	if(tabs[panel].linesCounter == 0) {
		tabs[panel].panel_btns = (char**) malloc(sizeof(char*));

		for (int i = 0; i < sizeof(char**); i++)
			tabs[panel].panel_btns[0] = (char*) malloc(sizeof(char*));
		
		strncpy(tabs[panel].panel_btns[0], str, NLINES);
	} else {
		tabs[panel].panel_btns = (char**) realloc(tabs[panel].panel_btns, sizeof(char*) * (tabs[panel].linesCounter + 1));
		tabs[panel].panel_btns[tabs[panel].linesCounter] = (char*) malloc(sizeof(char*));

		strncpy(tabs[panel].panel_btns[tabs[panel].linesCounter], str, NLINES);
	}

	tabs[panel].linesCounter++;

}

void init_tabs(struct Directory_struct directory[2], struct Tab_struct tabs[3], int win_tab) {	
	for (int i = 0; i < 3; i++) {
		tabs[i].win = newwin(NLINES, NCOLS, 2, 1 + (i * TAB_OFFSET));
		wbkgd(tabs[i].win, COLOR_PAIR(3));
		box(tabs[i].win, 0, 0);

		tabs[i].panel = new_panel(tabs[i].win);
		hide_panel(tabs[i].panel);

		tabs[i].highlight = 0;
		tabs[i].linesCounter = 0;
	}

	addElem(tabs, LEFT_PANEL, "View mode");
	addElem(tabs, LEFT_PANEL, "Tree mode");
	addElem(tabs, LEFT_PANEL, "List mode");

	addElem(tabs, SETTINGS, "option_1");
	addElem(tabs, SETTINGS, "option_2");
	addElem(tabs, SETTINGS, "option_3");

	addElem(tabs, RIGHT_PANEL, tabs[LEFT_PANEL].panel_btns[0]);
	addElem(tabs, RIGHT_PANEL, tabs[LEFT_PANEL].panel_btns[1]);
	addElem(tabs, RIGHT_PANEL, tabs[LEFT_PANEL].panel_btns[2]);

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
