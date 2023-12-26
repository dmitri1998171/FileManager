#include "../../include/header.h"

#define CTRL(x) (x & 0x1F)

inline void hidePanel(Directory directory[2], Tab tabs[3], int win_tab) {
    panel_state = HIDE;

    for(int i = 0; i < 3; i++)
        hide_panel(tabs[i].panel);

    if(directory[win_tab].mode == VIEW_MODE) {
        viewModeFunc(directory, win_tab);
    }
    
    else if(directory[win_tab].mode == TREE_MODE) {
    
    }

    else {
        redrawSubwindow(directory, win_tab);
        printList(directory, win_tab);
    }

    update_panels();
    doupdate();
}

void enterFunc(Directory directory[2], Tab tabs[3], int win_tab) {
    if(panel_state == HIDE) {
        // Если директория
        if(directory[win_tab].entity[directory[win_tab].highlight - 1].type == DT_DIR) {
            chdir(directory[win_tab].entity[directory[win_tab].highlight - 1].name);
            getcwd(directory[win_tab].path, ARR_SIZE);         // Получ. путь
            updateSubwindow(directory, win_tab);
            directory[win_tab].highlight = 1;
        }

        // Если приложение
        if(directory[win_tab].entity[directory[win_tab].highlight - 1].type == DT_REG) {
            pid_t pid = fork();
            
            if(pid == 0) {
                endwin();
                execl(directory[win_tab].entity[directory[win_tab].highlight - 1].name, directory[win_tab].entity[directory[win_tab].highlight - 1].name, NULL);
                exit(0);
            }

            wait(&pid);
            displayFunc(directory, win_tab);
        }
    } else {
        if(panel_state == LEFT_PANEL || panel_state == RIGHT_PANEL) {
            win_tab = (panel_state == LEFT_PANEL) ? 0 : 1;

            switch (tabs[panel_state].highlight) {
            // Show modes
                case 0:
                    directory[win_tab].mode = VIEW_MODE;
                    break;
                case 1:
                    directory[win_tab].mode = TREE_MODE;
                    break;
                case 2:
                    directory[win_tab].mode = LIST_MODE;
                    break;
                    
            // Sorts
                case 3:
                    sort = ALPHABET;
                    sortList(directory, win_tab);
                    break;
                case 4:
                    sort = TYPE;
                    sortList(directory, win_tab);
                    break;
                case 5:
                    sort = SORT_SIZE_MIN;
                    sortList(directory, win_tab);
                    break;
                case 6:
                    sort = SORT_SIZE_MAX;
                    sortList(directory, win_tab);
                    break;
                case 7:
                    sort = MOD_TIME_MIN;
                    sortList(directory, win_tab);
                    break;
                case 8:
                    sort = MOD_TIME_MAX;
                    sortList(directory, win_tab);
                    break;
            }
        }

        hidePanel(directory, tabs, win_tab);
    }
}

void switchFunc(Directory directory[2], Tab tabs[3], int *cycle, int *win_tab) {
    char tmp[ARR_SIZE * 2];
    char path_r[ARR_SIZE];
    char path_w[ARR_SIZE];

    int input = wgetch(directory[*win_tab].window);

    switch(input) {
        case KEY_DOWN:
            if(panel_state == HIDE) {                
                if(directory[*win_tab].mode == VIEW_MODE) {
                    if(viewMode.y < viewMode.linesCounter) {
                        viewMode.y++;

                        if(viewMode.x > strlen(viewMode.lines[viewMode.y - Y_OFFSET]))
                            viewMode.x = strlen(viewMode.lines[viewMode.y - Y_OFFSET]) + 2;
                        drawText(directory, *win_tab);
                    }
                }
                
                else if(directory[*win_tab].mode == TREE_MODE) {
                
                }

                else {
                    if(directory[*win_tab].highlight == directory[*win_tab].counter.total)
                        directory[*win_tab].highlight = 1;
                    else 
                        ++directory[*win_tab].highlight;  

                    int inactiveTab = *win_tab ? 0 : 1;

                    if(directory[inactiveTab].mode == VIEW_MODE) {
                        viewModeFunc(directory, inactiveTab);
                    }
                    
                    if(directory[inactiveTab].mode == TREE_MODE) {
                    
                    } 

                    printList(directory, *win_tab);
                }
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
                if(directory[*win_tab].mode == VIEW_MODE) {
                    if(viewMode.y > Y_OFFSET) {
                        viewMode.y--;

                        if(viewMode.x > strlen(viewMode.lines[viewMode.y - Y_OFFSET]))
                            viewMode.x = strlen(viewMode.lines[viewMode.y - Y_OFFSET]) + 2;
                        drawText(directory, *win_tab);
                    }
                }
                
                else if(directory[*win_tab].mode == TREE_MODE) {
                
                }

                else {
                    if(directory[*win_tab].highlight == 1)
                        directory[*win_tab].highlight = directory[*win_tab].counter.total;
                    else
                        --directory[*win_tab].highlight;

                    int inactiveTab = *win_tab ? 0 : 1;

                    if(directory[inactiveTab].mode == VIEW_MODE) {
                        viewModeFunc(directory, inactiveTab);
                    }

                    if(directory[inactiveTab].mode == TREE_MODE) {
                    
                    }

                    printList(directory, *win_tab);
                }
            } else {
                if(tabs[panel_state].highlight == 0)
                    tabs[panel_state].highlight = tabs[panel_state].linesCounter - 1;
                else 
                    --tabs[panel_state].highlight;

                showTabButtons(tabs);
            }

            break;

        case KEY_LEFT:
            if(panel_state == HIDE) {
                if(directory[*win_tab].mode == VIEW_MODE) {
                    if(viewMode.x > X_OFFSET)
                        viewMode.x--;
                        drawText(directory, *win_tab);
                }
            }

            break;

        case KEY_RIGHT:
            if(panel_state == HIDE) {
                if(directory[*win_tab].mode == VIEW_MODE) {
                    if(viewMode.x <= strlen(viewMode.lines[viewMode.y - Y_OFFSET]) + 1) 
                        viewMode.x++;
                        drawText(directory, *win_tab);
                }
            }

            break;

        case KEY_F(1):
            *cycle = 0;
            break;

        case KEY_F(5):
            pthreadStruct.filename = directory[*win_tab].entity[directory[*win_tab].highlight - 1].name;
            pthreadStruct.pBarName = "Copy: ";

            pthread_create(&tid2, NULL, progressBar, &pthreadStruct);

            snprintf(path_r, sizeof(path_r), "%s/%s", directory[*win_tab].path, directory[*win_tab].entity[directory[*win_tab].highlight - 1].name);
            renameFunc(directory[*win_tab].entity[directory[*win_tab].highlight - 1].name, path_w);
            snprintf(tmp, sizeof(path_r) + sizeof(directory[*win_tab].path), "cp %s %s/%s", path_r, directory[*win_tab].path, path_w);
            
            system(tmp);
            pthread_join(tid2, NULL);

            updateSubwindow(directory, 0);
            updateSubwindow(directory, 1);
            
            break;

        case KEY_F(8):
            snprintf(tmp, ARR_SIZE + 7, "rm -rf %s", directory[*win_tab].entity[directory[*win_tab].highlight - 1].name);

            system(tmp);
            updateSubwindow(directory, 0);
            updateSubwindow(directory, 1);
            break;

        case 127:                                   // Backspace
            if(directory[*win_tab].mode == VIEW_MODE) {
                if(viewMode.x == X_OFFSET && viewMode.y > Y_OFFSET) {
                    viewMode.x = strlen(viewMode.lines[viewMode.y - Y_OFFSET]);
                    deletechar(viewMode.lines[viewMode.y - Y_OFFSET - 1], viewMode.x);
                    mvwprintw(directory[*win_tab].window, viewMode.y, X_OFFSET, viewMode.lines[viewMode.y - Y_OFFSET]);
                    mvwprintw(directory[*win_tab].window, viewMode.y - 1, X_OFFSET, viewMode.lines[viewMode.y - 1 - Y_OFFSET]);
                    viewMode.y--;
                } 
                else if(viewMode.x > X_OFFSET && viewMode.y >= Y_OFFSET) {
                    deletechar(viewMode.lines[viewMode.y - Y_OFFSET], viewMode.x - X_OFFSET - 1);
                    mvwprintw(directory[*win_tab].window, viewMode.y, X_OFFSET, viewMode.lines[viewMode.y - Y_OFFSET]);
                    viewMode.x--;
                }

                mvwaddch(directory[*win_tab].window, viewMode.y, viewMode.x, ' ' | COLOR_PAIR(2) | A_BLINK);
            }
            break;

        case 9:                                     // Tab button
            *win_tab += 1;

            if(*win_tab > 1) 
                *win_tab = 0;

            break;

        case 10:                                    // Enter button
            enterFunc(directory, tabs, *win_tab);
            break;

        case 27:                                    // Escape button
            if(panel_state == HIDE) {
                for (int i = 0; i < directory[*win_tab].counter.total; i++) {
                    if( ! strcmp(directory[*win_tab].entity[i].name, ".."))
                        directory[*win_tab].highlight = i + 1;
                }

                enterFunc(directory, tabs, *win_tab);
            } else {
                hidePanel(directory, tabs, *win_tab);
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

        default:                                    // Symbol's insertion
            if(directory[*win_tab].mode == VIEW_MODE) {
                char symbol = input;
                append(viewMode.lines[viewMode.y - Y_OFFSET], symbol, viewMode.x - X_OFFSET);
                mvwprintw(directory[*win_tab].window, viewMode.y, X_OFFSET, viewMode.lines[viewMode.y - Y_OFFSET]);
                viewMode.x++;
            }

            break;
    }

    // Shortcuts
    if(input == CTRL('u')) 
        directory[*win_tab].highlight = 1;
    
    if(input == CTRL('d')) 
        directory[*win_tab].highlight = directory[*win_tab].counter.total;

}

inline void showTab(Directory directory[2], Tab tabs[3], int *win_tab) {
    for(int i = 0; i < 3; i++)
        hide_panel(tabs[i].panel);

    show_panel(tabs[panel_state].panel);
    showTabButtons(tabs);
    update_panels();
    doupdate();
}