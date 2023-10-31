#include "../../include/header.h"

void enterFunc(struct Arg_struct *params, int win_tab) {   
    int check = 0;
    
    for(int i = 0; i < params->dir_size; i++) {
        // Переходим в директорию
        if(!strcmp(params->choices[params->highlight - 1], params->dir_arr[i])) {
            chdir(params->choices[params->highlight - 1]);
            updateSubwindow(params, win_tab);

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
        displayFunc(params, win_tab);
    }
    
    refresh();
}

void switchFunc(struct Arg_struct params[2], int *cycle, int *win_tab) {
    char tmp[ARR_SIZE * 2];
    char path_r[ARR_SIZE];
    char path_w[ARR_SIZE];
    int input = wgetch(params[*win_tab].window);

    switch(input) {
        case KEY_DOWN:
            if(params[*win_tab].highlight == params[*win_tab].size)
                params[*win_tab].highlight = 1;
            else 
                ++params[*win_tab].highlight;  
            
            break;

        case KEY_UP:
            if(params[*win_tab].highlight == 1) {
                params[*win_tab].highlight = params[*win_tab].size;
            }
            else
                --params[*win_tab].highlight;

            break;

        case KEY_F(1):
            *cycle = 0;
            break;

        case KEY_F(5):
            pthreadStruct.filename = params[*win_tab].choices[params[*win_tab].highlight - 1];
            pthreadStruct.pBarName = "Copy: "; 

            pthread_create(&tid2, NULL, progressBar, &pthreadStruct);

            snprintf(path_r, sizeof(path_r), "%s/%s", params[*win_tab].path, params[*win_tab].choices[params[*win_tab].highlight - 1]);
            renameFunc(params[*win_tab].choices[params[*win_tab].highlight - 1], path_w);
            snprintf(tmp, sizeof(path_r) + sizeof(params[*win_tab].path), "cp %s %s/%s", path_r, params[*win_tab].path, path_w);
            
            system(tmp);

            pthread_join(tid2, NULL);

            /*
                Передается только один элемент массива структур.
                Чтобы обновить второй нужно его достать - где ???
            */ 

            LOG_NUM(LOG_DEBAG, *win_tab)
            updateSubwindow(params, *win_tab);
            updateSubwindow(params, *win_tab);
            break;

        case KEY_F(8):
            snprintf(tmp, ARR_SIZE + 7, "rm -rf %s", params[*win_tab].choices[params->highlight - 1]);

            system(tmp);

            updateSubwindow(params, *win_tab);
            break;

        case '\t':
            *win_tab += 1;

            if(*win_tab > 1) 
                *win_tab = 0;

            break;

        case 10:
            enterFunc(params, *win_tab);
            break;
    }
}

