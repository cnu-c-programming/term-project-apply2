#ifndef COMMAND_H
#define COMMAND_H
#include "command.h"

typedef enum {
    CMD_SUCCESS,
    CMD_ERROR,
    CMD_EXIT
} ShellResult;

typedef struct {
    const char* name;
    CommandHandler handler;
    const char* usage;
    const char* description;
} Command;

typedef ShellResult (*CommandHandler)(char* args, Student** head);

ShellResult process_command(char* input, Student** head);

#endif