#include "../include/header.h"



int main() {	
	#if DEBUG
		fdw = fopen("log.log", "w");
	#endif

	int cycle = 1, win_tab = 0;
	struct Arg_struct params[2];

	params[0].highlight = 1;
	params[1].highlight = 1;

	chdir(".");
	getcwd(params[0].path, ARR_SIZE);
	getcwd(params[1].path, ARR_SIZE);

	pthread_mutex_init(&mutex, NULL);
	displayFunc(params);

	scaner(&params[0]);
	// scaner(&params[1]);

	// printList(&params[0]);

// 
	int x = 2, y = 3;
	box(params[0].window, 0, 0);

	LOG_CHAR("\n", 0)

    for (int i = 0; i < params[0].size; i++) {
		// High light the present choice 
		if(params[0].highlight == i + 1) { 
            wattron(params[0].window, A_REVERSE); 
			mvwprintw(params[0].window, y, x, "%s", &params[0].choices[i]);
            wattroff(params[0].window, A_REVERSE);
		}
		else
			mvwprintw(params[0].window, y, x, "%s", &params[0].choices[i]);
        
		LOG_CHAR(&params[0].choices[i], 0)
		y++;
    }

	wrefresh(params[0].window);
// 
	#if DEBUG
		fclose(fdw);
	#endif

	while(cycle) {
		/* переключение между окнами */
		if(!win_tab) {
			switchFunc(&params[0], &cycle, &win_tab);
			// printMenu(windows[0], &params[0]);
			// printList(&params[0]);
		}

		if(win_tab) {
			switchFunc(&params[1], &cycle, &win_tab);
			// printMenu(windows[1], &params[1]);
		}

		// switchFunc(&params[win_tab], &cycle, &win_tab);
		// printMenu(windows[win_tab], &params[win_tab]);
	}

	pthread_mutex_destroy(&mutex);
	endwin();
	return 0;
}
