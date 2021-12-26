#include "../include/header.h"

void *progressBar(void *param){
    int h=6, w=50;
    WINDOW *mycopywin;
    WINDOW *mysubwin;

    struct pthread_struct *threadStruct = (struct pthread_struct *) param;  // ?

    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    
    mycopywin = newwin(h, w, (LINES/2)-(h/2), (COLS/2)-(w/2));
    box(mycopywin, 0, 0);

    wattron(mycopywin, COLOR_PAIR(1));
    mvwprintw(mycopywin, h-5, 2, threadStruct->pBarName);
    wattroff(mycopywin, COLOR_PAIR(1));
    mvwprintw(mycopywin, h-5, 8, threadStruct->filename);

    mysubwin = derwin(mycopywin, 3, w-2, 2, 1);
    box(mysubwin, 0,0);

    wattroff(mycopywin, COLOR_PAIR(1));
    for(int n = 0; n < w; n++){
        mvwaddch(mycopywin, h-3, n+2, '#');
        mvwprintw(mycopywin, h-5, strlen(threadStruct->filename)+10, "%d%%", (n*2)+6);
        wrefresh(mycopywin);
        usleep(100000);
    }

    delwin(mysubwin);
    delwin(mycopywin);
    
    return NULL;
}
