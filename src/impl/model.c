#include "../../include/header.h"

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

	directory[win_tab].entCounter = entCounter;
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

inline void bubbleSort(struct Entity_struct list[], int size) {
	struct Entity_struct tmp;

	for (size_t idx_i = 0; idx_i + 1 < size; ++idx_i) {
		for (size_t idx_j = 0; idx_j + 1 < size - idx_i; ++idx_j) {
			if (tolower(list[idx_j + 1].name[0]) < tolower(list[idx_j].name[0])) {
				tmp = list[idx_j + 1];
				list[idx_j + 1] = list[idx_j];
				list[idx_j] = tmp;
			}
		}
	}
}

inline void sortByAlpha(struct Directory_struct directory[2], int win_tab) {
	bubbleSort(directory[win_tab].entity, directory[win_tab].size);
}

void sortByType(struct Directory_struct directory[2], int win_tab) {
	struct Entity_struct folders[directory[win_tab].entCounter.dirCounter];
	struct Entity_struct files[(directory[win_tab].entCounter.fileCounter + directory[win_tab].entCounter.linkCounter)];
	int folCounter = 0, fileCounter = 0;

// Split entities by type
	for (int i = 0; i < directory[win_tab].size; i++) {
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

void sortBySize(struct Directory_struct directory[2], bool direction) {

}

void sortByTime(struct Directory_struct directory[2]) {

}


