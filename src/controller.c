#include "../include/header.h"

void enterFunc(struct Arg_struct *params) {   
    int check = 0;
    
    for(int i = 0; i < params->dir_size; i++) {
        // Переходим в директорию
        if(!strcmp(params->choices[params->highlight-1], params->dir_arr[i])) {
            chdir(params->choices[params->highlight-1]);
            updateSubwindow(params);

            params->highlight = 1;
            check += 1;   
        }
    }

    // Запускаем приложение
    if(check == 0) {
        pid_t pid = fork();
        
        if(pid == 0) {
            endwin();
            execl(params->choices[params->highlight - 1], params->choices[params->highlight - 1], NULL);
            exit(0);
        }

        wait(&pid);
        displayFunc(params);
    }
    
    refresh();
}

void switchFunc(struct Arg_struct *params, int *cycle, int *win_tab) {
    int input = wgetch(params->window);
    char *tmp;

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
            pthreadStruct.filename = params->choices[params->highlight - 1];
            pthreadStruct.pBarName = "Copy: "; 

            pthread_create(&tid1, NULL, copyFunc, params->choices[params->highlight - 1]);
            pthread_create(&tid2, NULL, progressBar, &pthreadStruct);

            pthread_join(tid1, NULL);
            pthread_join(tid2, NULL);

        	LOG_NUM(LOG_WARNING, *win_tab)

            updateSubwindow(&params[0]);
            updateSubwindow(&params[1]);
            break;

        case KEY_F(8):
            snprintf(tmp, ARR_SIZE + 7, "rm -rf %s", params->choices[params->highlight - 1]);
            
            // system(tmp);
            execl("/bin/rm", "/bin/rm", "-rf", params->choices[params->highlight - 1]);

            updateSubwindow(&params[*win_tab]);
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

