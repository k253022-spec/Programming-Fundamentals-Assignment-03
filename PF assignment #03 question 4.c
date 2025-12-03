#include<stdio.h>

struct Book{
    int id;
    int popularity;
    int lastAccess;
};

int findBook(struct Book shelf[], int count, int id) {
    int i;
    for(i = 0; i < count; i++)
        if (shelf[i].id == id)
            return i;
    return -1;
}

int findLeastRecent(struct Book shelf[], int count) {
    int i, index = 0;
    for (i = 1; i < count; i++)
        if (shelf[i].lastAccess < shelf[index].lastAccess)
            index = i;
    return index;
}

int main(){
    int capacity, Q;
    scanf("%d %d", &capacity, &Q);

    struct Book shelf[1000];
    int count = 0;
    int time = 1;

    while (Q--)
	{
        char cmd[10];
        scanf("%s", cmd);

        if(cmd[0] == 'A' && cmd[1] == 'D'){
            int id, pop;
            scanf("%d %d", &id, &pop);
            int idx = findBook(shelf, count, id);

            if(idx != -1){
                shelf[idx].popularity = pop;
                shelf[idx].lastAccess = time++;
            } else {
                if (count == capacity) {
                    int removeIndex = findLeastRecent(shelf, count);
                    shelf[removeIndex] = shelf[count - 1];
                    count--;
                }
                shelf[count].id = id;
                shelf[count].popularity = pop;
                shelf[count].lastAccess = time++;
                count++;
            }
        }

        else {
            int id;
            scanf("%d", &id);
            int idx = findBook(shelf, count, id);
            if (idx == -1)
                printf("-1\n");
            else {
                printf("%d\n", shelf[idx].popularity);
                shelf[idx].lastAccess = time++;
            }
        }
    }

    return 0;
}
