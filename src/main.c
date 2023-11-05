#include "../include/header.h"

void init_tabs(struct Arg_struct params[2], int win_tab) {
	for (int i = 0; i < 3; i++) {
		wins[i] = newwin(NLINES, NCOLS, 2, 1 + (i * TAB_OFFSET));
		wbkgd(wins[i], COLOR_PAIR(3));
		box(wins[i], 0, 0);
		tabs[i] = new_panel(wins[i]);
		hide_panel(tabs[i]);
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
	tab_btn_highlight = 0;
	struct Arg_struct params[2];				// 0 - Левое окно; 1 - правое окно

	pthread_mutex_init(&mutex, NULL);
	displayFunc(params, win_tab);				// Инициализируем интерфейс приложения
	init_tabs(params, win_tab);					// Создаем вкладки
	printList(params, win_tab);

	while(cycle) {
		switchFunc(params, &cycle, &win_tab);
	}

	pthread_mutex_destroy(&mutex);
	endwin();

	#if DEBUG
		fclose(fdw);
	#endif

	return 0;
}
