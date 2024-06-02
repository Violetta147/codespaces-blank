#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#define MAX 30
#define SALE 2000000
#define MAX_QTY 10
#define MAX_PATH_LENGTH 512
#define MAX_ORDER_ITEMS 5
#define MAX_STRING_LENGTH 1000
#define BASE_DATA_PATH "./data/"
#define INVOICES_PATH "Invoice/"
#define FORMATTED_READ_FIELDS "%5d\t\t%[^\t]\t\t%10f\t\t%7d\t\t%[^\t]\t\n"
#define FORMATTED_READ_BILLS "%18d\t\t%[^\t]\t%19f\t%15d\t%[^\t]\t%15f\t\n"

char PASS[MAX] = "23TDT1";

char x[1] = "X";

char zerozero[3] = "00";

char yesyes[3] = "yes";

char Date[11];

int slot;

typedef struct Dishes
{
    int PIN;
    char Name[MAX];
    float Price;
    int Status;
    char Unit[MAX];
} Dish;

typedef struct Menu
{
    Dish dishes[MAX];
    int total;
} Menu;

Menu menu;

typedef struct OrderItem
{
    Dish dish;
    int quantity;
    float cash;
} OrderItem;

typedef struct Order
{
    char BID[20];
    OrderItem items[MAX];
    int total;
    float sum;
    float sale;
    float pay;
    char status[10];
} Order;

typedef struct Bill
{
    char TsCode[20];
    Order order[MAX];
    int count;
    float gain;
} Bills;

Bills Invoice;

bool order(int ordi);

void bill_calc(int ordi);

void purchase(int ordi);

void date();

bool isvalid(char *date);

bool valid_Date(char *date);

char *hash(char *date);

int dateToInt(char *date);

int getListOrders(char listFiles[MAX][MAX], int *total);

void sortDateIndex();

void daily_bills(FILE *file);

bool YN(const char *ques);

bool choose_date(char *Date1, char *Date2);

void statistic(char *date1, char *date2)
{
    return;
}

char mode, move, ans;

void clstd(); //

void yawm();

void conf();

void readMenu(const char *filename);

void enter();

void Ans();

void arg();

void wandp();

void display(); //

void printMenuAdmin(); //

void printMenuCustomer(); //

bool iseq(char Name[]);

bool isex(int PIN);

void erase();

void delete(int array[], int count);

void reset();

bool isrep(int PIN);

bool isExistDate();

void extend();

bool input(const char *question, char str[]);

void custom();

void Customize(int PIN);

void writeMenu(const char *filename);

void inputUnit(int same, char Name[], bool *Continue, char Unit[]);

void deunit(char Name[], int array[], int *count);

bool UnpaidBill();

void PayUnpaidBill();

void rewriteorder();

void Customer();

void bill_print(int count);

void bill_print_file(FILE *file, int count);

int main(void)
{
    while (true)
    {
        printf("Input X to exit the program\n");
        enter();
        if (toupper(mode) == 'A')
        {
            printf("To access to Administrator mode please enter the password\n");
            conf();
            readMenu("Food.txt");
            printMenuAdmin();
            do
            {
                Ans();
                if (toupper(move) == 'D')
                {
                    erase();
                }
                if (toupper(move) == 'A')
                {
                    extend();
                }
                if (toupper(move) == 'C')
                {
                    custom();
                }
            } while (true);
        }
        else if (toupper(mode) == 'C')
        {
            Customer();
        }
    }
    return 0;
}

void clstd()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    return;
}

void yawm()
{
    // sleep(3);
    printf("\033[2J\033[1;1H");
}

void conf()
{
    for (int i = 0; i < 5; i++)
    {
        char pass[MAX];
        printf("Password: ");
        scanf("%s", pass);
        clstd();
        if (strcmp(pass, PASS) == 0)
        {
            printf("ACCESS GRANTED\n");
            yawm();
            break;
        }
        else
        {
            if (i == 4 || strcasecmp(pass, x) == 0)
            {
                printf("Too many tries failed! Exiting program...");
                yawm();
                exit(2);
            }
            printf("Password incorrect please try again\n");
        }
    }
}

void wandp()
{
    writeMenu("Food.txt");
    printf("%18sChange applied\n", "");
    reset();
    readMenu("Food.txt");
    printMenuAdmin();
    return;
}

void readMenu(const char *fileName)
{
    char filePath[MAX_PATH_LENGTH];
    strcpy(filePath, BASE_DATA_PATH);
    strcat(filePath, fileName);

    FILE *menuP = fopen(filePath, "r");
    if (menuP == NULL)
    {
        printf("%18sUnable to read file.", "");
        exit(-1);
    }
    char hole[256];
    fscanf(menuP, "%[^\n]\n", hole);
    menu.total = 0;
    while (true)
    {
        int check = fscanf(menuP, FORMATTED_READ_FIELDS, &menu.dishes[menu.total].PIN,
                           menu.dishes[menu.total].Name, &menu.dishes[menu.total].Price,
                           &menu.dishes[menu.total].Status, menu.dishes[menu.total].Unit);
        if (check != 5)
        {
            break;
        }
        ++menu.total;
    }
    for (int i = 0; i < menu.total; i++)
    {
        for (int j = strlen(menu.dishes[i].Name) - 1; j >= 0; j--)
        {
            if (isalpha(menu.dishes[i].Name[j]) == false)
            {
                continue;
            }
            else
            {
                menu.dishes[i].Name[j + 5] = '\0';
                break;
            }
        }
    }
    for (int i = 0; i < menu.total; i++)
    {
        for (int j = strlen(menu.dishes[i].Unit) - 1; j >= 0; j--)
        {
            if (isalpha(menu.dishes[i].Unit[j]) == false)
            {
                continue;
            }
            else
            {
                menu.dishes[i].Unit[j + 1] = '\0';
                break;
            }
        }
    }
    fclose(menuP);
    return;
}

void enter()
{
    for (int k = 0; k < 5; k++)
    {
        printf("Entering in Administrator / Customer mode (A / C)? :");
        scanf("%c", &mode);
        clstd();
        if (toupper(mode) == 'A' || toupper(mode) == 'C')
        {
            break;
        }
        else
        {
            if (k == 4 || toupper(mode) == 'X')
            {
                printf("Exiting program due to can indentify user role.....\n");
                yawm();
                exit(1);
            }
            else
                printf("Invalid command\n");
        }
    }
}

void Ans()
{
    printf("%18sEnter your choice: ", "");
    for (int i = 0; i < 5; i++)
    {
        scanf("%c", &move);
        int c;
        while (( c = getchar()) != '\n' && c != EOF);
        printf("\033[%dC", 18);
        if (toupper(move) == 'A' || toupper(move) == 'C' || toupper(move) == 'D')
        {
            break;
        }
        else if (i == 4 || toupper(move) == 'X')
        {
            printf("%18sNo change have been made\n", "");
            yawm();
            exit(4);
        }
        else
        {
            printf("Invalid command\n");
            printf("%18s", "");
    }
}
}

void display()
{
    for (int i = 0; i < menu.total; i++)
    {
        if (menu.dishes[i].Status == 1)
        {
            char hole[5];
            sprintf(hole, "%d", menu.dishes[i].PIN);
            int q = ((11 - strlen(hole)) / 2) + strlen(hole) + (strlen(hole) + 1) % 2;
            printf("%18c%*d", '|', q, menu.dishes[i].PIN);
            int F = ((11 - strlen(hole)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("|");
            q = ((24 - strlen(menu.dishes[i].Name)) / 2) + strlen(menu.dishes[i].Name) +
                (strlen(menu.dishes[i].Name)) % 2;
            printf("%*s", q, menu.dishes[i].Name);
            F = ((24 - strlen(menu.dishes[i].Name)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("|");
            // price
            char hole1[30];
            sprintf(hole1, "%0.f", menu.dishes[i].Price);
            q = ((16 - strlen(hole1)) / 2) + strlen(hole1) + (strlen(hole1) + 1) % 2;
            printf("%*s", q, hole1);
            F = ((16 - strlen(hole1)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("|");
            // unit
            q = ((24 - strlen(menu.dishes[i].Unit)) / 2) + strlen(menu.dishes[i].Unit) +
                (strlen(menu.dishes[i].Unit)) % 2;
            printf("%*s", q, menu.dishes[i].Unit);
            F = ((24 - strlen(menu.dishes[i].Unit)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("|");
            printf("\n");
        }
        else if (toupper(mode) == 'A')
        {
            printf("\033[1;31m");
            char hole[5];
            sprintf(hole, "%d", menu.dishes[i].PIN);
            int q = ((11 - strlen(hole)) / 2) + strlen(hole) + (strlen(hole) + 1) % 2;
            printf("%18c%*d", '|', q, menu.dishes[i].PIN);
            int F = ((11 - strlen(hole)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("|");
            q = ((24 - strlen(menu.dishes[i].Name)) / 2) + strlen(menu.dishes[i].Name) +
                (strlen(menu.dishes[i].Name)) % 2;
            printf("%*s", q, menu.dishes[i].Name);
            F = ((24 - strlen(menu.dishes[i].Name)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("|");
            // price
            char hole1[30];
            sprintf(hole1, "%0.f", menu.dishes[i].Price);
            q = ((16 - strlen(hole1)) / 2) + strlen(hole1) + (strlen(hole1) + 1) % 2;
            printf("%*s", q, hole1);
            F = ((16 - strlen(hole1)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("|");
            // unit
            q = ((24 - strlen(menu.dishes[i].Unit)) / 2) + strlen(menu.dishes[i].Unit) +
                (strlen(menu.dishes[i].Unit)) % 2;
            printf("%*s", q, menu.dishes[i].Unit);
            F = ((24 - strlen(menu.dishes[i].Unit)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("|");
            printf("\n");
            printf("\033[0m");
        }
    }
    return;
}

void printMenuAdmin()
{
    printf("\nWelcome to Teddy Restaurant!\n");
    printf("                                            ───▄▀▀▀▄▄▄▄▄▄▄▀▀▀▄───\n");
    printf("                                            ───█▒▒░░░░░░░░░▒▒█───\n");
    printf("                                            ────█░░█░░░░░█░░█────\n");
    printf("                                            ─▄▄──█░░░▀█▀░░░█──▄▄─\n");
    printf("                                            █░░█─▀▄░░░░░░░▄▀─█░░█\n");
    printf("                                            █▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█\n");
    printf("                                            █░░╦─╦╔╗╦─╔╗╔╗╔╦╗╔╗░░█\n");
    printf("                                            █░░║║║╠─║─║─║║║║║╠─░░█\n");
    printf("                                            █░░╚╩╝╚╝╚╝╚╝╚╝╩─╩╚╝░░█\n");
    printf("                                            █▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█\n");
    printf("%18c", '+');
    for (int i = 0; i < 11; i++)
    {
        printf("-");
    }
    printf("+");
    for (int i = 0; i < 22; i++)
    {
        printf("-");
    }
    printf("+");
    for (int i = 0; i < 15; i++)
    {
        printf("-");
    }
    printf("+");
    for (int i = 0; i < 24; i++)
    {
        printf("-");
    }
    printf("+");
    printf("\n                 |    PIN📌  |        Dishes        |     Price     |          Unit          |");
    printf("\n");
    printf("%18c", '|');
    for (int i = 0; i < 11; i++)
    {
        printf("-");
    }
    printf("|");
    for (int i = 0; i < 22; i++)
    {
        printf("-");
    }
    printf("|");
    for (int i = 0; i < 15; i++)
    {
        printf("-");
    }
    printf("|");
    for (int i = 0; i < 24; i++)
    {
        printf("-");
    }
    printf("|\n");
    display();
    printf("%18c", '+');
    for (int i = 0; i < 11; i++)
    {
        printf("-");
    }
    printf("+");
    for (int i = 0; i < 22; i++)
    {
        printf("-");
    }
    printf("+");
    for (int i = 0; i < 15; i++)
    {
        printf("-");
    }
    printf("+");
    for (int i = 0; i < 24; i++)
    {
        printf("-");
    }
    printf("+\n");
    printf("%17s", "");
    printf("+");
    for(int i = 0; i < 34; i++)
    {
        printf("-");
    }
    printf("+");
    printf("\n");
    printf("%17s|1. ADD NEW FOOD(A)                |\n", "");
    printf("%17s|2. CUSTOMIZE FOOD'S INFORMATION(C)|\n", "");
    printf("%17s|3. DELTE FOOD(D)                  |\n", "");
    printf("%17s|4. EXIT(X)                        |\n", "");
    printf("%17s", "");
    printf("+");
    for(int i = 0; i < 34; i++)
    {
        printf("-");
    }
    printf("+");
    printf("\n");
}
void printMenuCustomer()
{
     printf("\nWelcome to Teddy Restaurant!\n");
    printf("                                            ───▄▀▀▀▄▄▄▄▄▄▄▀▀▀▄───\n");
    printf("                                            ───█▒▒░░░░░░░░░▒▒█───\n");
    printf("                                            ────█░░█░░░░░█░░█────\n");
    printf("                                            ─▄▄──█░░░▀█▀░░░█──▄▄─\n");
    printf("                                            █░░█─▀▄░░░░░░░▄▀─█░░█\n");
    printf("                                            █▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█\n");
    printf("                                            █░░╦─╦╔╗╦─╔╗╔╗╔╦╗╔╗░░█\n");
    printf("                                            █░░║║║╠─║─║─║║║║║╠─░░█\n");
    printf("                                            █░░╚╩╝╚╝╚╝╚╝╚╝╩─╩╚╝░░█\n");
    printf("                                            █▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█\n");
    printf("%18c", '+');
    for (int i = 0; i < 11; i++)
    {
        printf("-");
    }
    printf("+");
    for (int i = 0; i < 22; i++)
    {
        printf("-");
    }
    printf("+");
    for (int i = 0; i < 15; i++)
    {
        printf("-");
    }
    printf("+");
    for (int i = 0; i < 24; i++)
    {
        printf("-");
    }
    printf("+");
    printf("\n                 |    PIN📌  |        Dishes        |     Price     |          Unit          |");
    printf("\n");
    printf("%18c", '|');
    for (int i = 0; i < 11; i++)
    {
        printf("-");
    }
    printf("|");
    for (int i = 0; i < 22; i++)
    {
        printf("-");
    }
    printf("|");
    for (int i = 0; i < 15; i++)
    {
        printf("-");
    }
    printf("|");
    for (int i = 0; i < 24; i++)
    {
        printf("-");
    }
    printf("|\n");
    display();
    printf("%18c", '+');
    for (int i = 0; i < 11; i++)
    {
        printf("-");
    }
    printf("+");
    for (int i = 0; i < 22; i++)
    {
        printf("-");
    }
    printf("+");
    for (int i = 0; i < 15; i++)
    {
        printf("-");
    }
    printf("+");
    for (int i = 0; i < 24; i++)
    {
        printf("-");
    }
    printf("+\n");
}


void arg()
{
    do
    {
        printf("%18sAre you sure to save change?(Y/N) : ", "");
        scanf("%c", &ans);
        printf("\033[%dC", 18);
        clstd();
    } while (toupper(ans) != 'N' && toupper(ans) != 'Y' && toupper(ans) != 'X');
    return;
}

void erase()
{
    int count = 1;
    int del[count];
    printf("Enter the Food PIN you wanna eradicate: ");
    while (scanf("%d", &del[0]) != 1 || isex(del[0]) == false)
    {
        clstd();
        printf("%18sInvalid PIN!\n", "");
        printf("%18sEnter the Food PIN you wanna eradicate: ", "");
    }
    clstd();
    arg();
    if (toupper(ans) == 'N')
    {
        printf("%18sNo change was applied!\n", "");
        yawm();
        printMenuAdmin();
    }
    else if (toupper(ans) == 'Y')
    {
        delete (del, count);
        printf("\033[2J\033[1;1H");
        wandp();
    }
    else
    {
        printf("%18sExiting program!\n", "");
        yawm();
        exit(1);
    }
}

void delete(int array[], int count)
{
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < menu.total; j++)
        {
            if (menu.dishes[j].PIN == array[i])
            {
                for (int k = j; k < menu.total - 1; k++)
                {
                    menu.dishes[k] = menu.dishes[k + 1];
                }
                (menu.total)--;
                break;
            }
        }
    }
    return;
}

void writeMenu(const char *fileName)
{
    char filePath[MAX_PATH_LENGTH];
    strcpy(filePath, BASE_DATA_PATH);
    strcat(filePath, fileName);

    FILE *menuP = fopen(filePath, "w");
    if (menuP == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }
    int longestPriceDigits = 0, longestName = 0, longestUnit = 0;
    for (int i = 0; i < menu.total; i++)
    {
        int priceDigits = log10(menu.dishes[i].Price) + 1;
        if (priceDigits > longestPriceDigits)
            longestPriceDigits = priceDigits;

        int nameLength = strlen(menu.dishes[i].Name);
        if (nameLength > longestName)
            longestName = nameLength;

        int unitLength = strlen(menu.dishes[i].Unit);
        if (unitLength > longestUnit)
            longestUnit = unitLength;
    }

    // Determine number of tabs for each field
    int nameTabs = (longestName >= 4) ? 1 : 2;
    int priceTabs = (longestPriceDigits >= 4) ? 1 : 2;

    // Header
    fprintf(menuP, "%5s", "PIN");
    fprintf(menuP, "\t\t%-*s", longestName + (nameTabs - 1) * 4, "Name📛");
    fprintf(menuP, "\t\t%-*s", longestPriceDigits + (priceTabs - 1) * 4, "Price");
    fprintf(menuP, "\t\t%-*s", longestPriceDigits + (priceTabs - 1) * 4, "Status");
    fprintf(menuP, "\t\t%s\t\n", "Unit");

    // Data
    for (int i = 0; i < menu.total; i++)
    {
        fprintf(menuP, "%5d", menu.dishes[i].PIN);
        fprintf(menuP, "\t\t%-*s", longestName + (nameTabs - 1) * 4, menu.dishes[i].Name);
        fprintf(menuP, "\t\t%-*.0f", longestPriceDigits + (priceTabs - 1) * 4,
                menu.dishes[i].Price);
        fprintf(menuP, "\t\t  %-*d", longestPriceDigits + (priceTabs - 1) * 4 - 2,
                menu.dishes[i].Status);
        fprintf(menuP, "\t\t  %s\t\n", menu.dishes[i].Unit);
    }

    fclose(menuP);
    return;
}

void reset()
{
    int array[MAX];
    int count = 0;
    for (int i = 0; i < menu.total; i++)
    {
        array[i] = menu.dishes[i].PIN;
        count++;
    }
    delete (array, count);
    return;
}

void extend()
{
    float FPrice;
    int same = 0;
    bool Continue = true;
    int FPIN, Stat;
    char FName[MAX], Unit[10];
    // clstd();
    while (input("Give the new Food name", FName) == false)
        printf("                  Invalid food name!\n");
    while (iseq(FName) == true)
    {
        printf("                  Your food name is already been used, if you want to continue with this name you "
               "                  must use a different unit with the currently active dishes!\n");
        if (YN("                  Are you sure with your choice?") == true)
        {
            same = 1;
            break;
        }
        while (input("                Change the Food name", FName) == false)
            printf("                  Invalid food name!\n");
    }
    printf("                  Give the price: ");
    while (scanf("%f", &FPrice) != 1)
    {
        clstd();
        printf("                  Please input a valid price!\n");
        printf("                  Give the price: ");
    }
    printf("                  Give the Food PIN: ");
    while (scanf("%d", &FPIN) != 1 || isrep(FPIN) == true || FPIN == 0)
    {
        clstd();
        printf("                  Your PIN maybe invalid or already used, please try another one!\n");
        printf("                  Give the Food PIN: ");
    }
    // clstd();
    printf("                  Give the Food Status:(1 for Available and 0 for the opposite) ");
    while (scanf("%d", &Stat) != 1 || (Stat != 0 && Stat != 1))
    {
        clstd();
        printf("                  Invalid status value!\n");
        printf("                  Give the Food status: ");
    }
    clstd();
    inputUnit(same, FName, &Continue, Unit);
    arg();
    if (toupper(ans) == 'N')
    {
        printf("                  No change was applied!");
        yawm();
        printMenuAdmin();
    }
    else if (toupper(ans) == 'Y')
    {

        menu.dishes[menu.total].PIN = FPIN;
        printf("%s\n", FName);
        strcpy(menu.dishes[menu.total].Name, FName);
        printf("%s\n", menu.dishes[menu.total].Name);
        menu.dishes[menu.total].Price = FPrice;
        menu.dishes[menu.total].Status = Stat;
        strcpy(menu.dishes[menu.total].Unit, Unit);
        menu.total += 1;
        printf("\033[2J\033[1;1H");
        wandp();
    }
    else
    {
        printf("                  Exiting program!\n");
        yawm();
        exit(1);
    }
}

bool isrep(int PIN)
{
    for (int i = 0; i < menu.total; i++)
    {
        if (menu.dishes[i].PIN == PIN)
        {
            slot = i;
            return true;
        }
    }
    return false;
}

bool isex(int PIN)
{
    for (int i = 0; i < menu.total; i++)
    {
        if (menu.dishes[i].PIN == PIN)
            return true;
    }
    printf("                  No food match this PIN!\n");
    return false;
}

bool iseq(char Name[])
{
    for (int i = 0; i < menu.total; i++)
    {
        if (strcasecmp(menu.dishes[i].Name, Name) == 0)
            return true;
    }
    return false;
}

void custom()
{
    int FID;
    printf("Give me the PIN of the food u wanna customize: ");
    while (scanf("%d", &FID) != 1 || isrep(FID) == false)
    {
        clstd();
        printf("                  Sorry but no PIN was found matched your input!\n");
        printf("                  Give me the PIN of the food u wanna customize: ");
    }
    if (FID != 0)
        Customize(FID);
    else
        return;
}
void Customize(int PIN)
{
    Dish array;
    bool Continue = false;
    char yN;
    int same = 0;
    int Pivot = 0;
    for (int i = 0; i < menu.total; i++)
    {
        if (menu.dishes[i].PIN == PIN)
        {
            Pivot = i;
            break;
        }
    }
    printf("                  Here you can change its elements, type 0 if u dont wanna make change with that "
           "element: \n");
    printf("                  Food current name: %s\n", menu.dishes[Pivot].Name);
    clstd();
    while (input("                  Food new name", array.Name) == false)
        printf("                  Invalid food name!\n");
    while (iseq(array.Name) == true)
    {
        printf("                  Your food name is already been used, if you want to continue with this name you "
               "                  must use a different unit with the currently active dishes!\n");
        if (YN("                  Are you sure with your choice?") == true)
        {
            same = 1;
            break;
        }
        printf("                  Food current name: %s\n", menu.dishes[Pivot].Name);
        while (input("                Food new name", array.Name) == false)
            printf("                  Invalid food name!\n");
    }
    array.PIN = menu.dishes[Pivot].PIN;
    printf("                  Food current Price: %0.f\n", menu.dishes[Pivot].Price);
    printf("                  Food new Price: ");
    while (scanf("%f", &array.Price) != 1)
    {
        printf("                  Invalid price, please reinput!\n");
        printf("                  Food current Price: %0.f\n", menu.dishes[Pivot].Price);
        printf("                  Food new Price: ");
        clstd();
    }
    if (strcmp(array.Name, "0") == 0)
        strcpy(array.Name, menu.dishes[Pivot].Name);
    clstd();
    if (array.Price == 0)
        array.Price = menu.dishes[Pivot].Price;
    printf("                  Food current Status: %s\n",
           ((menu.dishes[Pivot].Status) % 2 == 1) ? "Available" : "Unavailable");
    do
    {
        printf("                  Do you want to switch status?(Y/N): ");
        scanf("%c", &yN);
        clstd();
    } while (toupper(yN) != 'N' && toupper(yN) != 'Y');
    if (toupper(yN) == 'Y')
        array.Status = (menu.dishes[Pivot].Status + 1) % 2;
    else
        array.Status = menu.dishes[Pivot].Status;
    printf("                  Food current Unit: %s\n", menu.dishes[Pivot].Unit);
    inputUnit(same, array.Name, &Continue, array.Unit);
    if (strcmp(array.Unit, "0") == 0)
        strcpy(array.Unit, menu.dishes[Pivot].Unit);
    arg();
    if (toupper(ans) == 'Y')
    {
        menu.dishes[Pivot] = array;
    }
    else if (toupper(ans) == 'N')
    {
        printf("                  No change was applied!\n");
        yawm();
        printMenuAdmin();
    }
    else
    {
        printf("                  Exiting program!\n");
        yawm();
        exit(1);
    }
    printf("\033[2J\033[1;1H");
    wandp();
    return;
}

void date()
{
    do
    {
        printf("Working Date(dd-mm-yyyy): ");
        scanf("%s", Date);
        clstd();
    } while (isvalid(Date) == false);
}

bool isvalid(char *date)
{
    if (date[2] != '-' || date[5] != '-')
        return false;
    for (int i = 0; i < 10; i++)
    {
        if (i != 2 && i != 5)
        {
            if (isdigit(date[i]) == 0)
                return false;
        }
    }
    return valid_Date(date);
}

bool valid_Date(char *date)
{
    int year;
    int month;
    int day;
    sscanf(date, "%d-%d-%d", &day, &month, &year);
    if (year > 2124 || year < 2024)
        return false;
    if (month > 12 || month < 1)
        return false;
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 ||
        month == 12)
    {
        if (day > 31 || day < 1)
            return false;
        else
            return true;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day > 30 || day < 1)
            return false;
        else
            return true;
    }
    if (month == 2)
    {
        if (year % 4 == 0 && year % 400 != 0)
        {
            if (day < 1 || day > 29)
                return false;
            else
                return true;
        }
        else
        {
            if (day < 1 || day > 28)
                return false;
            else
                return true;
        }
    }
    return false;
}

char *hash(char *date)
{
    strncat(Invoice.TsCode, date + 6, 4);
    strncat(Invoice.TsCode, date + 3, 2);
    strncat(Invoice.TsCode, date, 2);
    Invoice.count = 0;
    return Invoice.TsCode;
}

int getListOrders(char listFiles[MAX][MAX], int *total)
{
    char filePath[MAX_PATH_LENGTH];
    strcpy(filePath, BASE_DATA_PATH);
    strcat(filePath, INVOICES_PATH);
    strcat(filePath, "index.txt");
    FILE *index = fopen(filePath, "r");
    if (index == NULL)
    {
        printf("Unable to read file.\n");
        exit(-1);
    }

    *total = 0;
    while (true)
    {
        char invoiceFileName[MAX];
        int check = fscanf(index, "%s\n", invoiceFileName);
        if (check != 1)
        {
            fscanf(index, "\n");
            break;
        }

        strcpy(listFiles[*total], invoiceFileName);
        ++*total;
    }
    fclose(index);
    return 0;
}

void sortDateIndex()
{
    char filePath[MAX_PATH_LENGTH];
    strcpy(filePath, BASE_DATA_PATH);
    strcat(filePath, INVOICES_PATH);
    strcat(filePath, "index.txt");
    FILE *indexFile = fopen(filePath, "r");
    if (indexFile == NULL)
    {
        printf("Unable to read file.");
        exit(-1);
    }
    char listFiles[MAX][MAX];
    int totalFileIndex = 0;
    getListOrders(listFiles, &totalFileIndex);
    int dateInts[MAX];
    for (int i = 0; i < totalFileIndex; i++)
    {
        dateInts[i] = dateToInt(listFiles[i]);
    }
    for (int i = 0; i < totalFileIndex - 1; i++)
    {
        for (int j = i + 1; j < totalFileIndex; j++)
        {
            if (dateInts[i] > dateInts[j])
            {
                int tempInt = dateInts[i];
                dateInts[i] = dateInts[j];
                dateInts[j] = tempInt;
                char tempStr[MAX];
                strcpy(tempStr, listFiles[i]);
                strcpy(listFiles[i], listFiles[j]);
                strcpy(listFiles[j], tempStr);
            }
        }
    }
    fclose(indexFile);
    indexFile = fopen(filePath, "w");
    if (indexFile == NULL)
    {
        printf("Unable to open file.");
        exit(-1);
    }
    for (int i = 0; i < totalFileIndex; i++)
    {
        fprintf(indexFile, "%s\n", listFiles[i]);
    }
    fclose(indexFile);
}

int dateToInt(char *date)
{
    int day, month, year;
    sscanf(date, "%d-%d-%d", &day, &month, &year);
    return year * 10000 + month * 100 + day;
}

void daily_bills(FILE *file)
{
    char buffer[130];
    Invoice.count = 0;
    Invoice.order[Invoice.count].total = 0;
    Invoice.gain = 0;
    while (!feof(file))
    {
        char buff[20];
        if (fgets(buff, sizeof(buff), file) == NULL)
            return;
        sscanf(buff, "Bill ID: %20s", Invoice.order[Invoice.count].BID);
        fgets(buffer, sizeof(buffer), file);
        int k;
        do
        {
            k = fscanf(
                file, FORMATTED_READ_BILLS,
                &Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].dish.PIN,
                Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].dish.Name,
                &Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].dish.Price,
                &Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].quantity,
                Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].dish.Unit,
                &Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].cash);
            if (k == 6)
                Invoice.order[Invoice.count].total++;
        } while (k == 6);
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%*101s%17f", &Invoice.order[Invoice.count].sum);
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%*101s%17f", &Invoice.order[Invoice.count].sale);
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%*101s%17f", &Invoice.order[Invoice.count].pay);
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%*101s%17s", Invoice.order[Invoice.count].status);
        if (strcmp(Invoice.order[Invoice.count].status, "Paid") == 0)
            Invoice.gain += Invoice.order[Invoice.count].pay;
        Invoice.count++;
    }
    return;
}

bool YN(const char *ques)
{
    char c;
    do
    {
        printf("%s [Y/N]: ", ques);
        scanf("%c", &c);
        clstd();
        if (toupper(c) == 'Y')
            return true;
        if (toupper(c) == 'N')
            return false;
    } while (toupper(c) != 'Y' && toupper(c) != 'N');
    return true;
}

bool choose_date(char *Date1, char *Date2)
{
    int year1, year2, month1, month2, day1, day2;
    do
    {
        printf("Starting Date(dd-mm-yyyy): ");
        scanf("%s", Date1);
        clstd();
    } while (isvalid(Date1) == false);
    sscanf(Date1, "%d-%d-%d", &day1, &month1, &year1);
    char *end;
    do
    {
        printf("Ending Date(dd-mm-yyyy): ");
        scanf("%s", Date2);
        clstd();
    } while (isvalid(Date2) == false);
    sscanf(Date2, "%d-%d-%d", &day2, &month2, &year2);
    if (year2 > year1)
        return true;
    else if (year2 < year1)
        return false;
    else if (year2 == year1)
    {
        if (month2 > month1)
            return true;
        else if (month2 < month1)
            return false;
        else if (month2 == month1)
        {
            if (day2 >= day1)
                return true;
            else
                return false;
        }
    }
    return true;
}

bool order(int ordi)
{
    int FPIN, i;
    int flag = 0;
    int quantity;
    do
    {
        printf("                  Type in the PIN of the food you want to order: ");
        while (scanf("%d", &FPIN) != 1)
        {   
            printf("\033[%dC", 18);
            clstd();
            printf("Invalid PIN, please reinput: ");
        }
        clstd();
        if (FPIN == 0)
        {
            printf("                  You have just cancelling ordering!\n");
            if (Invoice.order[ordi].total != 0)
            {
                printf("Calculating your order...!\n");
                bill_calc(ordi);
                return true;
            }
            else if (Invoice.order[ordi].total == 0)
            {
                printf("                  You have not order anything!\n");
                return false;
            }
        }
        if (isrep(FPIN) == true)
        {
            for (i = 0; i < Invoice.order[ordi].total; i++)
            {
                if (Invoice.order[ordi].items[i].dish.PIN == FPIN)
                {
                    flag = 1;
                    break;
                }
            }
            printf("                  How many of this food do you want? ");
            while (scanf("%d", &quantity) != 1 || quantity > 9999 || quantity < 1)
            {   
                printf("\033[%dC", 18);
                clstd();
                printf("Invalid quantity, please rechoose: ");
            }
            clstd();
            if (flag == 0)
            {
                Invoice.order[ordi].items[Invoice.order[ordi].total].dish =
                    menu.dishes[slot];
                Invoice.order[ordi].items[Invoice.order[ordi].total].quantity =
                    quantity;
                Invoice.order[ordi].items[Invoice.order[ordi].total].cash =
                    (float)(Invoice.order[ordi]
                                .items[Invoice.order[ordi].total]
                                .dish.Price *
                            quantity);
                Invoice.order[ordi].total++;
            }
            else
            {
                Invoice.order[ordi].items[i].quantity += quantity;
                Invoice.order[ordi].items[i].cash +=
                    (float)(Invoice.order[ordi].items[i].dish.Price * quantity);
                flag = 0;
            }
        }
        else
            printf("                  No food match this PIN!\n");
    } while (isrep(FPIN) == false || Invoice.order[ordi].total < 5);
    if (Invoice.order[ordi].total == 5)
    {
        printf("                  You have reached your order limits, we are calculating your order!\n");
        bill_calc(ordi);
    }
    return true;
}

void bill_calc(int ordi)
{
    char filename[15];
    char filePath[MAX_PATH_LENGTH];
    strcpy(filePath, BASE_DATA_PATH);
    strcat(filePath, INVOICES_PATH);
    int temp = ordi;
    sprintf(filename, "%s.txt", Date);
    ordi = temp;
    strcat(filePath, filename);
    FILE *invoice = fopen(filePath, "a+");
    if (invoice == NULL)
    {
        return;
    }
    for (int i = 0; i < Invoice.order[ordi].total; i++)
    {
        Invoice.order[ordi].sum += Invoice.order[ordi].items[i].cash;
    }
    if (Invoice.order[ordi].sum >= 100000)
    {
        Invoice.order[ordi].sale = Invoice.order[ordi].sum * 25 / 100;
        Invoice.order[ordi].sale = ceil(Invoice.order[ordi].sale / 1000) * 1000;
    }
    else
        Invoice.order[ordi].sale = 0;
    Invoice.order[ordi].pay =
        floor(Invoice.order[ordi].sum - Invoice.order[ordi].sale);
    printf("_______________________________________________________________________________________"
           "________________________________________________  \n");
    bill_print(ordi);
    bill_print_file(invoice, ordi);
    if (YN("Do you want to purchase your order now?") == false)
    {
        printf("Your order have been saved!\n");
        strcpy(Invoice.order[ordi].status, "Unpaid");
        fprintf(invoice, "%101s%15s\n", "Status:", Invoice.order[ordi].status);
    }
    else
    {
        purchase(ordi);
        fprintf(invoice, "%101s%15s\n", "Status:", Invoice.order[ordi].status);
    }
    fclose(invoice);
    return;
}

void purchase(int ordi)
{
    int temp = ordi;
    ordi = temp;
    float pay;
    int k;
    do
    {
        printf("Your total amount is %.0fvnd, you give(in vnd): ",
               Invoice.order[ordi].pay);
        k = scanf("%f", &pay);
        if (k != 1 || pay <= 0)
            printf("Please give a valid amount of cash!\n");
        clstd();
    } while (k != 1 || pay <= 0);
    if (pay >= Invoice.order[ordi].pay)
    {
        printf("We have receive %.0fvnd from you, your order have been purchase and you will "
               "receive %.0fvnd in charge\n",
               pay, pay - Invoice.order[ordi].pay);
        printf("Thank you for using our service, wish you a nice day!\n");
        strcpy(Invoice.order[ordi].status, "Paid");
        Invoice.gain += Invoice.order[ordi].pay;
    }
    else
    {
        float capital_sum = pay;
        do
        {
            printf("We sorry that your credit isn't enough, we have just receive a total paid "
                   "of %.0fvnd and still lack of %.0fvnd in cash!\n",
                   capital_sum, Invoice.order[ordi].pay - capital_sum);
            do
            {
                printf("How much in vnd you are paying now: ");
                k = scanf("%f", &pay);
                if (k != 1 || pay <= 0)
                    printf("Please give a valid amount of cash!\n");
                clstd();
            } while (k != 1 || pay <= 0);
            capital_sum += pay;
        } while (capital_sum < Invoice.order[ordi].pay);
        printf("We have receive %.0fvnd from you, your order have been purchase and you will "
               "receive %.0fvnd in charge\n",
               capital_sum, capital_sum - Invoice.order[ordi].pay);
        printf("Thank you for using our service, wish you a nice day!\n");
        strcpy(Invoice.order[ordi].status, "Paid");
        Invoice.gain += Invoice.order[ordi].pay;
    }
}

bool input(const char *question, char str[])
{
    printf("%s: ", question);
    fgets(str, MAX_STRING_LENGTH, stdin);
    str[strlen(str) - 1] = '\0';
    if (strlen(str) > 0)
    {
        return true;
    }
    return false;
}

void deunit(char Name[], int array[], int *count)
{
    for (int i = 0; i < menu.total; i++)
    {
        if (strcmp(menu.dishes[i].Name, Name) == 0)
        {
            array[*count] = i;
            *count = *count + 1;
        }
    }
    printf("Here are all units that the dishes %s already have:\n", Name);
    for (int i = 0; i < *count; i++)
    {
        printf("%d. %s.\n", i + 1, menu.dishes[array[i]].Unit);
    }
    return;
}

void inputUnit(int same, char Name[], bool *Continue, char Unit[])
{
    if (same == 1)
    {
        if (YN("                  Seems like your name is taken! Do you consider changing the name now(else we "
               "                  continue)?") == true)
        {
            same = 0;
            while (input("                Change the Food name", Name) == false)
                printf("                  Invalid food name!\n");
            while (iseq(Name) == true)
            {
                clstd();
                printf("                  This name is still already been used, if you want to continue with this "
                       "                  name you must use a different unit with the currently active dishes!\n");
                if (YN("                  Are you sure with your choice?") == true)
                {
                    same = 1;
                    break;
                }
                while (input("                Change the Food name", Name) == false)
                    printf("                  Invalid food name!\n");
            }
            inputUnit(same, Name, &*Continue, Unit);
        }
        else
        {
            *Continue = true;
            int sameUnit[100];
            int count = 0;
            deunit(Name, sameUnit, &count);

            do
            {
                while (input("                Change the Food unit", Unit) == false)
                    printf("Invalid food unit!\n");
                for (int i = 0; i < count; i++)
                {
                    if (strcasecmp(Unit, menu.dishes[sameUnit[i]].Unit) == 0)
                        *Continue = false;
                }
            } while (*Continue == false);
        }
    }
    else
    {
        while (input("                  Give the Food unit", Unit) == false)
            printf("                  Invalid food unit!\n");
    }
    return;
}

bool UnpaidBill()
{
    for (int i = 0; i < Invoice.count; i++)
    {
        if (strcmp(Invoice.order[i].status, "Unpaid") == 0)
            return true;
    }
    return false;
}

void PayUnpaidBill()
{
    int point[1000];
    int count = 0;
    int choose;
    for (int i = 0; i < Invoice.count; i++)
    {
        if (strcmp(Invoice.order[i].status, "Unpaid") == 0)
        {
            point[count] = i;
            count++;
        }
    }
    printf("Here are all the bill that have not been paid today:\n");
    for (int i = 0; i < count; i++)
    {
        printf("%d. Bill %d cost %.0fvnd\n", i + 1, point[i] + 1,
               Invoice.order[point[i]].sum - Invoice.order[point[i]].sale);
    }
    printf("Please choose the Bill you want to perform by its ordi number: ");
    while (scanf("%d", &choose) != 1 && choose > count && choose < 1)
    {
        clstd();
        printf("Your choice doesnt match anything please rechoose: ");
    }
    clstd();
    if (YN("Do you want to order more food before paying?") == true)
    {
        printf("Here's the menu!\n");
        printMenuCustomer();
        printf("\n");
        printf("\n");
        printf("                                                  HERE IS YOU CURRENT BILL✨\n");
        printf("\n");
        printf("______________________________________________________________________________________________"
               "______________________________________________________\n");
        bill_print(point[choose - 1]);
        printf("______________________________________________________________________________________________"
               "______________________________________________________\n");
               printf("\n");
        Invoice.order[point[choose - 1]].pay = 0;
        Invoice.order[point[choose - 1]].sum = 0;
        Invoice.order[point[choose - 1]].sale = 0;
        order(point[choose - 1]);
    }
    else
    {
        purchase(point[choose - 1]);
    }
    rewriteorder();
}

void rewriteorder()
{
    char filename[20];
    char filePath[MAX_PATH_LENGTH];
    strcpy(filePath, BASE_DATA_PATH);
    strcat(filePath, INVOICES_PATH);
    sprintf(filename, "%s.txt", Date);
    strcat(filePath, filename);
    FILE *invoice = fopen(filePath, "w");
    if (invoice == NULL)
    {
        return;
    }
    fprintf(invoice, "TS code: %s\n", Invoice.TsCode);
    for (int i = 0; i < Invoice.count; i++)
    {
        bill_print_file(invoice, i);
        fprintf(invoice, "%101s%15s\n", "Status:", Invoice.order[i].status);
    }
    fclose(invoice);
}

void Customer()
{
    yawm();
    char *filename = (char *)(malloc(20 * sizeof(char)));
    char filePath[MAX_PATH_LENGTH];
    strcpy(filePath, BASE_DATA_PATH);
    strcat(filePath, INVOICES_PATH);
    date();
    strcat(Date, "\0");
    sprintf(filename, "%s.txt", Date);
    strcat(filePath, filename);
    FILE *invoice = fopen(filePath, "a+");
    if (invoice == NULL)
    {
        printf("Cannot open the file\n");
        fclose(invoice);
        exit(11);
    }
    strcpy(filePath, BASE_DATA_PATH);
    strcat(filePath, INVOICES_PATH);
    strcat(filePath, "index.txt");
    FILE *index = fopen(filePath, "a+");
    if (index == NULL)
    {
        printf("Cannot open the file\n");
        fclose(index);
    }
    bool isExist = isExistDate();
    if (!isExist)
    {
        fprintf(index, "%s.txt\n", Date);
    }
    fclose(index);
    sortDateIndex();
    rewind(invoice);
    char buffer[20];
    if (fgets(buffer, sizeof(buffer), invoice) != NULL)
    {
        sscanf(buffer, "TS code: %19s\n", Invoice.TsCode);
        daily_bills(invoice);
        fclose(invoice);
    }
    else
    {
        char *k = hash(Date);
        fprintf(invoice, "TS code: %s\n", Invoice.TsCode);
        fclose(invoice);
    }
    bool isLoop = true;
    while (isLoop)
    {
        char choice;
        printf("1.Order and pay\n");
        printf("2.Unpaid bills\n");
        printf("3.Exit\n");
        printf("4.Exit and end day\n");
        printf("Please choose 1 option by pressing that key button: ");
        scanf("%c", &choice);
        readMenu("Food.txt");
        clstd();
        if (choice == '1')
        {
            printMenuCustomer();
            bool k = order(Invoice.count);
            if (k == true)
                Invoice.count++;
        }
        else if (choice == '2')
        {
            if (UnpaidBill() == true)
            {
                PayUnpaidBill();
            }
            else
                printf("There no order thats Unpaid\n");
        }
        else if (choice == '3')
        {
            isLoop = false;
        }
        else if (choice == '4')
        {
            if (UnpaidBill() == true)
            {
                printf("You cannot end day when there is still unpaid bill\n");
            }
            else
                isLoop = true;
        }
    }
    return;
}

void bill_print(int count)
{
    printf("\033[34m");
    printf("Bill ID: %d%s\n", count + 1, Invoice.TsCode);
    printf("\033[36m");
    printf("%18s\t%19s\t%19s\t%18s\t%15s\t%23s\t\n", "PIN", "Food😋", "Price", "Quantity", "Unit",
           "Payment");
    printf("\033[m");
    for (int i = 0; i < Invoice.order[count].total; i++)
    {
        printf("%18d\t%19s\t%19.0f\t%15d\t%23s\t%23.0f\t\n",
               Invoice.order[count].items[i].dish.PIN,
               Invoice.order[count].items[i].dish.Name,
               Invoice.order[count].items[i].dish.Price,
               Invoice.order[count].items[i].quantity,
               Invoice.order[count].items[i].dish.Unit,
               Invoice.order[count].items[i].cash);
    }
    printf("\033[1;32m");
    printf("%112s%23.0f\n", "Total:", Invoice.order[count].sum);
    printf("%112s%23.0f\n", "Sale:", Invoice.order[count].sale);
    printf("%112s%23.0f\n", "Payment:", Invoice.order[count].pay);
    printf("\033[m");
}

void bill_print_file(FILE *file, int count)
{
    fprintf(file, "Bill ID: %d%s\n", count + 1, Invoice.TsCode);
    fprintf(file, "%18s\t\t%19s\t%19s\t%18s\t%15s\t%15s\t\n", "PIN", "Food😋", "Price", "Quantity", "Unit",
            "Payment");
    for (int i = 0; i < Invoice.order[count].total; i++)
    {
        fprintf(file, "%18d\t\t%19s\t%19.0f\t%15d\t%19s\t%15.0f\t\n",
                Invoice.order[count].items[i].dish.PIN,
                Invoice.order[count].items[i].dish.Name,
                Invoice.order[count].items[i].dish.Price,
                Invoice.order[count].items[i].quantity,
                Invoice.order[count].items[i].dish.Unit,
                Invoice.order[count].items[i].cash);
    }
    fprintf(file, "%101s%15.0f\n", "Total:", Invoice.order[count].sum);
    fprintf(file, "%101s%15.0f\n", "Sale:", Invoice.order[count].sale);
    fprintf(file, "%101s%15.0f\n", "Payment:", Invoice.order[count].pay);
}
bool isExistDate()
{
    char filePath[512];
    strcpy(filePath, BASE_DATA_PATH);
    strcat(filePath, INVOICES_PATH);
    strcat(filePath, "index.txt");
    FILE *index = fopen(filePath, "r");
    if (index == NULL)
    {
        printf("Cannot open the file\n");
        fclose(index);
    }
    char line[15];
    while (fgets(line, 15, index))
    {
        char dateWithTXT[15];
        sprintf(dateWithTXT, "%s.txt", Date);
        if (strcmp(line, dateWithTXT) == 0)
        {
            fclose(index);
            return true;
        }
    }
    return false;
}
