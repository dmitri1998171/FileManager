#include "../../include/header.h"

EntitiesCounter entitiesCount(const char* path) {
	EntitiesCounter counter;
	counter.dirCounter = 0;
	counter.fileCounter = 0;
	counter.linkCounter = 0;
	counter.total = 0;

	DIR * dir;
	struct dirent * entry;

	dir = opendir(path); 
	while ((entry = readdir(dir)) != NULL) {
		counter.total++;

		if (entry->d_type == DT_REG) 
			counter.fileCounter++;

		if (entry->d_type == DT_LNK) 
			counter.linkCounter++;
	}

	counter.dirCounter = counter.total - counter.fileCounter - counter.linkCounter;
	closedir(dir);

	return counter;
}

char* pathNameConcat(Directory directory[2], int win_tab) {
    int inactiveTab = win_tab ? 0 : 1;
	int size = strlen(directory[inactiveTab].path) + strlen(directory[inactiveTab].entity[ directory[inactiveTab].highlight - 1 ].name) + 2;
	char *str = malloc(size);
	snprintf(str, size, "%s/%s", directory[inactiveTab].path, directory[inactiveTab].entity[ directory[inactiveTab].highlight - 1 ].name);
	return str;
}

void scaner(Directory directory[2], int win_tab) {
	struct dirent *dir_struct_t;
    DIR *dir;

	dir = opendir(directory[win_tab].path);
	if(dir == NULL) { 
        perror("opendir error!");
        exit(1); 
    }

	if(directory[win_tab].counter.total > 0) {
		free(directory[win_tab].entity);
	}

	int i = 0;
	directory[win_tab].highlight = 1;
	directory[win_tab].counter = entitiesCount(directory[win_tab].path);
	directory[win_tab].entity = (Entity*) malloc(sizeof(Entity) * directory[win_tab].counter.total);

	while((dir_struct_t = readdir(dir))) {
		struct stat attrib;
    	stat(dir_struct_t->d_name, &attrib);

		if(strcmp(dir_struct_t->d_name, ".") == 0) {
			continue;
	    }

		directory[win_tab].entity[ i ].type = dir_struct_t->d_type;
		directory[win_tab].entity[ i ].size = attrib.st_size;
		// strftime(directory[win_tab].entity[ i ].modify_time, MOD_TIME_SIZE, "%b %d %X", localtime(&(attrib.st_ctime)));
		directory[win_tab].entity[ i ].modify_time = attrib.st_ctime;
        strcpy(directory[win_tab].entity[ i ].name, dir_struct_t->d_name);
		
		i++;
	}

	directory[win_tab].counter.total = i;
	closedir(dir);
}

void append(char subject[], char symbol, int pos) {
	char *insert = malloc(1);
	*insert = symbol;

	char *buf = (char*)malloc(strlen(subject) + strlen(insert) + 1);

    strncpy(buf, subject, pos); 			// copy at most first pos characters
    int len = strlen(buf);
    strcpy(buf + len, insert); 				// copy all of insert[] at the end
    len += strlen(insert);  				// increase the length by length of insert[]
    strcpy(buf + len, subject + pos); 		// copy the rest

    strcpy(subject, buf);   				// copy it back to subject
	free(buf);
}

void deletechar(char *str,int pos){
	for(int i = pos; i < strlen(str); ++i)
		str[i] = str[i + 1];
}

void saveViewMode(char* path) {
    FILE* fd = fopen(path, "w");
	
	if(fd < 0) {
        perror("ERROR! Can't open the file!");
        exit(1);
    }

	for (int i = 0; i < viewMode.linesCounter; i++) {
		fwrite(viewMode.lines[i], strlen(viewMode.lines[i]), 1, fd);
	}

    fclose(fd);
}

long countLines(char* path) {
    long counter = 0;
    char buffer[STR_SIZE + 1];

	FILE* fd = fopen(path, "r");

	if(fd < 0) {
        perror("ERROR! Can't open the file!");
        exit(1);
    }

    while( fgets(buffer, STR_SIZE, fd) != NULL ) {
        counter++;
	}

    fclose(fd);
	return counter;
};

void readDir(WINDOW* wnd, char* path) {
	long i = 0;

	viewMode.x = X_OFFSET;
	viewMode.y = Y_OFFSET;
	viewMode.linesCounter = countLines(path);
	viewMode.lines = malloc(viewMode.linesCounter);

	for (long i = 0; i < viewMode.linesCounter; i++)
		viewMode.lines[i] = malloc(STR_SIZE);
	
    FILE* fd = fopen(path, "r");

    if(fd < 0) {
        perror("ERROR! Can't open the file!");
        exit(1);
    }

    while( fgets(viewMode.lines[i], STR_SIZE, fd) != NULL ) {
        int size = strlen(viewMode.lines[i]);
        viewMode.lines[i][size] = '\0';

		i++;
    }

    fclose(fd);
}

inline void bubbleSort(Entity list[], int size) {
	Entity tmp;

	for (size_t i = 0; i + 1 < size; ++i) {
		for (size_t j = 0; j + 1 < size - i; ++j) {
			if (tolower(list[j + 1].name[0]) < tolower(list[j].name[0])) {
				tmp = list[j + 1];
				list[j + 1] = list[j];
				list[j] = tmp;
			}
		}
	}
}

inline void sortList(Directory directory[2], int win_tab) {
    switch (sort) {
        case ALPHABET:
            sortByAlpha(directory, win_tab);
            break;
        case TYPE:
            sortByType(directory, win_tab);
            break;
        case SORT_SIZE_MIN:
            sortBySize(directory, win_tab, true);
            break;
        case SORT_SIZE_MAX:
            sortBySize(directory, win_tab, false);
            break;
        case MOD_TIME_MIN:
            sortByTime(directory, win_tab, true);
            break;
        case MOD_TIME_MAX:
            sortByTime(directory, win_tab, false);
            break;
    }
}

inline void sortByAlpha(Directory directory[2], int win_tab) {
	bubbleSort(directory[win_tab].entity, directory[win_tab].counter.total);
}

void sortByType(Directory directory[2], int win_tab) {
	Entity folders[directory[win_tab].counter.dirCounter];
	Entity files[(directory[win_tab].counter.fileCounter + directory[win_tab].counter.linkCounter)];
	int folCounter = 0, fileCounter = 0;

// Split entities by type
	for (int i = 0; i < directory[win_tab].counter.total; i++) {
		if(directory[win_tab].entity[i].type == DT_DIR) {
			folders[folCounter++] = directory[win_tab].entity[i];
		} else {
			files[fileCounter++] = directory[win_tab].entity[i];
		}
	}
	
// Sort every type of entities individually 
	bubbleSort(folders, folCounter);
	bubbleSort(files, fileCounter);

// Merge all entities
	int totalCounter = 0;

	for (int i = 0; i < folCounter; i++) 
		directory[win_tab].entity[totalCounter++] = folders[i];

	for (int i = 0; i < fileCounter; i++) 
		directory[win_tab].entity[totalCounter++] = files[i];
}

void sortBySize(Directory directory[2], int win_tab, bool min) {
	Entity tmp;

	if(min) {
		for (size_t i = 0; i + 1 < directory[win_tab].counter.total; ++i) {
			for (size_t j = 0; j + 1 < directory[win_tab].counter.total - i; ++j) {
				if (directory[win_tab].entity[j + 1].size < directory[win_tab].entity[j].size) {
					tmp = directory[win_tab].entity[j + 1];
					directory[win_tab].entity[j + 1] = directory[win_tab].entity[j];
					directory[win_tab].entity[j] = tmp;
				}
			}
		}
	} else {
		for (size_t i = 0; i + 1 < directory[win_tab].counter.total; ++i) {
			for (size_t j = 0; j + 1 < directory[win_tab].counter.total - i; ++j) {
				if (directory[win_tab].entity[j + 1].size > directory[win_tab].entity[j].size) {
					tmp = directory[win_tab].entity[j + 1];
					directory[win_tab].entity[j + 1] = directory[win_tab].entity[j];
					directory[win_tab].entity[j] = tmp;
				}
			}
		}
	}
}

void sortByTime(Directory directory[2], int win_tab, bool min) {
	Entity tmp;

	if(min) {
		for (size_t i = 0; i + 1 < directory[win_tab].counter.total; ++i) {
			char *str = malloc(STR_SIZE);
			snprintf(str, STR_SIZE, "%s %ld", directory[win_tab].entity[i].name, directory[win_tab].entity[i].modify_time);

			for (size_t j = 0; j + 1 < directory[win_tab].counter.total - i; ++j) {
				if (directory[win_tab].entity[j + 1].modify_time > directory[win_tab].entity[j].modify_time) {
					tmp = directory[win_tab].entity[j + 1];
					directory[win_tab].entity[j + 1] = directory[win_tab].entity[j];
					directory[win_tab].entity[j] = tmp;
				}
			}
		}
	} else {
		for (size_t i = 0; i + 1 < directory[win_tab].counter.total; ++i) {
			for (size_t j = 0; j + 1 < directory[win_tab].counter.total - i; ++j) {
				if (directory[win_tab].entity[j + 1].modify_time < directory[win_tab].entity[j].modify_time) {
					tmp = directory[win_tab].entity[j + 1];
					directory[win_tab].entity[j + 1] = directory[win_tab].entity[j];
					directory[win_tab].entity[j] = tmp;
				}
			}
		}
	}
}


