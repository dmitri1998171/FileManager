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

void merge(struct Entity_struct list[],int start, int end, int mid) {
	int mergedList[255];
	int i, j, k;
	i = start;
	k = start;
	j = mid + 1;
	
	while (i <= mid && j <= end) {
		if (list[i].name[0] < list[j].name[0]) {
			mergedList[k] = list[i].name[0];
			k++;
			i++;
		}
		else {
			mergedList[k] = list[j].name[0];
			k++;
			j++;
		}
	}
	
	while (i <= mid) {
		mergedList[k] = list[i].name[0];
		k++;
		i++;
	}
	
	while (j <= end) {
		mergedList[k] = list[j].name[0];
		k++;
		j++;
	}
	
	for (i = start; i < k; i++) {
		list[i].name[0] = mergedList[i];
	}
}

void mergeSort(struct Entity_struct list[], int start, int end) {
	int mid;

	if (start < end) {
		mid = (start + end) / 2;
		mergeSort(list, start, mid);
		mergeSort(list, mid + 1, end);
		merge(list, start, end, mid);
	}
}

void bubbleSort(struct Entity_struct list[], int size) {
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

void sortByAlpha(struct Directory_struct directory[2]) {
	bubbleSort(directory[0].entity, directory[0].size);
	bubbleSort(directory[1].entity, directory[1].size);
}

void sortByType(struct Directory_struct directory[2]) {

}

void sortBySize(struct Directory_struct directory[2], bool direction) {

}

void sortByTime(struct Directory_struct directory[2]) {

}


