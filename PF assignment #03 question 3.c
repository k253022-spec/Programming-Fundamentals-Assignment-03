#include<stdio.h>
#include<string.h>

struct Employee{
    int id;
    char name[50];
    char designation[50];
    float salary;
};

void displayEmployees(struct Employee e[], int n){
    int i;
    printf("\nID\tName\t\tDesignation\t\tSalary\n");
    for (i = 0; i < n; i++) {
        printf("%d\t%s\t\t%s\t\t%.2f\n", e[i].id, e[i].name, e[i].designation, e[i].salary);
    }
}

void findHighestSalary(struct Employee e[], int n) {
    int i, index = 0;
    for (i = 1; i < n; i++)
	{
        if (e[i].salary > e[index].salary)
            index = i;
    }
    printf("\nHighest Salary Employee:\n");
    printf("ID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
           e[index].id, e[index].name, e[index].designation, e[index].salary);
}

void searchByID(struct Employee e[], int n, int id)
{
    int i;
    for (i = 0; i < n; i++)
	{
        if (e[i].id == id){
            printf("ID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
                   e[i].id, e[i].name, e[i].designation, e[i].salary);
            return;
        }
    }
    printf("Employee not found\n");
}

void searchByName(struct Employee e[], int n, char name[])
{
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(e[i].name, name) == 0) {
            printf("ID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
                   e[i].id, e[i].name, e[i].designation, e[i].salary);
            return;
        }
    }
    printf("Employee not found\n");
}

int main()
{
    int n, i, choice, id;
    char name[50];

    printf("Enter number of employees: ");
    scanf("%d", &n);

    struct Employee e[n];

    for (i = 0; i < n; i++)
	{
        printf("\nEnter ID: ");
        scanf("%d", &e[i].id);
        printf("Enter Name: ");
        scanf("%s", e[i].name);
        printf("Enter Designation: ");
        scanf("%s", e[i].designation);
        printf("Enter Salary: ");
        scanf("%f", &e[i].salary);
    }

    displayEmployees(e, n);
    findHighestSalary(e, n);

    printf("\nSearch by:\n1. ID\n2. Name\nEnter choice: ");
    scanf("%d", &choice);

    if(choice == 1)
	{
        printf("Enter ID to search: ");
        scanf("%d", &id);
        searchByID(e, n, id);
    } else if (choice == 2) {
        printf("Enter Name to search: ");
        scanf("%s", name);
        searchByName(e, n, name);
    }

    return 0;
}
