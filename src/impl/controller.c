#include "../../include/header.h"

#define CTRL(x) (x & 0x1F)

void enterFunc(struct Directory_struct directory[2], int win_tab) {   
    // Если директория
    if(directory[win_tab].entity[directory[win_tab].highlight - 1].type == DT_DIR) {
        chdir(directory[win_tab].entity[directory[win_tab].highlight - 1].name);
        updateSubwindow(directory, win_tab);

        directory[win_tab].highlight = 1;
    }

    // Если приложение
    // if(directory->entity[directory->highlight - 1].type == DT_REG) {
    //     pid_t pid = fork();
        
    //     if(pid == 0) {
    //         endwin();
    //         execl(directory->entity[directory->highlight - 1].name, directory->entity[directory->highlight - 1].name, NULL);
    //         exit(0);
    //     }

    //     wait(&pid);
    //     displayFunc(directory, win_tab);
    // }
    
    // refresh();
    // updateSubwindow(directory, win_tab);
}

void switchFunc(struct Directory_struct directory[2], struct Tab_struct tabs[3], int *cycle, int *win_tab) {
    char tmp[ARR_SIZE * 2];
    // char path_r[ARR_SIZE];
    // char path_w[ARR_SIZE];

    int input = wgetch(directory[*win_tab].window);

    switch(input) {
        case KEY_DOWN:
            if(panel_state == HIDE) {
                if(directory[*win_tab].highlight == directory[*win_tab].size)
                    directory[*win_tab].highlight = 1;
                else 
                    ++directory[*win_tab].highlight;  
                
                printList(directory, *win_tab);
            } else {
                if(tabs[panel_state].highlight == tabs[panel_state].linesCounter - 1)
                    tabs[panel_state].highlight = 0;
                else 
                    ++tabs[panel_state].highlight;

                showTabButtons(tabs);
            }

            break;

        case KEY_UP:
            if(panel_state == HIDE) {
                if(directory[*win_tab].highlight == 1)
                    directory[*win_tab].highlight = directory[*win_tab].size;
                else
                    --directory[*win_tab].highlight;

                printList(directory, *win_tab);
            } else {
                if(tabs[panel_state].highlight == 0)
                    tabs[panel_state].highlight = tabs[panel_state].linesCounter - 1;
                else 
                    --tabs[panel_state].highlight;

                showTabButtons(tabs);
            }

            break;

        case KEY_F(1):
            *cycle = 0;
            break;

        case KEY_F(5):
            // pthreadStruct.filename = directory[*win_tab].choices[directory[*win_tab].highlight - 1];
            // pthreadStruct.pBarName = "Copy: "; 

            // pthread_create(&tid2, NULL, progressBar, &pthreadStruct);

            // snprintf(path_r, sizeof(path_r), "%s/%s", directory[*win_tab].path, directory[*win_tab].choices[directory[*win_tab].highlight - 1]);
            // renameFunc(directory[*win_tab].choices[directory[*win_tab].highlight - 1], path_w);
            // snprintf(tmp, sizeof(path_r) + sizeof(directory[*win_tab].path), "cp %s %s/%s", path_r, directory[*win_tab].path, path_w);
            
            // system(tmp);

            pthread_join(tid2, NULL);

            /*
                Передается только один элемент массива структур.
                Чтобы обновить второй нужно его достать - где ???
            */ 

            LOG_NUM(LOG_DEBUG, *win_tab)
            updateSubwindow(directory, *win_tab);
            updateSubwindow(directory, *win_tab);
            break;

        case KEY_F(8):
            // snprintf(tmp, ARR_SIZE + 7, "rm -rf %s", directory[*win_tab].choices[directory->highlight - 1]);

            system(tmp);

            updateSubwindow(directory, *win_tab);
            break;

        case 9:                                     // Tab button
            *win_tab += 1;

            if(*win_tab > 1) 
                *win_tab = 0;

            break;

        case 10:                                    // Enter button
            enterFunc(directory, *win_tab);
            break;

        case 27:                                    // Escape button
            if(panel_state == HIDE) {
                directory[*win_tab].highlight = 1;
                enterFunc(directory, *win_tab);
            } else {
                panel_state = HIDE;
                
                for(int i = 0; i < 3; i++)
                    hide_panel(tabs[i].panel);

                redrawSubwindow(directory, *win_tab);
    			printList(directory, *win_tab);

                update_panels();
                doupdate();
            }

            break;

        case '1':                                   // Left panel tab
            panel_state = LEFT_PANEL;
            showTab(directory, tabs, win_tab);
            break;

        case '2':                                   // Settings tab
            panel_state = SETTINGS;
            showTab(directory, tabs, win_tab);
            break;

        case '3':                                   // Right panel tab
            panel_state = RIGHT_PANEL;
            showTab(directory, tabs, win_tab);
            break;
    }

    // Shortcuts
    if(input == CTRL('u')) 
        directory[*win_tab].highlight = 1;
    
    if(input == CTRL('d')) 
        directory[*win_tab].highlight = directory[*win_tab].size;

}

inline void showTab(struct Directory_struct directory[2], struct Tab_struct tabs[3], int *win_tab) {
    for(int i = 0; i < 3; i++)
        hide_panel(tabs[i].panel);

    redrawSubwindow(directory, *win_tab);
    show_panel(tabs[panel_state].panel);
    showTabButtons(tabs);
    update_panels();
    doupdate();
}