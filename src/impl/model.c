#include "../../include/header.h"

void scaner(struct Arg_struct params[2], int win_tab) {
	int counter = 0, dir_counter = 0;
	struct dirent *dir_struct_t;
    DIR *dir;

	dir = opendir(params[win_tab].path);
	if(dir == NULL) { 
        perror("opendir error!");
        exit(1); 
    }

	while((dir_struct_t = readdir(dir))) {
		if(strcmp(dir_struct_t->d_name, ".") == 0) {
			continue;
	    }

        params[win_tab].choices[counter] = dir_struct_t->d_name;

		if(dir_struct_t->d_type == DT_DIR) {
			params[win_tab].dir_arr[dir_counter] = dir_struct_t->d_name;
			dir_counter++;
		}

        counter++;
	}

    params[win_tab].size = counter;
    params[win_tab].dir_size = dir_counter;
	closedir(dir);
}

int countLines(char arr[][NCOLS], int totalLines) {
    int i = 0, lines = 0;
    
    while(i < totalLines) {
        if(strlen(arr[i]) > 0)
            lines++;

        i++;
    }

    return lines;
}