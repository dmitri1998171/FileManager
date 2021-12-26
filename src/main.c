#include "../include/header.h"

int main(){	
	int c, cycle = 1, win_tab = 0;
    highlight_left = 1, highlight_right = 1;

    pthread_mutex_init(&mutex, NULL);
    displayFunc();
    
	while(cycle)
	{
        /* переключение между окнами */
        if(win_tab == 0){
            c = wgetch(windows[0]); 
            switchFunc(windows[0], path_left, dir_arr_left, 
                    &dir_size_left, choices_left, &c, &highlight_left, 
                    &size_left, &cycle, &win_tab);

            print_menu(windows[0], highlight_left, choices_left, 
                    &size_left);
        }

        if(win_tab == 1){
            c = wgetch(windows[1]);
            switchFunc(windows[1], path_right, dir_arr_right, 
                    &dir_size_right, choices_right, &c, &highlight_right, 
                    &size_right, &cycle, &win_tab);
            
            print_menu(windows[1], highlight_right, choices_right, 
                    &size_right);   
        }
	}	
    pthread_mutex_destroy(&mutex);
	endwin();
	return 0;
}
