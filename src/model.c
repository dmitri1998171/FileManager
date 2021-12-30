#include "../include/header.h"

void scaner(struct Arg_struct *params) {
	int counter = 0, dir_counter = 0;
	struct dirent *dir_struct_t;
    DIR *dir;

	dir = opendir(params->path);
	if(dir == NULL) { 
        perror("opendir");
        exit(1); 
    }

	while((dir_struct_t = readdir(dir))) {
		if(strcmp(dir_struct_t->d_name, ".") == 0) {
			continue;
	    }

        params->choices[counter] = dir_struct_t->d_name;

		if(dir_struct_t->d_type == DT_DIR) {
			params->dir_arr[dir_counter] = dir_struct_t->d_name;
			dir_counter++;
		}

        counter++;
	}

    params->size = counter;
    params->dir_size = dir_counter;
	closedir(dir);
}
