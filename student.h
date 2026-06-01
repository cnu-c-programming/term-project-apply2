typedef struct Student {
    int id;
    char* name;
    int score;
    struct Student* next;
} Student;
void addStudent(Student** head, int id, char* name, int score){
    Student* newStudent = (Student*)malloc(sizeof(Student));
    newStudent->id = id;
    strcpy(newStudent->name, name);
    newStudent->score = score;
    newStudent->next = NULL;
    *head = newStudent;
    if(*head == NULL) {
        *head = newStudent;
        return;
    }
    
};
void printStudents(Student* head) {
    
};
void freeStudents(Student* head) {

};