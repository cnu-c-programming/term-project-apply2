#ifndef FILE_IO_H
#define FILE_IO_H
#include "student.h"

int loadStudent(const char* filename, Student** head);
int saveStudent(const char* filename, Student* head);

#endif