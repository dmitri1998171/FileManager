#include "../../include/header.h"

void scaner(struct Directory_struct directory[2], int win_tab) {
	int counter = 0, dir_counter = 0;
	struct dirent *dir_struct_t;
    DIR *dir;

	dir = opendir(directory[win_tab].path);
	if(dir == NULL) { 
        perror("opendir error!");
        exit(1); 
    }

	while((dir_struct_t = readdir(dir))) {
		struct stat attrib;
    	stat(dir_struct_t->d_name, &attrib);

		if(strcmp(dir_struct_t->d_name, ".") == 0) {
			continue;
	    }

		if(sizeof(directory[win_tab].entity) > 0)
			directory[win_tab].entity = realloc(directory[win_tab].entity, sizeof(directory[win_tab].entity) + sizeof(struct Entity_struct));
		
		directory[win_tab].entity[ directory[win_tab].size ].type = dir_struct_t->d_type;
		directory[win_tab].entity[ directory[win_tab].size ].size = dir_struct_t->d_reclen;
		strftime(directory[win_tab].entity[ directory[win_tab].size ].modify_time, MOD_TIME_SIZE, "%b %d %X", localtime(&(attrib.st_ctime)));
        strcpy(directory[win_tab].entity[ directory[win_tab].size ].name, dir_struct_t->d_name);
		
		directory[win_tab].size++;
	}

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