#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "student.h"
#include "file_io.h"
#include "command.h"

extern char* globalfilename;

shellResult handlerList(char* args, Student** head) {
    if (*head == NULL) {
        printf("No students found.\n");
    } else {
        listStudents(*head);
    }
    return CMD_SUCCESS;
}

shellResult handlerFind(char* args, Student** head) {
    char* idStr = strtok(NULL, " \t\n");
    if (!idStr) {
        printf("Error: missing ID.\n");
        return CMD_ERROR;
    }
    findStudent(*head, atoi(idStr));
    return CMD_SUCCESS;
}

shellResult handlerReload(char* args, Student** head) {
    freeStudents(*head);
    *head = NULL;
    int count = loadStudentsFromCSV(globalfilename, head);
    if (count >= 0) printf("Reloaded %d students from %s.\n", count, globalfilename);
    return CMD_SUCCESS;
}

shellResult handlerExit(char* args, Student** head) {
    printf("Goodbye.\n");
    freeStudents(*head);
    return CMD_EXIT;
}
//어드민 모드
#ifdef ADMIN_MODE
shellResult handlerAdd(char* args, Student** head) {
    char* idStr = strtok(NULL, " \t\n");
    char* nameStr = strtok(NULL, " \t\n");
    char* scoreStr = strtok(NULL, " \t\n");

    if (!idStr || !nameStr || !scoreStr) {
        printf("Error: missing arguments.\n");
        return CMD_ERROR;
    }
    addStudent(head, atoi(idStr), nameStr, atoi(scoreStr));
    return CMD_SUCCESS;
}

shellResult handlerDelete(char* args, Student** head) {
    char* idStr = strtok(NULL, " \t\n");
    if (!idStr) {
        printf("Error: missing ID.\n");
        return CMD_ERROR;
    }
    deleteStudent(head, atoi(idStr));
    return CMD_SUCCESS;
}

shellResult handlerUpdate(char* args, Student** head) {
    char* idStr = strtok(NULL, " \t\n");
    char* scoreStr = strtok(NULL, " \t\n");
    if (!idStr || !scoreStr) {
        printf("Error: missing arguments.\n");
        return CMD_ERROR;
    }
    updateStudent(*head, atoi(idStr), atoi(scoreStr));
    return CMD_SUCCESS;
}

shellResult handlerSave(char* args, Student** head) {
    int count = saveStudentsToCSV(globalfilename, *head);
    if (count >= 0) printf("Saved %d students to %s.\n", count, globalfilename);
    return CMD_SUCCESS;
}
#endif

static Command carr[] = {
    {"list",   handlerList,   "list", "Show all students"},
    {"find",   handlerFind,   "find <id>", "Find student by ID"},
    {"reload", handlerReload, "reload", "Reload data from CSV"},
#ifdef ADMIN_MODE
    {"add",    handlerAdd,    "add <id> <name> <score>", "Add a student"},
    {"delete", handlerDelete, "delete <id>", "Delete a student"},
    {"update", handlerUpdate, "update <id> <score>", "Update student score"},
    {"save",   handlerSave,   "save", "Save data to CSV"},
#endif
    {"exit",   handlerExit,   "exit", "Exit program"}
};

shellResult processingcommand(char* input, Student** head) {
    char* cmdName = strtok(input, " \t\n");
    if (cmdName == NULL) return CMD_SUCCESS;

    int nums = sizeof(carr) / sizeof(Command);
    for (int i = 0; i < nums; i++) {
        if (strcmp(cmdName, carr[i].name) == 0) {
            return carr[i].handler(NULL, head);
        }
    }
    printf("Unknown command or permission denied.\n");
    return CMD_ERROR;
}