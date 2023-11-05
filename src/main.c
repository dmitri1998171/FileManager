#include "../include/header.h"

#define NLINES 10
#define NCOLS 20
#define TAB_OFFSET 13

void init_tabs() {
	WINDOW *wins[3];

	for (int i = 0; i < 3; i++) {
		wins[i] = newwin(NLINES, NCOLS, 2, 1 + (i * TAB_OFFSET));
		box(wins[i], NLINES, NCOLS);
		tabs[i] = new_panel(wins[i]);
	}
}

int main() {	
	#if DEBUG
		fdw = fopen("log.log", "w");
	#endif

	ESCDELAY = 0;								// Убирает задержку при нажатии Escape
	panel_state = HIDE;

	int cycle = 1, win_tab = 0;
	struct Arg_struct params[2];				// 0 - Левое; 1 - правое окно

	pthread_mutex_init(&mutex, NULL);
	chdir(".");									// Устан. путь
	displayFunc(params, win_tab);				// Инициализируем интерфейс приложения
	
	init_tabs();

	while(cycle) {
		/* переключение между окнами */
		if(!win_tab) {		// Левое окно
			printList(params, win_tab);
			switchFunc(params, &cycle, &win_tab);
		}

		if(win_tab) {		// Правое окно
			printList(params, win_tab);
			switchFunc(params, &cycle, &win_tab);
		}
	}

	pthread_mutex_destroy(&mutex);
	endwin();

	#if DEBUG
		fclose(fdw);
	#endif

	return 0;
}
