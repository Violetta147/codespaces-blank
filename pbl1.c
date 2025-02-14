#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#define MAX_ORDERS 20
#define MAX 100 // i made change here
#define SALE 2000000
#define MAX_QTY 10
#define MAX_PATH_LENGTH 512
#define MAX_ORDER_ITEMS 5
#define MAX_STRING_LENGTH 1000
#define BASE_DATA_PATH "./data/"
#define INVOICES_PATH "Invoice/"
#define REVENUE_PATH "Revenue/"
#define FORMATTED_READ_FIELDS "%5d\t\t%[^\t]\t\t%10f\t\t%7d\t\t%[^\t]\t\n"
#define FORMATTED_READ_BILLS "%18d\t\t%[^\t]\t%19f\t%15d\t%[^\t]\t%15f\n"

char PASS[MAX] = "23TDT1";
char yesyes[4] = "yes";
char x[1] = "X";

char Date[11];

int slot;
int tempo;

typedef struct Dishes
{
    int FID;
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
    char state[20];
} Bills;

Bills Invoice;

typedef struct stat
{
    char Name[MAX];
    int quantity;
    float profit;
} Stat;

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

bool isYes(char *c);

bool isNo(char *c);

bool choose_date(char *Date1, char *Date2);

void Statistics();

char mode, move, ans;

void clstd();

void yawm();

void conf();

void readMenu(const char *filename);

void enter();

void Ans();

void arg();

void wandp();

void display();

void printMenuCustomer();

void printMenuAdmin();

bool iseq(char Name[]);

bool isex(int FID);

void erase();

void delete(int array[], int count);

void reset();

bool isrep(int FID);

bool isExistDate();

void extend();

bool input(const char *question, char str[]);

void custom();

void Customize(int FID);

void writeMenu(const char *filename);

void inputUnit();

void deunit(char Name[], int array[], int *count);

bool UnpaidBill();

void PayUnpaidBill();

void rewriteorder();

void Customer();

void bill_print(int count);

void bill_print_file(FILE *file, int count);

bool day_recap(Stat Stats[], int *count);

void print_stat(Stat Stats[], int count);

void fprint_stat(Stat Stats[], int count, FILE *file);

void sort_recap(Stat Stats[], int count);

void change_order_item(int ordi);

void delete_order_item(int ordi);

void show_bill();

void stat();
void drawBar(int maxBarHeight, int barWidth, float payDays[], int start, int end, char listFiles[][MAX]);
int countDigits(int n);

int main(void)
{
    stat();
    while (true)
    {
        printf("Input X to exit the program\n");
        enter();
        if (toupper(mode) == 'A')
        {
            printf("To access to Administrator mode please enter the password\n");
            conf();
            readMenu("Food.txt");
            do
            {
                printMenuAdmin();
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
                if (toupper(move) == 'S')
                {
                    Statistics();
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
    printf("Change applied\n");
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
        printf("Unable to read file.");
        exit(-1);
    }
    char hole[256];
    fscanf(menuP, "%[^\n]\n", hole);
    menu.total = 0;
    while (true)
    {
        int check = fscanf(menuP, FORMATTED_READ_FIELDS, &menu.dishes[menu.total].FID,
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
    for (int i = 0; i < 5; i++)
    {
        printf("%9sPlease enter your choice: ", "");
        scanf("%c", &move);
        clstd();
        if (toupper(move) == 'A' || toupper(move) == 'C' || toupper(move) == 'D' ||
            toupper(move) == 'S')
        {
            break;
        }
        else if (i == 4 || toupper(move) == 'X')
        {
            printf("%9sNo change have been made\n", "");
            yawm();
            exit(4);
        }
        else
            printf("%9sInvalid command\n", "");
    }
}

void display()
{
    for (int i = 0; i < menu.total; i++)
    {
        if (menu.dishes[i].Status == 1)
        {
            char hole[5];
            sprintf(hole, "%d", menu.dishes[i].FID);
            int q = ((11 - strlen(hole)) / 2) + strlen(hole) + (strlen(hole) + 1) % 2;
            printf("%11s%*d", "│", q, menu.dishes[i].FID);
            int F = ((11 - strlen(hole)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("│");
            q = ((24 - strlen(menu.dishes[i].Name)) / 2) + strlen(menu.dishes[i].Name) +
                (strlen(menu.dishes[i].Name)) % 2;
            printf("%*s", q, menu.dishes[i].Name);
            F = ((24 - strlen(menu.dishes[i].Name)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("│");
            char price[10];
            sprintf(price, "%.0f", menu.dishes[i].Price);
            q = ((14 - strlen(price)) / 2) + strlen(price) + (strlen(price) + 1) % 2;
            printf("%*s", q, price);
            F = ((14 - strlen(price)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("│");
            q = ((14 - strlen(menu.dishes[i].Unit)) / 2) + strlen(menu.dishes[i].Unit) +
                (strlen(menu.dishes[i].Unit)) % 2;
            printf("%*s", q, menu.dishes[i].Unit);
            F = ((14 - strlen(menu.dishes[i].Unit)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("│");
            printf("\n");
        }
        else if (toupper(mode) == 'A')
        {
            printf("\033[31m");
            char hole[5];
            sprintf(hole, "%d", menu.dishes[i].FID);
            int q = ((11 - strlen(hole)) / 2) + strlen(hole) + (strlen(hole) + 1) % 2;
            printf("%11s%*d", "│", q, menu.dishes[i].FID);
            int F = ((11 - strlen(hole)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("│");
            q = ((24 - strlen(menu.dishes[i].Name)) / 2) + strlen(menu.dishes[i].Name) +
                (strlen(menu.dishes[i].Name)) % 2;
            printf("%*s", q, menu.dishes[i].Name);
            F = ((24 - strlen(menu.dishes[i].Name)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("│");
            char price[10];
            sprintf(price, "%.0f", menu.dishes[i].Price);
            q = ((14 - strlen(price)) / 2) + strlen(price) + (strlen(price) + 1) % 2;
            printf("%*s", q, price);
            F = ((14 - strlen(price)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("│");
            q = ((14 - strlen(menu.dishes[i].Unit)) / 2) + strlen(menu.dishes[i].Unit) +
                (strlen(menu.dishes[i].Unit)) % 2;
            printf("%*s", q, menu.dishes[i].Unit);
            F = ((14 - strlen(menu.dishes[i].Unit)) / 2);
            for (; F > 0; F--)
                printf(" ");
            printf("│");
            printf("\n");
            printf("\033[0m");
        }
    }
    return;
}

void printMenuCustomer()
{
    printf("\nWelcome to Teddy Restaurant!\n");
    printf("                                ───▄▀▀▀▄▄▄▄▄▄▄▀▀▀▄───\n");
    printf("                                ───█▒▒░░░░░░░░░▒▒█───\n");
    printf("                                ────█░░█░░░░░█░░█────\n");
    printf("                                ─▄▄──█░░░▀█▀░░░█──▄▄─\n");
    printf("                                █░░█─▀▄░░░░░░░▄▀─█░░█\n");
    printf("                                █▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█\n");
    printf("                                █░░╦─╦╔╗╦─╔╗╔╗╔╦╗╔╗░░█\n");
    printf("                                █░░║║║╠─║─║─║║║║║╠─░░█\n");
    printf("                                █░░╚╩╝╚╝╚╝╚╝╚╝╩─╩╚╝░░█\n");
    printf("                                █▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█\n");
    printf("%11s", "┌");
    for (int i = 0; i < 11; i++)
    {
        printf("─");
    }
    printf("┬");
    for (int i = 0; i < 22; i++)
    {
        printf("─");
    }
    printf("┬");
    for (int i = 0; i < 13; i++)
    {
        printf("─");
    }
    printf("┬");
    for (int i = 0; i < 14; i++)
    {
        printf("─");
    }
    printf("┐");
    printf("\n        │    FID📌  │        Dishes        │    Price    │     Unit     │\n");
    printf("%11s", "├");
    for (int i = 0; i < 11; i++)
    {
        printf("─");
    }
    printf("┼");
    for (int i = 0; i < 22; i++)
    {
        printf("─");
    }
    printf("┼");
    for (int i = 0; i < 13; i++)
    {
        printf("─");
    }
    printf("┼");
    for (int i = 0; i < 14; i++)
    {
        printf("─");
    }
    printf("┤\n");
    display();
    printf("%11s", "└");
    for (int i = 0; i < 11; i++)
    {
        printf("─");
    }
    printf("┴");
    for (int i = 0; i < 22; i++)
    {
        printf("─");
    }
    printf("┴");
    for (int i = 0; i < 13; i++)
    {
        printf("─");
    }
    printf("┴");
    for (int i = 0; i < 14; i++)
    {
        printf("─");
    }
    printf("┘\n");
}

void arg()
{
    do
    {
        printf("Are you sure to save change?(Y/N) : ");
        scanf("%c", &ans);
        clstd();
    } while (toupper(ans) != 'N' && toupper(ans) != 'Y' && toupper(ans) != 'X');
    return;
}

void erase()
{
    int count = 1;
    int del[count];
    printf("Enter the Food FID you wanna eradicate: ");
    while (scanf("%d", &del[0]) != 1 || isex(del[0]) == false)
    {
        clstd();
        printf("Invalid FID!\n");
        printf("Enter the Food FID you wanna eradicate: ");
    }
    clstd();
    arg();
    if (toupper(ans) == 'N')
    {
        printf("No change was applied!\n");
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
        printf("Exiting program!\n");
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
            if (menu.dishes[j].FID == array[i])
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
    fprintf(menuP, "%5s", "FID");
    fprintf(menuP, "\t\t%-*s", longestName + (nameTabs - 1) * 4, "Name📛");
    fprintf(menuP, "\t\t%-*s", longestPriceDigits + (priceTabs - 1) * 4, "Price");
    fprintf(menuP, "\t\t%-*s", longestPriceDigits + (priceTabs - 1) * 4, "Status");
    fprintf(menuP, "\t\t%s\t\n", "Unit");

    // Data
    for (int i = 0; i < menu.total; i++)
    {
        fprintf(menuP, "%5d", menu.dishes[i].FID);
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
        array[i] = menu.dishes[i].FID;
        count++;
    }
    delete (array, count);
    return;
}

void extend()
{
    float FPrice;
    int FPIN, Stat;
    char FName[MAX], Unit[10];
    // clstd();
    while (input("Give the new Food name", FName) == false)
        printf("Invalid food name!\n");
    while (iseq(FName) == true)
    {
        printf("Your food name is already been used, please use another name in order not to "
               "confused anyone!\n");
        while (input("Change the Food name", FName) == false)
            printf("Invalid food name!\n");
    }
    printf("Give the price: ");
    while (scanf("%f", &FPrice) != 1)
    {
        clstd();
        printf("Please input a valid price!\n");
        printf("Give the price: ");
    }
    printf("Give the Food FID: ");
    while (scanf("%d", &FPIN) != 1 || isrep(FPIN) == true || FPIN == 0)
    {
        clstd();
        printf("Your FID maybe invalid or already used, please try another one!\n");
        printf("Give the Food FID: ");
    }
    // clstd();
    printf("Give the Food Status:(1 for Available and 0 for the opposite) ");
    while (scanf("%d", &Stat) != 1 || (Stat != 0 && Stat != 1))
    {
        clstd();
        printf("Invalid status value!\n");
        printf("Give the Food status: ");
    }
    clstd();
    while (input("Give the Food unit", Unit) == false)
        printf("Invalid food unit!\n");
    arg();
    if (toupper(ans) == 'N')
    {
        printf("No change was applied!");
        yawm();
        printMenuAdmin();
    }
    else if (toupper(ans) == 'Y')
    {

        menu.dishes[menu.total].FID = FPIN;
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
        printf("Exiting program!\n");
        yawm();
        exit(1);
    }
}

bool isrep(int FID)
{
    for (int i = 0; i < menu.total; i++)
    {
        if (menu.dishes[i].FID == FID)
        {
            slot = i;
            return true;
        }
    }
    return false;
}

bool isex(int FID)
{
    for (int i = 0; i < menu.total; i++)
    {
        if (menu.dishes[i].FID == FID)
            return true;
    }
    printf("No food match this FID!\n");
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
    printf("Give me the FID of the food u wanna customize: ");
    while (scanf("%d", &FID) != 1 || isrep(FID) == false)
    {
        clstd();
        printf("Sorry but no FID was found matched your input!\n");
        printf("Give me the FID of the food u wanna customize: ");
    }
    if (FID != 0)
        Customize(FID);
    else
        return;
}
void Customize(int FID)
{
    Dish array;
    bool Yn = true;
    int Pivot = 0;
    for (int i = 0; i < menu.total; i++)
    {
        if (menu.dishes[i].FID == FID)
        {
            Pivot = i;
            break;
        }
    }
    array = menu.dishes[Pivot];
    printf("\033[32m"); // green
    printf("Current food name: %s\n", menu.dishes[Pivot].Name);
    printf("\033[1;36m"); // Cyan
    printf("Current Price: %.0f\n", menu.dishes[Pivot].Price);
    printf("\033[1;34m"); // Blue
    printf("Current Status: %d\n", menu.dishes[Pivot].Status);
    printf("\033[1;37m");
    printf("Current Unit: %s\n", menu.dishes[Pivot].Unit);
    printf("\033[0m");
    while (Yn)
    {
        printf("Select the filed you want to change:\n");
        printf("1. Name\n");
        printf("2. Price\n");
        printf("3. Status\n");
        printf("4. Unit\n");
        printf("Your choice: ");
        int choice;
        while (scanf("%d", &choice) != 1 || choice < 1 || choice > 5)
        {
            clstd();
            printf("Invalid choice!\n");
            printf("Your choice: ");
        }
        clstd();
        switch (choice)
        {
        case 1:
            while (input("Give food new name", array.Name) == false || iseq(array.Name) == true)
            {
                if (iseq(array.Name) == true)
                    printf("This name is already used, please try another one!\n");
                else
                {
                    printf("Invalid food name!\n");
                    clstd();
                }
            }
            if (strcmp(array.Name, "0") == 0)
            {
                strcpy(array.Name, menu.dishes[Pivot].Name);
            }
            break;
        case 2:
            printf("Food new price: ");
            while (scanf("%f", &array.Price) != 1 || array.Price < 0)
            {
                clstd();
                printf("Invalid price!\n");
                printf("Food new price: ");
            }
            if (array.Price == 0)
            {
                array.Price = menu.dishes[Pivot].Price;
            }
            clstd();
            break;
        case 3:
            Yn = YN("Do you want to switch status?");
            if (Yn)
            {
                array.Status = (array.Status + 1) % 2;
            }
            else
            {
                // array.Status = menu.dishes[Pivot].Status;
                break;
            }
            break;
        case 4:
            while (input("Food new unit", array.Unit) == false)
            {
                printf("Invalid food unit!\n");
            }
            if (strcmp(array.Unit, "0") == 0)
            {
                strcpy(array.Unit, menu.dishes[Pivot].Unit);
            }
            break;
        default:
            break; // not necessary
        }
        Yn = YN("Do you want to change another field?");
    }
    Yn = YN("Do you want to save the change?");
    if (Yn == true)
    {
        menu.dishes[Pivot] = array;
    }
    else if (Yn == false)
    {
        printf("No change was applied!\n");
        yawm();
        printMenuAdmin();
    }
    else
    {
        printf("Exiting program!\n");
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
    FILE *indexFile = fopen(filePath, "w");
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
    char buffer[200];
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
        int k = 0;
        do
        {
            k = fscanf(
                file, FORMATTED_READ_BILLS,
                &Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].dish.FID,
                Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].dish.Name,
                &Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].dish.Price,
                &Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].quantity,
                Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].dish.Unit,
                &Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].cash);
            if (k == 6)
                Invoice.order[Invoice.count].total++;
        } while (k == 6);
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%*100s%16f", &Invoice.order[Invoice.count].sum);
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%*100s%16f", &Invoice.order[Invoice.count].sale);
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%*100s%16f", &Invoice.order[Invoice.count].pay);
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%*100s%16s", Invoice.order[Invoice.count].status);
        tempo = Invoice.order[Invoice.count].total;
        if (strcmp(Invoice.order[Invoice.count].status, "Paid") == 0)
        {
            Invoice.gain += Invoice.order[Invoice.count].pay;
        }
        Invoice.count++;
    }
    return;
}

bool YN(const char *ques)
{
    char temp[MAX];
    while (true)
    {
        printf("%s [Y/N]: ", ques);
        scanf("%s", temp);
        clstd();
        if (isYes(temp))
            return true;
        if (isNo(temp))
            return false;
    }
    return true;
}

bool isYes(char *c)
{
    for (int i = 0; c[i] != '\0'; i++)
    {
        c[i] = tolower(c[i]);
    }
    return (strcmp(c, yesyes) == 0 || strcmp(c, "y") == 0);
}

bool isNo(char *c)
{
    for (int i = 0; c[i] != '\0'; i++)
    {
        c[i] = tolower(c[i]);
    }
    return (strcmp(c, "no") == 0 || strcmp(c, "n") == 0);
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
    int FPIN = 0;
    int i;
    int flag = 0;
    int quantity;
    while (Invoice.order[ordi].total < MAX_ORDERS)
    {
        do
        {
            printf("Type in the FID of the food you want to order: ");
            while (scanf("%d", &FPIN) != 1)
            {
                clstd();
                printf("Invalid FID, please reinput: ");
            }
            clstd();
            if (FPIN == 0)
            {
                printf("You have just cancelling ordering!\n");
                if (Invoice.order[ordi].total != 0)
                {
                    printf("Calculating your order...!\n");
                    bill_calc(ordi);
                    return true;
                }
                else if (Invoice.order[ordi].total == 0)
                {
                    printf("You have not order anything!\n");
                    return false;
                }
            }
            if (isrep(FPIN) == true)
            {
                for (i = 0; i < Invoice.order[ordi].total; i++)
                {
                    if (Invoice.order[ordi].items[i].dish.FID == FPIN)
                    {
                        flag = 1;
                        break;
                    }
                }
                printf("How many of this food do you want? ");
                while (scanf("%d", &quantity) != 1 || quantity > 9999 || quantity < 1)
                {
                    clstd();
                    printf("Invalid quantity, please rechoose: ");
                }
                clstd();
                if (flag == 0)
                {
                    Invoice.order[ordi].items[Invoice.order[ordi].total].dish = menu.dishes[slot];
                    Invoice.order[ordi].items[Invoice.order[ordi].total].quantity = quantity;
                    Invoice.order[ordi].items[Invoice.order[ordi].total].cash =
                        (float)(Invoice.order[ordi].items[Invoice.order[ordi].total].dish.Price *
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
                printf("You have just ordered %s with a quantity of %d\n", Invoice.order[ordi].items[i].dish.Name, quantity);
            }
            else
                printf("No food match this FID!\n");
        } while (isrep(FPIN) == false);
    }
    if (Invoice.order[ordi].total == MAX_ORDERS)
    {
        printf("You have reached your order limits, we are calculating your order!\n");
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
    char num[100];
    sprintf(num, "%d", ordi + 1);
    strcat(num, Invoice.TsCode);
    strcpy(Invoice.order[ordi].BID, num);
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
    Invoice.order[ordi].pay = floor(Invoice.order[ordi].sum - Invoice.order[ordi].sale);
    printf("────────────────────────────────────────────────────────────────────────────────────────────────"
           "───────────────────────────────────────\n");
    bill_print(ordi);
    bill_print_file(invoice, ordi);
    printf("────────────────────────────────────────────────────────────────────────────────────────────────"
           "───────────────────────────────────────\n");
    if (YN("Do you want to purchase your order now?") == false)
    {
        printf("Your order have been saved!\n");
        strcpy(Invoice.order[ordi].status, "Unpaid");
        fprintf(invoice, "%100s%16s\n", "Status:", Invoice.order[ordi].status);
    }
    else
    {
        purchase(ordi);
        fprintf(invoice, "%100s%16s\n", "Status:", Invoice.order[ordi].status);
        printf("Here is your Bills to confirm you paid it\n");
        printf("────────────────────────────────────────────────────────────────────────────────────────────────"
               "───────────────────────────────────────\n");
        bill_print(ordi);
        printf("%112s%23s\n", "Status:", Invoice.order[ordi].status);
        printf("────────────────────────────────────────────────────────────────────────────────────────────────"
               "───────────────────────────────────────\n");
        printf("Thank you for using our service, wish you a nice meal\n");
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
        printf("Your total amount is %.0fvnd, you give(in vnd): ", Invoice.order[ordi].pay);
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
        printf("%d. Bill %s cost %.0fvnd\n", i + 1, Invoice.order[point[i]].BID,
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
        printf("Here your current bill!\n");
        bill_print(point[choose - 1]);
        Invoice.order[point[choose - 1]].pay = 0;
        Invoice.order[point[choose - 1]].sum = 0;
        Invoice.order[point[choose - 1]].sale = 0;
        order(point[choose - 1]);
    }
    else
    {
        purchase(point[choose - 1]);
        printf("Here is your Bills to confirm you paid it\n");
        printf("────────────────────────────────────────────────────────────────────────────────────────────────"
               "───────────────────────────────────────\n");
        bill_print(point[choose - 1]);
        printf("%112s%23s\n", "Status:", Invoice.order[point[choose - 1]].status);
        printf("────────────────────────────────────────────────────────────────────────────────────────────────"
               "───────────────────────────────────────\n");
        printf("Thank you for using our service, wish you a nice meal!\n");
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
    fprintf(invoice, "State: %s\n", Invoice.state);
    fprintf(invoice, "TS code: %s\n", Invoice.TsCode);
    for (int i = 0; i < Invoice.count; i++)
    {
        bill_print_file(invoice, i);
        fprintf(invoice, "%100s%16s\n", "Status:", Invoice.order[i].status);
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
        sscanf(buffer, "State: %19s\n", Invoice.state);
        fgets(buffer, sizeof(buffer), invoice);
        sscanf(buffer, "TS code: %19s\n", Invoice.TsCode);
        printf("%s\n", Invoice.TsCode);
        daily_bills(invoice);
        printf("%d\n", Invoice.count);
        fclose(invoice);
    }
    else
    {
        Invoice.TsCode[0] = '\0';
        hash(Date);
        fprintf(invoice, "State: %s\n", "Active");
        strcpy(Invoice.state, "Active");
        fprintf(invoice, "TS code: %s\n", Invoice.TsCode);
        fclose(invoice);
    }
    bool isLoop = true;
    if (strcasecmp(Invoice.state, "Ended") == 0)
    {
        printf("This day is no longer support!\n");
        isLoop = false;
    }
    while (isLoop)
    {
        char choice;
        printf("1.Order and pay\n");
        printf("2.Unpaid bills\n");
        printf("3.Show bills\n");
        printf("4.Exit\n");
        printf("5.Exit and end day\n");
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
            show_bill();
        }
        else if (choice == '4')
        {
            isLoop = false;
        }
        else if (choice == '5')
        {
            if (UnpaidBill() == true)
            {
                printf("You cannot end day when there is still unpaid bill\n");
            }
            else
            {
                Stat Stats[100] = {0};
                int count = 0;
                if (day_recap(Stats, &count) == true)
                {
                    sort_recap(Stats, count);
                    printf("Here is the rank of food selling today:\n");
                    print_stat(Stats, count);
                    printf("Total gain of today is: %.0fvnd\n", Invoice.gain);
                }
                else
                    printf("No order today!\n");
                strcpy(filePath, BASE_DATA_PATH);
                strcat(filePath, REVENUE_PATH);
                strcat(filePath, filename);
                FILE *revenue = fopen(filePath, "w");
                if (revenue == NULL)
                {
                    printf("Cannot open the file\n");
                    fclose(revenue);
                }
                fprint_stat(Stats, count, revenue);
                fprintf(revenue, "Total gain of today is: %.0fvnd\n", Invoice.gain);
                fclose(revenue);
                strcpy(Invoice.state, "Ended");
                rewriteorder();
                printf("Day ending.....\n");
                isLoop = false;
            }
        }
    }
    memset(&Invoice, 0, sizeof(Invoice));
    return;
}

void bill_print(int count)
{
    printf("\033[34m");
    printf("Bill ID: %s\n", Invoice.order[count].BID);
    printf("\033[36m");
    printf("%18s\t%19s\t%19s\t%18s\t%15s\t%23s\n", "FID", "Food😋", "Price", "Quantity", "Unit",
           "Payment");
    printf("\033[m");
    for (int i = 0; i < Invoice.order[count].total; i++)
    {
        printf("%18d\t%19s\t%19.0f\t%15d\t%23s\t%23.0f\n", Invoice.order[count].items[i].dish.FID,
               Invoice.order[count].items[i].dish.Name, Invoice.order[count].items[i].dish.Price,
               Invoice.order[count].items[i].quantity, Invoice.order[count].items[i].dish.Unit,
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
    fprintf(file, "Bill ID: %s\n", Invoice.order[count].BID);
    fprintf(file, "%18s\t\t%19s\t%19s\t%18s\t%15s\t%15s\n", "FID", "Food😋", "Price", "Quantity",
            "Unit", "Payment");
    for (int i = 0; i < Invoice.order[count].total; i++)
    {
        fprintf(file, "%18d\t\t%19s\t%19.0f\t%15d\t%19s\t%15.0f\n",
                Invoice.order[count].items[i].dish.FID, Invoice.order[count].items[i].dish.Name,
                Invoice.order[count].items[i].dish.Price, Invoice.order[count].items[i].quantity,
                Invoice.order[count].items[i].dish.Unit, Invoice.order[count].items[i].cash);
    }
    fprintf(file, "%100s%16.0f\n", "Total:", Invoice.order[count].sum);
    fprintf(file, "%100s%16.0f\n", "Sale:", Invoice.order[count].sale);
    fprintf(file, "%100s%16.0f\n", "Payment:", Invoice.order[count].pay);
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

bool day_recap(Stat Stats[], int *count)
{
    int hint = 0;
    for (int i = 0; i < Invoice.count; i++)
    {
        for (int j = 0; j < Invoice.order[i].total; j++)
        {
            for (int k = 0; k < *count + 1; k++)
            {
                if (strcasecmp(Invoice.order[i].items[j].dish.Name, Stats[k].Name) == 0)
                {
                    Stats[k].quantity += Invoice.order[i].items[j].quantity;
                    Stats[k].profit += Invoice.order[i].items[j].cash;
                    hint = 1;
                    break;
                }
            }
            if (hint == 0)
            {
                strcpy(Stats[*count].Name, Invoice.order[i].items[j].dish.Name);
                Stats[*count].quantity += Invoice.order[i].items[j].quantity;
                Stats[*count].profit += Invoice.order[i].items[j].cash;
                *count = *count + 1;
            }
            hint = 0;
        }
    }
    if (*count > 0)
    {
        return true;
    }
    else
        return false;
}

void sort_recap(Stat Stats[], int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (Stats[i].quantity < Stats[j].quantity)
            {
                Stat tmp = Stats[i];
                Stats[i] = Stats[j];
                Stats[j] = tmp;
            }
        }
    }
}

void print_stat(Stat Stats[], int count)
{
    int sign = 0;
    int i = 0;
    printf("┌──────┬────────────────────┬────────────────┬──────────────┐\n");
    printf("│ Rank │        Name        │    Quantity    │    Total     │\n");
    printf("├──────┼────────────────────┼────────────────┼──────────────┤\n");
    for (; i < count;)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (Stats[i].quantity != Stats[j].quantity)
            {
                sign = j;
                break;
            }
            sign = count;
        }
        for (int k = 0; k < sign - i; k++)
        {
            printf("│ %-4d │ %-20s │ %14d │ %13.0f│\n", i + 1, Stats[k + i].Name,
                   Stats[k + i].quantity, Stats[k + i].profit);
        }
        if (sign == count - 1)
            i = count + 1;
        else
            i = sign;
    }
    if (sign == count - 1)
    {
        printf("│ %-4d │ %-20s │ %14d │ %13.0f│\n", count, Stats[count - 1].Name,
               Stats[count - 1].quantity, Stats[count - 1].profit);
    }
    printf("└──────┴────────────────────┴────────────────┴──────────────┘\n");
}

void fprint_stat(Stat Stats[], int count, FILE *file)
{
    int sign = 0;
    int i = 0;
    fprintf(file, "Day TS code: %s\n", Invoice.TsCode);
    fprintf(file, "+-----------------------------------------------------------+\n");
    fprintf(file, "| Rank |        Name        |    Quantity    |    Total     |\n");
    fprintf(file, "+-----------------------------------------------------------+\n");
    for (; i < count;)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (Stats[i].quantity != Stats[j].quantity)
            {
                sign = j;
                break;
            }
            sign = count;
        }
        for (int k = 0; k < sign - i; k++)
        {
            fprintf(file, "| %-4d | %-20s | %14d | %13.0f|\n", i + 1, Stats[k + i].Name,
                    Stats[k + i].quantity, Stats[k + i].profit);
        }
        if (sign == count - 1)
            i = count + 1;
        else
            i = sign;
    }
    if (sign == count - 1)
    {
        fprintf(file, "| %-4d | %-20s | %14d | %13.0f|\n", count, Stats[count - 1].Name,
                Stats[count - 1].quantity, Stats[count - 1].profit);
    }
    fprintf(file, "+-----------------------------------------------------------+\n");
}

void printMenuAdmin()
{

    printf("\nWelcome to Teddy Restaurant!\n");
    printf("                                ───▄▀▀▀▄▄▄▄▄▄▄▀▀▀▄───\n");
    printf("                                ───█▒▒░░░░░░░░░▒▒█───\n");
    printf("                                ────█░░█░░░░░█░░█────\n");
    printf("                                ─▄▄──█░░░▀█▀░░░█──▄▄─\n");
    printf("                                █░░█─▀▄░░░░░░░▄▀─█░░█\n");
    printf("                                █▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█\n");
    printf("                                █░░╦─╦╔╗╦─╔╗╔╗╔╦╗╔╗░░█\n");
    printf("                                █░░║║║╠─║─║─║║║║║╠─░░█\n");
    printf("                                █░░╚╩╝╚╝╚╝╚╝╚╝╩─╩╚╝░░█\n");
    printf("                                █▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█\n");
    printf("%11s", "┌");
    for (int i = 0; i < 11; i++)
    {
        printf("─");
    }
    printf("┬");
    for (int i = 0; i < 22; i++)
    {
        printf("─");
    }
    printf("┬");
    for (int i = 0; i < 13; i++)
    {
        printf("─");
    }
    printf("┬");
    for (int i = 0; i < 14; i++)
    {
        printf("─");
    }
    printf("┐");
    printf("\n        │    FID📌  │        Dishes        │    Price    │     Unit     │\n");
    printf("%11s", "├");
    for (int i = 0; i < 11; i++)
    {
        printf("─");
    }
    printf("┼");
    for (int i = 0; i < 22; i++)
    {
        printf("─");
    }
    printf("┼");
    for (int i = 0; i < 13; i++)
    {
        printf("─");
    }
    printf("┼");
    for (int i = 0; i < 14; i++)
    {
        printf("─");
    }
    printf("┤\n");
    display();
    printf("%11s", "└");
    for (int i = 0; i < 11; i++)
    {
        printf("─");
    }
    printf("┴");
    for (int i = 0; i < 22; i++)
    {
        printf("─");
    }
    printf("┴");
    for (int i = 0; i < 13; i++)
    {
        printf("─");
    }
    printf("┴");
    for (int i = 0; i < 14; i++)
    {
        printf("─");
    }
    printf("┘\n");

    printf("%8s┌─────────────────────────────────────────┐\n", "");
    printf("%8s│1. ADD NEW FOOD(A)                       │\n", "");
    printf("%8s│2. CUSTOMIZE FOOD'S INFORMATION(C)       │\n", "");
    printf("%8s│3. DELETE FOOD(D)                        │\n", "");
    printf("%8s│4. STATISTIC(S)                          │\n", "");
    printf("%8s│5. EXIT(X)                               │\n", "");
    printf("%8s└─────────────────────────────────────────┘\n", "");
    printf("\n");
}

void Statistics()
{
    char Date1[11], Date2[11];
    while (choose_date(Date1, Date2) == false)
    {
        printf("Invalid range cannot statistic please choose again!\n");
    }
    int date1 = dateToInt(Date1);
    int date2 = dateToInt(Date2);
    char filePath[MAX_PATH_LENGTH];
    strcpy(filePath, BASE_DATA_PATH);
    strcat(filePath, INVOICES_PATH);
    char listFiles[MAX][MAX];
    int totalFileIndex = 0;
    getListOrders(listFiles, &totalFileIndex);
    int end = totalFileIndex;
    int start = end + 1;
    int dateInts[MAX];
    for (int i = 0; i < totalFileIndex; i++)
    {
        dateInts[i] = dateToInt(listFiles[i]);
    }
    for (int i = 0; i < totalFileIndex; i++)
    {
        if (date1 < dateInts[i])
        {
            start = i - 1;
            break;
        }
        else if (date1 == dateInts[i])
        {
            start = i;
            break;
        }
    }
    for (int i = start + 1; i < totalFileIndex; i++)
    {
        if (date2 < dateInts[i])
        {
            end = i - 1;
            break;
        }
        else if (date2 == dateInts[i])
        {
            end = i;
            break;
        }
    }
    Stat Stats[100] = {0};
    int count = 0;
    for (int i = start; i <= end; i++)
    {
        char filename[MAX_PATH_LENGTH];
        strcpy(filename, filePath);
        strcat(filename, listFiles[i]);
        FILE *invoice = fopen(filename, "a+");
        if (invoice == NULL)
        {
            continue;
        }
        rewind(invoice);
        char buffer[1000];
        if (fgets(buffer, sizeof(buffer), invoice) != NULL)
        {
            fgets(buffer, sizeof(buffer), invoice);
            daily_bills(invoice);
            day_recap(Stats, &count);
            memset(&Invoice, 0, sizeof(Invoice));
        }
    }
    if (count > 0)
    {
        sort_recap(Stats, count);
        printf("Here is the rank of food selling from %s to %s:\n", Date1, Date2);
        print_stat(Stats, count);
        memset(&Stats, 0, sizeof(Stats));
    }
    else
    {
        printf("No order to statistics\n");
    }
    bool Yn = YN("Do you want to go back to ADMIN PANEL?");
    if (Yn == true)
    {
        printf("\033[0m");
    }
    else
    {
        printf("Press any key to continue: ");
        getchar();
    }
}

void delete_order_item(int ordi)
{
    int FID;
    printf("Enter the FID of the item you want to delete: ");
    while (scanf("%d", &FID) != 1)
    {
        clstd();
        printf("Invalid FID, please reinput: ");
    }
    clstd();
    int i;
    for (i = 0; i < Invoice.order[ordi].total; i++)
    {
        if (Invoice.order[ordi].items[i].dish.FID == FID)
        {
            break;
        }
    }
    if (i == Invoice.order[ordi].total)
    {
        printf("No item with FID %d found in the order!\n", FID);
    }
    else
    {
        for (int j = i; j < Invoice.order[ordi].total - 1; j++)
        {
            Invoice.order[ordi].items[j] = Invoice.order[ordi].items[j + 1];
        }
        Invoice.order[ordi].total--;
        Invoice.order[ordi].sum = 0;
        for (i = 0; i < Invoice.order[ordi].total; i++)
        {
            Invoice.order[ordi].sum += Invoice.order[ordi].items[i].cash;
        }
        if (Invoice.order[ordi].sum >= 100000)
        {
            Invoice.order[ordi].sale = Invoice.order[ordi].sum * 25 / 100;
            Invoice.order[ordi].sale = ceil(Invoice.order[ordi].sale / 1000) * 1000;
        }
        Invoice.order[ordi].pay = floor(Invoice.order[ordi].sum - Invoice.order[ordi].sale);
    }
}

void change_order_item(int ordi)
{
    int FID;
    printf("Enter the FID of the item you want to change quantity: ");
    while (scanf("%d", &FID) != 1)
    {
        clstd();
        printf("Invalid FID, please reinput: ");
    }
    clstd();
    int i;
    for (i = 0; i < Invoice.order[ordi].total; i++)
    {
        if (Invoice.order[ordi].items[i].dish.FID == FID)
        {
            break;
        }
    }
    if (i == Invoice.order[ordi].total)
    {
        printf("No item with FID %d found in the order!\n", FID);
    }
    else
    {
        int quantity;
        printf("Enter the new quantity: ");
        while (scanf("%d", &quantity) != 1)
        {
            clstd();
            printf("Invalid quantity, please reinput: ");
        }
        clstd();
        Invoice.order[ordi].items[i].quantity = quantity;
        Invoice.order[ordi].items[i].cash = (float)(Invoice.order[ordi].items[i].dish.Price * quantity);
        // recalculate the total, sale, payment of the order
        Invoice.order[ordi].sum = 0;
        Invoice.order[ordi].sale = 0;
        for (i = 0; i < Invoice.order[ordi].total; i++)
        {
            Invoice.order[ordi].sum += Invoice.order[ordi].items[i].cash;
        }
        if (Invoice.order[ordi].sum >= 100000)
        {
            Invoice.order[ordi].sale = Invoice.order[ordi].sum * 25 / 100;
            Invoice.order[ordi].sale = ceil(Invoice.order[ordi].sale / 1000) * 1000;
        }
        Invoice.order[ordi].pay = floor(Invoice.order[ordi].sum - Invoice.order[ordi].sale);
    }
}

bool delete_change_order(int ordi)
{
    char c;
    do
    {
        printf("If you want to delete any dish(D) or change any dish quantity(Q) or confirm your order(C)?(D/Q/C): ");
        scanf("%c", &c);
        clstd();
    } while (toupper(c) != 'D' && toupper(c) != 'Q' && toupper(c) != 'C');
    if (toupper(c) == 'D')
    {
        delete_order_item(ordi);
        return false;
    }
    if (toupper(c) == 'Q')
    {
        change_order_item(ordi);
        return false;
    }
    return true;
}

void show_bill()
{
    int ordi;
    printf("You have receive a total of %d order today!\n", Invoice.count);
    printf("Type in the ordi number of your number(from 1 to %d): ", Invoice.count);
    scanf("%d", &ordi);
    while (ordi <= 0 || ordi > Invoice.count)
    {
        printf("Invalid bill ordi number\n");
        clstd();
    }
    clstd();
    printf("Here is the Bill you want to see:\n");
    bill_print(ordi - 1);
}
void stat()
{
    char Date1[11], Date2[11];
    while (choose_date(Date1, Date2) == false)
    {
        printf("Invalid range cannot statistic please choose again!\n");
    }
    int date1 = dateToInt(Date1);
    int date2 = dateToInt(Date2);
    char filePath[MAX_PATH_LENGTH];
    strcpy(filePath, BASE_DATA_PATH);
    strcat(filePath, INVOICES_PATH);
    char listFiles[MAX][MAX];
    int totalFileIndex = 0;
    getListOrders(listFiles, &totalFileIndex);
    int end = totalFileIndex;
    int start = end + 1;
    int dateInts[MAX];
    for (int i = 0; i < totalFileIndex; i++)
    {
        dateInts[i] = dateToInt(listFiles[i]);
    }
    for (int i = 0; i < totalFileIndex; i++)
    {
        if (date1 < dateInts[i])
        {
            start = i - 1;
            break;
        }
        else if (date1 == dateInts[i])
        {
            start = i;
            break;
        }
    }
    for (int i = start + 1; i < totalFileIndex; i++)
    {
        if (date2 < dateInts[i])
        {
            end = i - 1;
            break;
        }
        else if (date2 == dateInts[i])
        {
            end = i;
            break;
        }
    }
    // if day range is wrong
    if (start == end + 1)
    {
        return;
    }
    else if (start == end & start == - 1 & end == -1)
    {
        return;
    }
    float payDays[MAX] = {0};
    // Bills* invoiceBarChart;
    for (int i = start; i < end; i++)
    {
        char filename[MAX_PATH_LENGTH];
        strcpy(filename, filePath);
        strcat(filename, listFiles[i]);
        FILE *invoice = fopen(filename, "r");
        if (invoice == NULL)
        {
            continue;
        }
        char buffer[1000];
        Invoice.count = 0;
        Invoice.gain = 0;
        fscanf(invoice, "State: %s\n", Invoice.state);
        fscanf(invoice, "TS code: %s\n", Invoice.TsCode);

        while (fscanf(invoice, "Bill ID: %s\n", Invoice.order[Invoice.count].BID) == 1)
        {
            fgets(buffer, sizeof(buffer), invoice);
            while (fscanf(invoice, FORMATTED_READ_BILLS,
                          &Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].dish.FID,
                          Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].dish.Name,
                          &Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].dish.Price,
                          &Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].quantity,
                          Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].dish.Unit,
                          &Invoice.order[Invoice.count].items[Invoice.order[Invoice.count].total].cash) == 6)
            {
                Invoice.order[Invoice.count].total++;
            }
            fgets(buffer, sizeof(buffer), invoice);
            sscanf(buffer, "%*100s%16f", &Invoice.order[Invoice.count].sum);
            fgets(buffer, sizeof(buffer), invoice);
            sscanf(buffer, "%*100s%16f", &Invoice.order[Invoice.count].sale);
            fgets(buffer, sizeof(buffer), invoice);
            sscanf(buffer, "%*100s%16f", &Invoice.order[Invoice.count].pay);
            fgets(buffer, sizeof(buffer), invoice);
            sscanf(buffer, "%*100s%16s", Invoice.order[Invoice.count].status);
            Invoice.gain += Invoice.order[Invoice.count].pay;
            Invoice.count++;
        }
        fclose(invoice);
        payDays[i] = Invoice.gain;
        // invoiceBarChart = (Bills*)malloc(sizeof(Bills) * (end - start));
        // invoiceBarChart[i] = Invoice;
        memset(&Invoice, 0, sizeof(Invoice));
    }
    // print the statistic
    printf("\033[2J\033[H");
    printf("                                                          ┌────────────────────────────────────────────────────────────┐\n");

    printf("%*s%*s%s%s%s%s%*s\n", 61, "│", 10, "", "STATISTIC FROM ", Date1, " TO ", Date2, 14, "│");
    printf("                                                          └────────────────────────────────────────────────────────────┘\n");
    for (int i = 0; i < totalFileIndex; i++)
    {
        listFiles[i][strlen(listFiles[i]) - 4] = '\0';
    }
    for (int i = start; i < end; i++)
    {
        printf("The total gain of day %s is %20.0f VND\n", listFiles[i], payDays[i]);
    }
    printf("\n");
    printf("──────────────────────────────────────────────────────────────────────────────────────────");
    float total = 0;
    for (int i = start; i < end; i++)
    {
        total += payDays[i];
    }
    printf("\n\t\t\t\t\t\t\t\t\t  TOTAL GAIN  \n%80.0f VND\n", total);

    bool yN = YN("Do you want to see the statistic in form of bar chart?");
    if (yN == true)
    {
        printf("\033[2J\033[H");
        int maxBarHeight = 15;
        int barWidth = 5;
        printf("\n\n\n");
        drawBar(maxBarHeight, barWidth, payDays, start, end, listFiles);
        getchar();
    }
}
void drawBar(int maxBarHeight, int barWidth, float payDays[], int start, int end, char listFiles[][MAX])
{
    for (int i = 0; i < maxBarHeight; i++)
    {
        for (int j = start; j < end; j++)
        {
            if (payDays[j] / 120000 * maxBarHeight >= maxBarHeight - i)
            {
                printf("\t");
                for (int w = 0; w < barWidth; w++)
                {
                    printf("█");
                }
            }
            else
            {
                for (int w = 0; w < barWidth; w++)
                {
                    printf(" ");
                }
            }
            printf("         "); // Space between bars
        }
        printf("\n");
    }
    for (int i = start; i < end; i++)
    {
        printf("      %s", listFiles[i]);
    }
    printf("\n");
    for (int i = 0; i < maxBarHeight + 2; i++)
    {
        printf("│");
        printf("\033[4D");
        printf("\033[1A");
    }
    printf("\t");
    for (int j = start; j < end; j++)
    {
        float currentBarHeight = payDays[j] / 120000 * maxBarHeight;
        if((int)currentBarHeight - maxBarHeight == 0)
        {
            printf("%.0f", payDays[j]);
            printf("\033[%dD", countDigits((int)payDays[j]));
            printf("      ");
            continue;
        }
        printf("\033[%dB%.0f",maxBarHeight - (int)currentBarHeight, payDays[j]);
        printf("\033[%dD", countDigits((int)payDays[j]));
        printf("\033[%dA", maxBarHeight - (int)currentBarHeight);
        printf("      ");
    }
}
int countDigits(int n)
{
    int count = 0;
    while (n != 0)
    {
        n /= 10;
        count++;
    }
    return count;
}
