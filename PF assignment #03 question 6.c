#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct
{
    int id;
    char fullName[100];
    char batch[32];
    char membershipType[16];
    char registrationDate[11];
    char dob[11];
    char interest[16];
} Student;

void freeMemory(Student *arr) { free(arr); }

void loadDatabase(const char *filename, Student **arrPtr, int *nPtr, int *capPtr)
{
    FILE *f = fopen(filename, "rb");
    if(!f) { *arrPtr = NULL; *nPtr = 0; *capPtr = 0; return; }
    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);
    fseek(f, 0, SEEK_SET);
    if(filesize == 0) { fclose(f); *arrPtr = NULL; *nPtr = 0; *capPtr = 0; return; }
    int count = filesize / sizeof(Student);
    Student *arr = malloc(count * sizeof(Student));
    if (!arr) { perror("malloc"); fclose(f); exit(EXIT_FAILURE); }
    size_t read = fread(arr, sizeof(Student), count, f);
    if ((int)read != count) { perror("fread"); free(arr); fclose(f); exit(EXIT_FAILURE); }
    fclose(f);
    *arrPtr = arr;
    *nPtr = count;
    *capPtr = count;
}

void saveDatabase(const char *filename, Student *arr, int n) {
    FILE *f = fopen(filename, "wb");
    if (!f) { perror("fopen"); return; }
    if (n > 0) {
        if (fwrite(arr, sizeof(Student), n, f) != (size_t)n) { perror("fwrite"); }
    }
    fclose(f);
}

/* Ensure capacity for in-memory array */
void ensureCapacityStudents(Student **arrPtr, int *capPtr, int min) {
    if (*capPtr >= min) return;
    int newcap = (*capPtr == 0) ? 8 : *capPtr * 2;
    while (newcap < min) newcap *= 2;
    Student *tmp = realloc(*arrPtr, newcap * sizeof(Student));
    if (!tmp) { perror("realloc"); exit(EXIT_FAILURE); }
    *arrPtr = tmp;
    *capPtr = newcap;
}

int findStudentByID(Student *arr, int n, int id) {
    for (int i = 0; i < n; ++i) if (arr[i].id == id) return i;
    return -1;
}

void addStudent(Student **arrPtr, int *nPtr, int *capPtr, Student s, const char *filename) {
    if (findStudentByID(*arrPtr, *nPtr, s.id) != -1) {
        printf("Duplicate ID. Student not added.\n");
        return;
    }
    ensureCapacityStudents(arrPtr, capPtr, (*nPtr) + 1);
    (*arrPtr)[(*nPtr)++] = s;
    saveDatabase(filename, *arrPtr, *nPtr);
    printf("Student added and saved.\n");
}

void updateStudent(Student *arr, int n, int id, const char *newBatch, const char *newMembership, const char *filename) {
    int idx = findStudentByID(arr, n, id);
    if (idx == -1) { printf("Student not found\n"); return; }
    if (newBatch && strlen(newBatch)) strcpy(arr[idx].batch, newBatch);
    if (newMembership && strlen(newMembership)) strcpy(arr[idx].membershipType, newMembership);
    saveDatabase(filename, arr, n);
    printf("Student updated and saved.\n");
}

void deleteStudent(Student **arrPtr, int *nPtr, int *capPtr, int id, const char *filename) {
    int idx = findStudentByID(*arrPtr, *nPtr, id);
    if (idx == -1) { printf("Student not found\n"); return; }
    if (idx < (*nPtr) - 1) memmove(&(*arrPtr)[idx], &(*arrPtr)[idx + 1], ((*nPtr) - idx - 1) * sizeof(Student));
    (*nPtr)--;
    saveDatabase(filename, *arrPtr, *nPtr);
    printf("Student deleted and saved.\n");
}

void displayAll(Student *arr, int n) {
    printf("ID\tName\t\tBatch\tMembership\tRegDate\tDOB\tInterest\n");
    for (int i = 0; i < n; ++i) {
        printf("%d\t%s\t%s\t%s\t%s\t%s\t%s\n", arr[i].id, arr[i].fullName, arr[i].batch, arr[i].membershipType, arr[i].registrationDate, arr[i].dob, arr[i].interest);
    }
}

void generateBatchReport(Student *arr, int n, const char *batch, const char *membership) {
    printf("Report for batch=%s, membership=%s\n", batch, membership);
    for (int i = 0; i < n; ++i) {
        if ((strcmp(arr[i].batch, batch) == 0) && (strcmp(membership, "Both") == 0 || strcmp(arr[i].interest, membership) == 0 || strcmp(arr[i].membershipType, membership) == 0)) {
            printf("%d\t%s\t%s\t%s\n", arr[i].id, arr[i].fullName, arr[i].membershipType, arr[i].interest);
        }
    }
}

/* Simple CLI loop */
int main() {
    const char *filename = "members.dat";
    Student *arr = NULL;
    int n = 0, cap = 0;
    loadDatabase(filename, &arr, &n, &cap);

    int choice;
    while (1) {
        printf("\n1.Add 2.Update 3.Delete 4.ViewAll 5.Report 6.Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) break;
        while (getchar() != '\n');
        if (choice == 1) {
            Student s;
            printf("ID: "); scanf("%d", &s.id); while (getchar() != '\n');
            printf("Full Name: "); fgets(s.fullName, sizeof(s.fullName), stdin); if (s.fullName[strlen(s.fullName)-1]=='\n') s.fullName[strlen(s.fullName)-1]='\0';
            printf("Batch (CS/SE/Cyber/AI): "); scanf("%31s", s.batch); while (getchar() != '\n');
            printf("Membership Type (IEEE/ACM): "); scanf("%15s", s.membershipType); while (getchar() != '\n');
            printf("Registration Date (YYYY-MM-DD): "); scanf("%10s", s.registrationDate); while (getchar() != '\n');
            printf("DOB (YYYY-MM-DD): "); scanf("%10s", s.dob); while (getchar() != '\n');
            printf("Interest (IEEE/ACM/Both): "); scanf("%15s", s.interest); while (getchar() != '\n');
            addStudent(&arr, &n, &cap, s, filename);
        } else if (choice == 2) {
            int id;
            printf("Student ID to update: "); scanf("%d", &id); while (getchar() != '\n');
            char newBatch[32] = ""; char newMembership[16] = "";
            printf("New Batch (leave empty to skip): "); fgets(newBatch, sizeof(newBatch), stdin); if (newBatch[strlen(newBatch)-1]=='\n') newBatch[strlen(newBatch)-1]='\0';
            printf("New Membership Type (leave empty to skip): "); fgets(newMembership, sizeof(newMembership), stdin); if (newMembership[strlen(newMembership)-1]=='\n') newMembership[strlen(newMembership)-1]='\0';
            updateStudent(arr, n, id, newBatch, newMembership, filename);
        } else if (choice == 3) {
            int id;
            printf("Student ID to delete: "); scanf("%d", &id); while (getchar() != '\n');
            deleteStudent(&arr, &n, &cap, id, filename);
        } else if (choice == 4) {
            displayAll(arr, n);
        } else if (choice == 5) {
            char batch[32], membership[16];
            printf("Batch: "); scanf("%31s", batch); while (getchar() != '\n');
            printf("Membership/Interest (IEEE/ACM/Both): "); scanf("%15s", membership); while (getchar() != '\n');
            generateBatchReport(arr, n, batch, membership);
        } else if (choice == 6) {
            saveDatabase(filename, arr, n);
            freeMemory(arr);
            break;
        } else {
            printf("Invalid choice\n");
        }
    }
    return 0;
}
