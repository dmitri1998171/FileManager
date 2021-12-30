#include "../include/header.h"

int main() {	
	#if DEBUG
		fdw = fopen("log.log", "w");
	#endif

	int cycle = 1, win_tab = 0;
	struct Arg_struct params[2];

	pthread_mutex_init(&mutex, NULL);
	chdir(".");									// Устан. путь
	displayFunc(params);						// Инициализируем интерфейс приложения
	
	LOG_CHAR(params[0].path, 0);
	
	while(cycle) {
		/* переключение между окнами */
		if(!win_tab) {
			switchFunc(&params[0], &cycle, &win_tab);
			printList(&params[0]);
		}

		if(win_tab) {
			switchFunc(&params[1], &cycle, &win_tab);
			printList(&params[1]);
		}

		// switchFunc(&params[win_tab], &cycle, &win_tab);
		// printMenu(windows[win_tab], &params[win_tab]);
	}

	pthread_mutex_destroy(&mutex);
	endwin();

	#if DEBUG
		fclose(fdw);
	#endif

	return 0;
}
