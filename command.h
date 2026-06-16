#ifndef COMMAND_H
#define COMMAND_H
#include "student.h"

typedef enum {
    CMD_SUCCESS,
    CMD_ERROR,
    CMD_EXIT
} shellResult;

typedef struct {
    const char* name;
    CommandHandler handler;
    const char* usage;
    const char* des;
} Command;

typedef shellResult (*CommandHandler)(char* args, Student** head);

shellResult processingcommand(char* input, Student** head);

#endif