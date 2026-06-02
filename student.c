#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

void addStudent(Student** head, int id, char* name, int score) {
    Student* newStudent = malloc(sizeof(Student));
    if (newStudent == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }

    newStudent->id = id;
    strncpy(newStudent->name, name, sizeof(newStudent->name) - 1);
    newStudent->name[sizeof(newStudent->name) - 1] = '\0';
    newStudent->score = score;
    newStudent->next = NULL;

    Student* temp = *head;
    while (temp != NULL) {
        if (temp->id == id) {
            printf("Error: duplicate ID\n");
            free(newStudent);
            return;
        }
        temp = temp->next;
    }

    if(*head == NULL) {
        *head = newStudent;
    }
    else {
        temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newStudent;
    }
    printf("Student added\n");
}
void listStudents(Student* head) {
    Student* temp = head;

    if (head == NULL) {
        printf("No students found\n");
        return;
    }

    printf("% 5s % 15s % 5s\n", "ID", "Name", "Score");
    while (temp != NULL) {
        printf("% 5d % 15s % 5d\n", temp->id, temp->name, temp->score);
        temp = temp->next;
    }
}
void deleteStudent(Student** head, int id) {
    Student *temp = *head, *prev = NULL;
    if (*head == NULL) {
        printf("ERROR: student not found\n");
        return;
    }
    while (temp != NULL) {
        if (temp -> id == id) {
            if (prev == NULL) *head = temp -> next;
            else prev->next = temp->next;

            free(temp);
            printf("Student deleted\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("ERROR: student not found\n");
}
void updateStudent(Student* head, int id, int newScore) {
    Student* temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            temp->score = newScore;
            printf("Student updated\n");
            return;
        }
        temp = temp->next;
    }
    printf("ERROR: student not found\n");
}
void findStudent(Student* head, int id) {
    Student* temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            printf("ID: %d\n", temp->id);
            printf("Name: %s\n", temp->name);
            printf("Score: %d\n", temp->score);
            return;
        }
        temp = temp->next;
    }
    printf("ERROR: student not found\n");
}
void freeStudents(Student* head) {
    while (head) {
        Student* tmp = head;
        head = head->next;
        free(tmp);
    }
}