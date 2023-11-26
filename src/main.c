#include "../include/header.h"

int main() {	
	#if DEBUG
		fdw = fopen("log.log", "w");
	#endif

	int cycle = 1;
	int win_tab = 0;
	Directory directory[2];			// 0 - Левое окно; 1 - правое окно
	Tab tabs[3];

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
