#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

void addStudent(Student** head, int id, char* name, int score) {
    Student* newStudent = malloc(sizeof(Student));
    if (newStudent == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    };
    newStudent->id = id;
    strcpy(newStudent->name, name, sizeof(newStudent->name) - 1);
    newStudent->name[sizeof(newStudent->name) - 1] = '\0';
    newStudent->score = score;
    newStudent->next = NULL;
}
void listStudents(Student* head) {

};
void deleteStudent(Student** head, int id) {

};
void updateStudent(Student** head, int id, int score) {
    
};
void findStudent(Student** head, int id) {
    
};
void freeStudents(Student* head) {
    
};