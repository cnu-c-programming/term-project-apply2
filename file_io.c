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
    bf[strcspn(bf, "\r\n")] = '\0';
    if (strcmp(bf, "id,name,score") != 0 && strcmp(bf, "id,name,score\r") != 0) {
        printf("Error: invalid CSV header\n");
        fclose(fp);
        return -1;
    }

    while (fgets(bf, sizeof(bf), fp) != NULL) {
        bf[strcspn(bf, "\r\n")] = '\0';
        char* a = strtok(bf, ",");
        char* b = strtok(NULL, ",");
        char* c = strtok(NULL, ",");

        if(a&&b&&c) {
            int id = atoi(a);
            int score = atoi(c);

            Student* newStudent = malloc(sizeof(Student));
            if (newStudent) {
                newStudent->id = id;
                strncpy(newStudent->name, b, sizeof(newStudent->name) - 1);
                newStudent->name[sizeof(newStudent->name) - 1] = '\0';
                newStudent->score = score;
                newStudent->next = NULL;

                if (*head == NULL) {
                    *head = newStudent;
                } else {
                    Student* temp = *head;
                    while (temp->next != NULL) {
                        temp = temp->next;
                    }
                    temp->next = newStudent;
                }
                count++;
            }
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