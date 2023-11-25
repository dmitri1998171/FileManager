#include "../../include/header.h"

// Структура для подсчета кол-ва эл-ов в директории
typedef struct EntitiesCounterStruct {
	int fileCounter;
	int dirCounter;
	int linkCounter;
	int total;
} EntCounter;

EntCounter entitiesCount(const char* path) {
	EntCounter entCounter;
	entCounter.dirCounter = 0;
	entCounter.fileCounter = 0;
	entCounter.linkCounter = 0;
	entCounter.total = 0;

	DIR * dirp;
	struct dirent * entry;

	dirp = opendir(path); 
	while ((entry = readdir(dirp)) != NULL) {
		entCounter.total++;

		if (entry->d_type == DT_REG) 
			entCounter.fileCounter++;

		if (entry->d_type == DT_LNK) 
			entCounter.linkCounter++;
	}

	entCounter.dirCounter = entCounter.total - entCounter.fileCounter - entCounter.linkCounter;
	closedir(dirp);

	return entCounter;
}

void scaner(struct Directory_struct directory[2], int win_tab) {
	struct dirent *dir_struct_t;
    DIR *dir;

	EntCounter entCounter = entitiesCount(directory[win_tab].path);
	
	dir = opendir(directory[win_tab].path);
	if(dir == NULL) { 
        perror("opendir error!");
        exit(1); 
    }

	if(directory[win_tab].size > 0) {
		free(directory[win_tab].entity);
	}

	directory[win_tab].size = 0;
	directory[win_tab].highlight = 1;
	directory[win_tab].entity = (struct Entity_struct*) malloc(sizeof(struct Entity_struct) * entCounter.total);

	while((dir_struct_t = readdir(dir))) {
		struct stat attrib;
    	stat(dir_struct_t->d_name, &attrib);

		if(strcmp(dir_struct_t->d_name, ".") == 0) {
			continue;
	    }

		directory[win_tab].entity[ directory[win_tab].size ].type = dir_struct_t->d_type;
		directory[win_tab].entity[ directory[win_tab].size ].size = dir_struct_t->d_reclen;
		strftime(directory[win_tab].entity[ directory[win_tab].size ].modify_time, MOD_TIME_SIZE, "%b %d %X", localtime(&(attrib.st_ctime)));
        strcpy(directory[win_tab].entity[ directory[win_tab].size ].name, dir_struct_t->d_name);
		
		directory[win_tab].size++;
	}

	closedir(dir);
}
