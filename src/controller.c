#include "../include/header.h"

void enterFunc(struct Arg_struct *params) {   
    int check = 0;
    for(int i=0; i < params->dir_size; i++) {
        if(strcmp(params->choices[params->highlight-1], params->dir_arr[i]) == 0) {
            chdir(params->choices[params->highlight-1]);
            
            // reloadWinFunc(win, choices, path, dir_arr, highlight, size, dir_size);

            params->highlight = 1;
            check +=1;   
        }
    }
    if(check == 0) {
        pid_t pid = fork();
        if(pid == 0) {
            endwin();
            execl(params->choices[params->highlight-1], params->choices[params->highlight-1], NULL);
            exit(0);
        }
        wait(&pid);
        displayFunc(params);
    }
    refresh();
}

void switchFunc(struct Arg_struct *params, int *cycle, int *win_tab) {
    int input = wgetch(params->window);

    switch(input) {
        case KEY_DOWN:
            if(params->highlight == params->size)
                params->highlight = 1;
            else 
                ++params->highlight;  
            
            break;

        case KEY_UP:
            if(params->highlight == 1) {
                params->highlight = params->size;
            }
            else
                --params->highlight;
            break;

        case KEY_F(1):
            *cycle = 0;
            break;

        case KEY_F(5):
            strcpy(pthreadStruct.filename, params->choices[params->highlight-1]); 
            strcpy(pthreadStruct.pBarName, "Copy: "); 

            // pthread_create(&tid1, NULL, copyFunc, params->choices[params->highlight-1]);
            // pthread_create(&tid2, NULL, progressBar, &pthreadStruct);

            // pthread_join(tid1, NULL);
            // pthread_join(tid2, NULL);

            // reloadWinFunc(windows[0], choices, path, dir_arr, highlight, size, dir_size);
            // reloadWinFunc(windows[1], choices, path, dir_arr, highlight, size, dir_size);

            break;

        case '\t':
            *win_tab += 1;
            if(*win_tab > 1) 
                *win_tab = 0;
            
            break;

        case 10:
            enterFunc(params);
            break;
    }
}

