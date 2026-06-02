#ifndef STUDENT_H
#define STUDENT_H
#include <stdio.h>

typedef struct Student {
    int id;
    char name[32];
    int score;
    struct Student* next;
} Student;

void addStudent(Student** head, int id, char* name, int score);
void listStudents(Student* head);
void deleteStudent(Student** head, int id);
void updateStudent(Student* head, int id, int score);
void findStudent(Student* head, int id);
void freeStudents(Student* head);

#endif