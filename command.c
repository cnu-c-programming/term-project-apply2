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
    if (*head == NULL) {
        printf("No student data available\n");
        return SHELL_OK;
    }
    int count = 0;
    int max = -1;
    int min = 101;
    int sum = 0;
    Student* temp = *head;
    while (temp != NULL) {
        count++;
        if (temp->score > max) max = temp->score;
        if (temp->score < min) min = temp->score;
        sum += temp->score;
        temp = temp->next;
    }
    printf("Count: %d\n", count);
    printf("Average: %.1f\n", (float)sum / count);
    printf("Max: %d\n", max);
    printf("Min: %d\n", min);
    return SHELL_OK;
}

ShellResult handle_sort(char* args, Student** head) {
    char* key = strtok(NULL, " \t\n");
    if (!key) {
        printf("Error: missing sort key\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }
    if (strcmp(key, "name") != 0 && strcmp(key, "score") != 0) {
        printf("Error: invalid sort key\n");
        return SHELL_OK;
    }
    if (*head == NULL || (*head)->next == NULL) {
        printf("sorted by %s\n", key);
        return SHELL_OK;
    }
    
    int swapped;
    Student *ptr1;
    Student *lptr = NULL;
    
    do {
        swapped = 0;
        ptr1 = *head;
        while (ptr1->next != lptr) {
            int cmp = 0;
            if (strcmp(key, "name") == 0) {
                cmp = strcmp(ptr1->name, ptr1->next->name);
            } else {
                cmp = ptr1->score - ptr1->next->score;
            }
            if (cmp > 0) {
                int temp_id = ptr1->id;
                char temp_name[32];
                strcpy(temp_name, ptr1->name);
                int temp_score = ptr1->score;
                
                ptr1->id = ptr1->next->id;
                strcpy(ptr1->name, ptr1->next->name);
                ptr1->score = ptr1->next->score;
                
                ptr1->next->id = temp_id;
                strcpy(ptr1->next->name, temp_name);
                ptr1->next->score = temp_score;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    
    printf("sorted by %s\n", key);
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
    {"sort",   handle_sort,   "sort <name|score>",       "Sort students"},
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
    {"sort",   handle_sort,   "sort <name|score>", "Sort students"},
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