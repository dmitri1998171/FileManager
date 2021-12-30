#include "../include/header.h"

int main() {	
	int cycle = 1, win_tab = 0;
	struct Arg_struct params[2];

	params[0].highlight = 1;
	params[1].highlight = 1;

	chdir(".");
	getcwd(params[0].path, ARR_SIZE);
	getcwd(params[1].path, ARR_SIZE);

	pthread_mutex_init(&mutex, NULL);
	displayFunc(params);

	scaner(windows[0], &params[0]);
	scaner(windows[1], &params[1]);

//////////////////////////////////////////////////////////////////////


	while(cycle) {
		/* переключение между окнами */
		if(!win_tab) {
				// c = wgetch(windows[0]); 
				switchFunc(&params[0], &cycle, &win_tab);
				// printMenu(windows[0], &params[0]);
				scaner(windows[0], &params[0]);
		}

		if(win_tab) {
				// switchFunc(windows[1], path[1], dir_arr[1], &dir_size[1], choices[0], &c, &highlight[1], &size[1], &cycle, &win_tab);
				switchFunc(&params[1], &cycle, &win_tab);
				// printMenu(windows[1], &params[1]);
		}

		// switchFunc(&params[win_tab], &cycle, &win_tab);
		// printMenu(windows[win_tab], &params[win_tab]);
	}	
	getch();
	pthread_mutex_destroy(&mutex);
	endwin();
	return 0;
}
