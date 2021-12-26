#include "../include/header.h"

int main() {	
        int c, cycle = 1, win_tab = 0;
        highlight[0] = 1, highlight[1] = 1;

        pthread_mutex_init(&mutex, NULL);
        displayFunc();

        while(cycle) {
                /* переключение между окнами */
                if(win_tab == 0) {
                        c = wgetch(windows[0]); 
                        switchFunc(windows[0], path[0], dir_arr[0], &dir_size[0], choices[1], &c, &highlight[0], &size[0], &cycle, &win_tab);
                        print_menu(windows[0], highlight[0], choices[1], &size[0]);
                }

                if(win_tab == 1) {
                        c = wgetch(windows[1]);
                        switchFunc(windows[1], path[1], dir_arr[1], &dir_size[1], choices[0], &c, &highlight[1], &size[1], &cycle, &win_tab);
                        print_menu(windows[1], highlight[1], choices[0], &size[1]);   
                }
        }	
        pthread_mutex_destroy(&mutex);
        endwin();
        return 0;
}
