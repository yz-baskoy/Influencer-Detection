#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 255

typedef struct Person
{
	int id;
	char* name[255];
	char* surname[255];
} person;



int fileReader()
{
    FILE *fp;
    person *p;
    int size = 0, i, offset, outcome;
    char *buffer, *temp;

    fp = fopen("./socialNET.txt", "r");
    if(fp == NULL) {
        printf("Error opening file");
        return -1;
    }
    buffer = (char*) malloc(sizeof(char)*BUFFER_SIZE);
    if(buffer == NULL) {
        printf("Error allocation buffer");
        return -2;
    }	
	while(!feof(fp)) {
        fgets(buffer, BUFFER_SIZE, fp);
        size++;
    }
	printf("%d", size);

    p = (person*) malloc(sizeof(person)*size);
	fseek(fp, 0, SEEK_SET);
    while(i < size) {
        fgets(buffer, BUFFER_SIZE, fp);

        temp = strtok(buffer, ",");
        p[i].id = atoi(temp);
        offset = strlen(temp) + 1;
        buffer += offset;

        temp = strtok(buffer, ",");
		strcpy(p[i].name, temp);
        offset = strlen(temp) + 1;
        buffer += offset; 

        temp = strtok(buffer, "\n"); 
		strcpy(p[i].surname, temp);
        offset = strlen(temp) + 1;
        buffer += offset; 

        i = i + 2;
    }
	for (i = 0; i < size; i++)
    {
        printf("ads id: %d\t start time: %s\tduration: %sn", p[i].id, p[i].name, p[i].surname);
    }

	free(p);
	return 0;
}

int main() {
	fileReader();
	
	return 0;
}
