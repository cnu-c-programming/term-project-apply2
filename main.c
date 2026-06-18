/*
 * main.c  –  Mini Student Shell
 *
 * TODO: Implement admin_shell and client_shell.
 *
 * Build:
 *   make admin   →  admin_shell  (compiled with -DADMIN_MODE)
 *   make client  →  client_shell (compiled with -DCLIENT_MODE)
 *
 * Usage:
 *   ./admin_shell [students.csv]
 *   ./admin_shell -f commands.txt [students.csv]
 *   ./client_shell [students.csv]
 *   ./client_shell -f commands.txt [students.csv]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "file_io.h"
#include "command.h"

char* globalfilename = NULL;
/* ---------------------------------------------------------------
 * TODO: Implement the interactive shell loop.
 *   - Print a prompt and read a line from stdin.
 *   - Parse the line into a command and arguments.
 *   - Dispatch to the appropriate handler function.
 *   - Loop until the user types "exit" or EOF.
 * --------------------------------------------------------------- */
 
void run_shell(Student** head) {
    char input[256];
    ShellResult result = SHELL_OK;

    #ifdef ADMIN_MODE
        const char* prompt = "admin> ";
    #else
        const char* prompt = "client> ";
    #endif
    while (result != SHELL_EXIT) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = 0;
        
        char* first_ns = input;
        while (*first_ns == ' ' || *first_ns == '\t') first_ns++;
        if (*first_ns == '\0' || *first_ns == '#') continue;

        result = process_command(input, head);
    }
}
/* ---------------------------------------------------------------
 * TODO: Implement batch mode – read commands from a file.
 *   - Open cmd_file for reading.
 *   - Execute each line as a command (same logic as run_shell).
 *   - Close the file when done.
 * --------------------------------------------------------------- */
void run_command_file(const char *cmd_file, Student** head) {
    FILE* fp = fopen(cmd_file, "r");
    if (!fp) {
        perror("Error: could not open command file");
        return;
    }

    char input[256];
    int line_num = 1;
    ShellResult result = SHELL_OK;

    while (fgets(input, sizeof(input), fp)) {
        input[strcspn(input, "\n")] = 0;

        char* first_ns = input;
        while (*first_ns == ' ' || *first_ns == '\t') first_ns++;
        if (*first_ns == '\0' || *first_ns == '#') {
            continue;
        }

        printf("[command file:%d] %s\n", line_num++, input);
        
        result = process_command(input, head);
        if (result == SHELL_EXIT) {
            break;
        } else if (result != SHELL_OK) {
            printf("Skipped line %d\n", line_num - 1);
        }
    }
    fclose(fp);
    
    if (result != SHELL_EXIT) {
        run_shell(head);
    }
}

int main(int argc, char *argv[]) {
    const char *csv_path  = NULL;
    const char *cmd_file  = NULL;

    if (argc < 2) {
        printf("Usage: %s [-f cmd_file] [students.csv]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                cmd_file = argv[++i];
            } else {
                printf("Error: -f requires a filename\n");
                return 1;
            }
        } else {
            csv_path = argv[i];
        }
    }
    if (csv_path == NULL) {
        csv_path = "students.csv";
    }
    globalfilename = (char*)csv_path;

    Student* head = NULL;
    int loaded = loadStudent(csv_path, &head);
    if (loaded >= 0) {
        printf("Loaded %d students from %s.\n", loaded, csv_path);
    }

#ifdef ADMIN_MODE
    /* Admin shell: supports add, delete, update, save, load, sort, list, find, help, exit */
    printf("[Admin Program]\n");
#elif defined(CLIENT_MODE)
    /* Client shell: supports find, list, help, exit  (read-only) */
    printf("[Client Program]\n");
#else
#error "Define either -DADMIN_MODE or -DCLIENT_MODE when compiling."
#endif

    if (cmd_file) {
        run_command_file(cmd_file, &head);
    } else {
        run_shell(&head);
    }
    freeStudents(head);
    return 0;
}
