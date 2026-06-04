#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"

int loadStudent(const char* filename, Student** head) {
    FILE* fp = fopen(filename, "r");
    if(fp == NULL) {
        perror("fopen");
        return 0;
    }
    char bf[256];
    int count = 0;

    if (fgets(bf, sizeof(bf), fp) == NULL) {
        fclose(fp);
        return 0;
    }
    while (fgests(bf, sizeof(bf), fp) != NULL) {
        bf[strcspn(bf, "r\n")] = '\0';
        char* a = strtok(bf, ",");
        char* b = strtok(NULL, ",");
        char* c = strtok(NULL, ",");

        if(a&&b&&c) {
            int id = atoi(a);
            int score = atoi(c);

            addStudent(head, id, b, score);
            count++;
        }
    }
    fclose(fp);
    return count;
};
int saveStudent(const char* filename, Student* head){
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Cannot Open file %s\n", filename);
        return -1;
    }
    int count = 0;

    fprintf(fp, "id,name,score\n");

    Student* temp = head;
    while (temp != NULL) {
        fprintf(fp, "%d,%s,%d\n", temp->id, temp->name, temp->score);
        count++;
        temp = temp->next;
    }
    fclose(fp);
    return count;
};