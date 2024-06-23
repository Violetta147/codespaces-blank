#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#define ENTER 10 // Correct value for the Enter key in ncurses
#define TAB 9
#define BCKSPC 127 // Correct value for Backspace key in ncurses

struct user {
    char fullName[50];
    char email[50];
    char username[50];
    char password[50];
    char phone[50];
};

void takeinput(char ch[50]) {
    fgets(ch, 50, stdin);
    size_t len = strlen(ch);
    if (len > 0 && ch[len - 1] == '\n') {
        ch[len - 1] = '\0'; // Remove newline character if it exists
    }
}

void generateUsername(char email[50], char username[50]) {
    int i;
    for (i = 0; i < strlen(email); i++) {
        if (email[i] == '@') break;
        else username[i] = email[i];
    }
    username[i] = '\0';
}

void takepassword(char pwd[50]) {
    int i = 0;
    char ch;
    initscr(); // Initialize ncurses mode
    noecho(); // Disable echoing of characters
    keypad(stdscr, TRUE); // Enable special keys to be captured
    while (1) {
        ch = getch();
        if (ch == ENTER || ch == TAB) {
            pwd[i] = '\0';
            break;
        } else if (ch == BCKSPC) {
            if (i > 0) {
                i--;
                printw("\b \b");
                refresh();
            }
        } else if (i < 49) { // Ensure we don't overflow the array
            pwd[i++] = ch;
            printw("*");
            refresh(); 
        }
    }
    echo(); // Re-enable echoing of characters
    endwin(); // End ncurses mode
}

int main() {
    printf("\033[1;94m");
    int opt;
    struct user user;
    printf("\n\t\t\t\t---------------WELCOME TO LOGIN PAGE---------------\n\n");
    printf("\nPlease choose an option:\n");
    printf("1. Signup\n");
    printf("2. Login\n");
    printf("3. Exit\n");

    printf("\nEnter your choice:\t");
    if (scanf("%d", &opt) != 1) {
        fprintf(stderr, "Invalid input\n");
        exit(1);
    }
    fgetc(stdin); // to clear the newline

    switch (opt) {
        case 1:
            printf("Enter your full name:\t");
            takeinput(user.fullName);
            printf("Enter your email:\t");
            takeinput(user.email);
            printf("Enter your contact no:\t");
            takeinput(user.phone);
            printf("Enter your password:\t");
            takepassword(user.password);

            // Generate username from email
            generateUsername(user.email, user.username);

            // Save user data to a file (for demonstration purposes)
            FILE *file = fopen("userdata.txt", "a");
            if (file == NULL) {
                fprintf(stderr, "Could not open userdata.txt for writing\n");
                return 1;
            }
            fprintf(file, "Full Name: %s\n", user.fullName);
            fprintf(file, "Email: %s\n", user.email);
            fprintf(file, "Username: %s\n", user.username);
            fprintf(file, "Phone: %s\n", user.phone);
            fprintf(file, "Password: %s\n", user.password);
            fclose(file);

            printf("\nSignup successful!\n");
            break;
        case 2:
            // Implement login functionality here
            printf("\nLogin functionality not implemented yet.\n");
            break;
        case 3:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid option. Please try again.\n");
    }

    return 0;
}
