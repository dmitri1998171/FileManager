#include "../include/header.h"

int main() {	
	#if DEBUG
		fdw = fopen("log.log", "w");
	#endif

	int cycle = 1, win_tab = 0;
	struct Arg_struct params[2];				// 0 - Левое; 1 - правое окно

	pthread_mutex_init(&mutex, NULL);
	chdir(".");									// Устан. путь
	displayFunc(params);						// Инициализируем интерфейс приложения
	
	while(cycle) {
		/* переключение между окнами */
		if(!win_tab) {		// Левое окно
			switchFunc(&params[0], &cycle, &win_tab);
			printList(&params[0]);
		}

		if(win_tab) {		// Правое окно
			switchFunc(&params[1], &cycle, &win_tab);
			printList(&params[1]);
		}
	}

	pthread_mutex_destroy(&mutex);
	endwin();

	#if DEBUG
		fclose(fdw);
	#endif

	return 0;
}
