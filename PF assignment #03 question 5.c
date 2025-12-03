#include<stdio.h>
#include<stdlib.h>
#include<string.h>


char *readLineStdin(void){
    int cap = 128;
    int len = 0;
    char *buf = malloc(cap);
    if(!buf){ perror("malloc"); exit(EXIT_FAILURE); }
    int c;
    while((c = getchar()) != EOF) {
        if(c == '\r') continue;
        if(c == '\n') break;
        if(len + 1 >= cap) {
            cap *= 2;
            char *tmp = realloc(buf, cap);
            if (!tmp) { free(buf); perror("realloc"); exit(EXIT_FAILURE); }
            buf = tmp;
        }
        buf[len++] = (char)c;
    }
    buf[len] = '\0';
    if(len == 0 && c == EOF){
        free(buf);
        return NULL;
    }
    char *exact = malloc(len + 1);
    if(!exact) { free(buf); perror("malloc"); exit(EXIT_FAILURE); }
    memcpy(exact, buf, len + 1);
    free(buf);
    return exact;
}


void ensureCapacity(char ***linesPtr, int *capacity, int min)
{
    if(*capacity >= min) return;
    int newcap = (*capacity == 0) ? 4 : *capacity;
    while (newcap < min) newcap *= 2;
    char **tmp = realloc(*linesPtr, newcap * sizeof(char *));
    if (!tmp) { perror("realloc"); exit(EXIT_FAILURE); }
    *linesPtr = tmp;
    *capacity = newcap;
}


void insertLine(char ***linesPtr, int *size, int *capacity, int index, const char *text) {
    if (index < 0) index = 0;
    if (index > *size) index = *size;
    ensureCapacity(linesPtr, capacity, (*size) + 1);
    if (index < *size) {
        memmove(&(*linesPtr)[index + 1], &(*linesPtr)[index], ((*size) - index) * sizeof(char *));
    }
    char *copy = malloc(strlen(text) + 1);
    if (!copy) { perror("malloc"); exit(EXIT_FAILURE); }
    strcpy(copy, text);
    (*linesPtr)[index] = copy;
    (*size)++;
}


void appendLine(char ***linesPtr, int *size, int *capacity, const char *text) {
    insertLine(linesPtr, size, capacity, *size, text);
}


void deleteLine(char ***linesPtr, int *size, int *capacity, int index)
{
    if (index < 0 || index >= *size) {
        printf("Invalid index\n");
        return;
    }
    free((*linesPtr)[index]);
    if (index < (*size) - 1) {
        memmove(&(*linesPtr)[index], &(*linesPtr)[index + 1], ((*size) - index - 1) * sizeof(char *));
    }
    (*size)--;
}


void printAllLines(char **lines, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d: %s\n", i + 1, lines[i]);
    }
}


void freeAll(char **lines, int size) {
    for (int i = 0; i < size; ++i) free(lines[i]);
    free(lines);
}


char **shrinkToFit(char **lines, int size, int *capacity) {
    if (size == 0) {
        free(lines);
        *capacity = 0;
        return NULL;
    }
    char **tmp = realloc(lines, size * sizeof(char *));
    if (!tmp) { perror("realloc"); exit(EXIT_FAILURE); }
    *capacity = size;
    return tmp;
}


void saveToFile(const char *filename, char **lines, int size)
{
    FILE *f = fopen(filename, "w");
    if (!f) { perror("fopen"); return; }
    for (int i = 0; i < size; ++i) {
        if (fprintf(f, "%s\n", lines[i]) < 0) { perror("fprintf"); fclose(f); return; }
    }
    fclose(f);
    printf("Saved %d lines to %s\n", size, filename);
}


char **loadFromFile(const char *filename, int *sizePtr, int *capacityPtr) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror("fopen"); return NULL; }
    char **lines = NULL;
    int size = 0, capacity = 0;
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), f))
	{
        size_t ln = strlen(buffer);
        if (ln > 0 && buffer[ln - 1] == '\n') buffer[ln - 1] = '\0';
        ensureCapacity(&lines, &capacity, size + 1);
        char *copy = malloc(strlen(buffer) + 1);
        if (!copy) { perror("malloc"); fclose(f); freeAll(lines, size); exit(EXIT_FAILURE); }
        strcpy(copy, buffer);
        lines[size++] = copy;
    }
    fclose(f);
    *sizePtr = size;
    *capacityPtr = capacity;
    return lines;
}


int main()
{
    char **lines = NULL;
    int size = 0, capacity = 0;
    char cmd[16];

    printf("Minimal Line Editor\n");
    printf("Commands: append, insert, delete, print, save, load, shrink, exit\n");

    while (1) {
        printf(">> ");
        if (scanf("%15s", cmd) != 1) break;
        while (getchar() != '\n');
        if (strcmp(cmd, "append") == 0) {
            printf("Enter line: ");
            char *ln = readLineStdin();
            if (ln) { appendLine(&lines, &size, &capacity, ln); free(ln); }
        } else if (strcmp(cmd, "insert") == 0) {
            int idx;
            printf("Index (1-based): ");
            if (scanf("%d", &idx) != 1) { while (getchar() != '\n'); continue; }
            while (getchar() != '\n');
            printf("Enter line: ");
            char *ln = readLineStdin();
            if (ln) { insertLine(&lines, &size, &capacity, idx - 1, ln); free(ln); }
        } else if (strcmp(cmd, "delete") == 0) {
            int idx;
            printf("Index (1-based): ");
            if (scanf("%d", &idx) != 1) { while (getchar() != '\n'); continue; }
            while (getchar() != '\n');
            deleteLine(&lines, &size, &capacity, idx - 1);
        } else if (strcmp(cmd, "print") == 0) {
            printAllLines(lines, size);
        } else if (strcmp(cmd, "save") == 0) {
            char filename[256];
            printf("Filename: ");
            if (scanf("%255s", filename) != 1) continue;
            while (getchar() != '\n');
            saveToFile(filename, lines, size);
        } else if (strcmp(cmd, "load") == 0) {
            char filename[256];
            printf("Filename: ");
            if (scanf("%255s", filename) != 1) continue;
            while (getchar() != '\n');
            freeAll(lines, size);
            lines = loadFromFile(filename, &size, &capacity);
            if (!lines) { size = 0; capacity = 0; }
        } else if (strcmp(cmd, "shrink") == 0) {
            lines = shrinkToFit(lines, size, &capacity);
            printf("Shrunk capacity to %d\n", capacity);
        } else if (strcmp(cmd, "exit") == 0) {
            freeAll(lines, size);
            break;
        } else {
            printf("Unknown command\n");
        }
    }
    return 0;
}
