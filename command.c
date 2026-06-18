#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "student.h"
#include "file_io.h"
#include "command.h"

extern char* globalfilename;

ShellResult handle_list(char* args, Student** head) {
    (void)args;
    if (*head == NULL) {
        printf("No students found.\n");
    } else {
        listStudents(*head);
    }
    return SHELL_OK;
}

ShellResult handle_find(char* args, Student** head) {
    char* idStr = strtok(NULL, " \t\n");
    if (!idStr) {
        printf("Error: missing ID.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }
    findStudent(*head, atoi(idStr));
    return SHELL_OK;
}

ShellResult handle_reload(char* args, Student** head) {
    freeStudents(*head);
    *head = NULL;
    int count = loadStudent(globalfilename, head);
    if (count >= 0) printf("Reloaded %d students from %s.\n", count, globalfilename);
    return SHELL_OK;
}

ShellResult handle_stats(char* args, Student** head) {
    printf("Statistics displayed.\n");
    return SHELL_OK;
}

ShellResult handle_clear(char* args, Student** head) {
    system("clear || cls");
    return SHELL_OK;
}

ShellResult handle_exit(char* args, Student** head) {
    printf("Goodbye.\n");
    freeStudents(*head);
    return SHELL_EXIT;
}

ShellResult handle_help(char* args, Student** head); 


#ifdef ADMIN_MODE

ShellResult handle_add(char* args, Student** head) {
    char* idStr = strtok(NULL, " \t\n");
    char* nameStr = strtok(NULL, " \t\n");
    char* scoreStr = strtok(NULL, " \t\n");
    
    if (!idStr || !nameStr || !scoreStr) {
        printf("Error: missing arguments.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }
    addStudent(head, atoi(idStr), nameStr, atoi(scoreStr));
    
    return SHELL_OK;
}

ShellResult handle_delete(char* args, Student** head) {
    char* idStr = strtok(NULL, " \t\n");
    if (!idStr) {
        printf("Error: missing ID.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }
    deleteStudent(head, atoi(idStr));
    return SHELL_OK;
}

ShellResult handle_update(char* args, Student** head) {
    char* idStr = strtok(NULL, " \t\n");
    char* scoreStr = strtok(NULL, " \t\n");
    
    if (!idStr || !scoreStr) {
        printf("Error: missing arguments.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }
    updateStudent(*head, atoi(idStr), atoi(scoreStr));
    return SHELL_OK;
}

ShellResult handle_save(char* args, Student** head) {
    int count = saveStudent(globalfilename, *head);
    if (count >= 0) printf("Saved %d students to %s.\n", count, globalfilename);
    return SHELL_OK;
}

#endif

#ifdef ADMIN_MODE
Command commands[] = {
    {"save",   handle_save,   "save",                    "Save students to CSV"},
    {"reload", handle_reload, "reload",                  "Reload students from CSV"},
    {"add",    handle_add,    "add <id> <name> <score>", "Add a student"},
    {"delete", handle_delete, "delete <id>",             "Delete a student"},
    {"update", handle_update, "update <id> <score>",     "Update student score"},
    {"find",   handle_find,   "find <id>",               "Find student"},
    {"list",   handle_list,   "list",                    "List students"},
    {"stats",  handle_stats,  "stats",                   "Show statistics"},
    {"help",   handle_help,   "help",                    "Show help"},
    {"clear",  handle_clear,  "clear",                   "Clear screen"},
    {"exit",   handle_exit,   "exit",                    "Exit shell"}
};
#endif

#ifdef CLIENT_MODE
Command commands[] = {
    {"reload", handle_reload, "reload",      "Reload students from CSV"},
    {"find",   handle_find,   "find <id>",   "Find student"},
    {"list",   handle_list,   "list",        "List students"},
    {"stats",  handle_stats,  "stats",       "Show statistics"},
    {"help",   handle_help,   "help",        "Show help"},
    {"clear",  handle_clear,  "clear",       "Clear screen"},
    {"exit",   handle_exit,   "exit",        "Exit shell"}
};
#endif

const int num_commands = sizeof(commands) / sizeof(Command);

ShellResult handle_help(char* args, Student** head) {
    printf("\nAvailable Commands:\n");
    for (int i = 0; i < num_commands; i++) {
        printf("  %-25s : %s\n", commands[i].usage, commands[i].description);
    }
    printf("\n");
    return SHELL_OK;
}

ShellResult process_command(char* input, Student** head) {
    char* cmd = strtok(input, " \t\n");
    if (cmd == NULL) {
        return SHELL_OK; 
    }
    for (int i = 0; i < num_commands; i++) {
        if (strcmp(cmd, commands[i].name) == 0) {
            return commands[i].handler(NULL, head);
        }
    }
    printf("Unknown command or permission denied.\n");
    return SHELL_ERR_UNKNOWN_COMMAND;
}