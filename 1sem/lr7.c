# include <locale.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <windows.h>

typedef struct {
    int id;
    char name[12];
    char name2[12];
    char name3[12];
} Client;

void generate_str(char name[11]) {
    int len = rand() % 10 + 2;

    for (int i = 0; i < len; i++) {
        if (i == 0) {
            name[i] = rand() % 26 + 'A';
        } else {
            name[i] = rand() % 26 + 'a';
        }
    }
    name[len] = '\0';
}
int generate_file(unsigned int seed, unsigned int amount) {
    srand(seed);
    FILE* clients_list = fopen("clients_list.txt", "w");
    if (clients_list == NULL) {
        perror("open file");
        exit(1);
    }

    // Clients list generation
    int clients_cnt = rand() % 10 + 3;
    Client* clients = (Client*)malloc(sizeof(Client) * clients_cnt);
    int* clients_trs = (int*)malloc(sizeof(int) * clients_cnt);
    for (int i = 0; i < clients_cnt; i++) {
        Client temp;
        temp.id = rand();

        generate_str(temp.name);

        generate_str(temp.name2);

        generate_str(temp.name3);

        clients[i] = temp;
        clients_trs[i] = 0;
    }

    // File generation
    for (int i = 0; i < amount; i++) {
        int client_num = rand() % clients_cnt;
        Client client = clients[client_num];

        int day = rand() % 28 + 1;
        int month = rand() % 12 + 1;
        int year = rand() % 100;

        int hour = rand() % 24;
        int minute = rand() % 60;
        int second = rand() % 60;

        char* type = (char* []){"Prihod", "Rashod"}[rand() % 2];
        double money = rand() / 100.0;

        fprintf(clients_list,
            "%d\t%s\t%s\t%s\t%d\t%.2d.%.2d.%.2d\t%.2d.%.2d.%.2d\t%s\t%.2lf\n",
            client.id, client.name, client.name2, client.name3, ++clients_trs[client_num], day, month, year, hour, minute, second, type, money);
    }
    fclose(clients_list);
    free(clients);
    free(clients_trs);
    return 1;
}

typedef struct {
    int days;
    int months;
    int years;
} Date;

typedef struct {
    Client client;
    int amount;
    Date date;
    int hours;
    int minutes;
    int seconds;
    char type[6];
    double money;
} Transaction;

int date_to_int(Date date) {
    return date.years * 10000 + date.months * 100 + date.days;
}

int main() {
    // 3.	Найти ФИО и ID клиентов с минимальными расходами в заданный период (даты с .. по..)
    generate_file(42, 100);
    FILE* file = fopen("clients_list.txt", "r");
    if (file == NULL) {
        perror("open file");
        return 1;
    }

    int clients_cnt = 3;
    Date from = {0, 0, 0}, to = {31, 31, 99};
    scanf("%d", &clients_cnt);
    printf("Enter date in format dd.mm.yy:");
    scanf("%d.%d.%d", &from.days, &from.months, &from.years);
    printf("Enter date in format dd.mm.yy:");
    scanf("%d.%d.%d", &to.days, &to.months, &to.years);
    typedef struct {
        Client client;
        double rashod;
    } Client_rashod;

    Client_rashod* clients = (Client_rashod*)malloc(sizeof(Client_rashod));
    int size = 0;

    Transaction tr;
    while (fscanf(file, "%d\t%s\t%s\t%s\t%d\t%d.%d.%d\t%d.%d.%d\t%s\t%lf",
        &tr.client.id, tr.client.name, tr.client.name2, tr.client.name3, &tr.amount,
        &tr.date.days, &tr.date.months, &tr.date.years,
        &tr.hours, &tr.minutes, &tr.seconds, tr.type, &tr.money) == 13) {

        if (date_to_int(from) <= date_to_int(tr.date) && date_to_int(tr.date) <= date_to_int(to)
            && !strcmp(tr.type, "Rashod")) {
            bool is_in = false;
            for (int i = 0; i < size; i++) {
                if (clients[i].client.id == tr.client.id) {
                    is_in = true;
                    clients[i].rashod += tr.money;
                    break;
                }
            }
            if (!is_in) {
                realloc(clients, sizeof(Client_rashod) * ++size);
                clients[size - 1].client = tr.client;
                clients[size - 1].rashod = tr.money;
            }
        }

        int c;
        while ((c = fgetc(file)) != '\n' && c != EOF);
    }

    for (int i = 0; i < size; i++) {
        Client_rashod key = clients[i];
        int j = i - 1;
        while (j >= 0 && clients[j].rashod > key.rashod) {
            clients[j + 1] = clients[j];
            j--;
        }
        clients[j + 1] = key;
    }

    for (int i = 0; i < clients_cnt; i++) {
        Client_rashod cl = clients[i];
        printf("%d. %d %s %s %s - %.2lf\n", i + 1, cl.client.id, cl.client.name, cl.client.name2, cl.client.name3, cl.rashod);
    }

    free(clients);
    fclose(file);
    return 0;
}