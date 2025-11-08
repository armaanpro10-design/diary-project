#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define USER_FILE "user.txt"

struct User{
    char username[50];
    char password[50];
};

void clearScreen();
void clearInputBuffer();
void registerUser();
void loginUser();
void diaryMenu(char username[]);
void addEntry(char username[]);
void viewEntries(char username[]);


int main(){

    int choice;
    
    do{
        clearScreen();
        printf("=====================================\n");
        printf("   PERSONAL DIARY MANAGEMENT SYSTEM\n");
        printf("=====================================\n");
        printf("\n1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("\nEnter your choice: ");

        if(scanf("%d", &choice) != 1){
            printf("Invalid input. Enter a number: \n");
            clearInputBuffer();
            choice = 0;
            continue;
        }
        clearInputBuffer();

        switch(choice){
            case 1:
                registerUser();
            case 2:
                loginUser();
            case 3:
                printf("Thanks for using our diary Manager system.");
                break;
        }

        if(choice != 3){
            printf("\n Press Enter to continue.");
            getchar();
        }
    }while(choice != 3);

    return 0;
}

void clearScreen(){
    #ifdef _WIN32
        system("cls");
    #endif
}
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void registerUser(){
    struct User user;
    struct User tempUser; 
    FILE *fp;
    int usernameExists = 0;

    clearScreen();

    printf("--- User Registration ---\n");

    printf("Enter a new username (no spaces): ");
    scanf("%49s", user.username);
    clearInputBuffer();

    printf("Enter a new password (no spaces): ");
    scanf("%49s", user.password);
    clearInputBuffer();

    fp = fopen(USER_FILE, "r");
    if(fp != NULL){
        while(fscanf(fp, "%49s %49s", tempUser.username, tempUser.password) == 2){
            if(strcmp(user.username, tempUser.username) == 0){
                usernameExists = 1;
            }
        }fclose(fp);
    }

    if(usernameExists){
        printf("\nError: This username is already taken. Choose another.\n");
    }
    else{
        fp = fopen(USER_FILE, "a");
        if(fp == NULL){
            printf("\nError: Could not open user file for writing.\n");
            return;
        }
        fprintf(fp, "%49s %49s", user.username, user.password);
        fclose(fp);
        printf("\nRegistration successful! You can now login.\n");
    }

}
void loginUser(){
    char username[50], password[50];
    struct User tempuser;
    FILE *fp;
    int loginsucces = 0;

    clearScreen();
    printf("--- User Login ---\n");

    printf("Enter username: ");
    scanf("%49s", username);
    clearInputBuffer();
    
    printf("Enter password: ");
    scanf("%49s", password);
    clearInputBuffer();

    fp = fopen(USER_FILE, "r");
    if(fp == NULL){
        printf("\nError: No users are registered. Please register first.\n");
        return ;
    }
    while(fscanf(fp, "%49s %49s", tempuser.username, tempuser.password) == 2){
        if(strcmp(username, tempuser.username) == 0 && strcmp(password, tempuser.password) == 0){
            loginsucces = 1;
            break;
        } 
    }
    fclose(fp);

    if(loginsucces){
        printf("\nLogin successful! Welcome, %s.\n", username);
        printf("Press Enter to proceed to your diary...");
        getchar();
        diaryMenu(username);
    }
    else {
        printf("\nError: Invalid username or password.\n");
    }
}

void diaryMenu(char username[]){
    int choice;

    do{
        clearScreen();
        printf("=====================================\n");
        printf("   DIARY MENU - Welcome, %s\n", username);
        printf("=====================================\n");
        printf("\n1. Add New Diary Entry\n");
        printf("2. View All Entries\n");
        printf("3. Logout\n");
        printf("\nEnter your choice: ");

        if(scanf("%d", &choice) != 1){
            printf("Invalid choice, Please enter a number: ");
            clearInputBuffer();
            choice = 0;
            continue;
        }

        clearInputBuffer();

        switch(choice){
            case 1:
                addEntry(username);
            case 2:
                viewEntries(username);
            case 3:
                printf("\nLogging out...\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
        if (choice != 3) {
            printf("\nPress Enter to return to the diary menu...");
            getchar();
        }

    }while (choice != 3);
}

void addEntry(char username[]) {
    FILE *fp;
    char diaryFilename[100];
    char entryBuffer[2048];
    time_t now;
    
    // Create the user-specific filename
    // e.g., if username is "alex", filename becomes "alex_diary.txt"
    sprintf(diaryFilename, "%s_diary.txt", username);

    // Open in 'a' (append) mode to add to existing entries
    fp = fopen(diaryFilename, "a");
    if (fp == NULL) {
        printf("Error: Could not open your diary file.\n");
        return;
    }

    clearScreen();
    printf("--- Add New Entry ---\n");
    printf("Type your entry. Enter a new line with just '~' to save.\n\n");

    // Get and write the current time
    time(&now);
    fprintf(fp, "\n========================================\n");
    fprintf(fp, "Date: %s", ctime(&now)); // ctime() includes a newline
    fprintf(fp, "----------------------------------------\n\n");

    // Read lines from user until they type "~"
    while (1) {
        fgets(entryBuffer, sizeof(entryBuffer), stdin);
        // Check if the user just typed "~"
        if (strcmp(entryBuffer, "~\n") == 0) {
            break;
        }
        fputs(entryBuffer, fp); // Write the line to the file
    }

    fclose(fp);
    printf("\nEntry saved successfully!\n");
}

void viewEntries(char username[]) {
    FILE *fp;
    char diaryFilename[100];
    char readBuffer[2048];
    
    // Create the user-specific filename to read from
    sprintf(diaryFilename, "%s_diary.txt", username);

    fp = fopen(diaryFilename, "r");
    if (fp == NULL) {
        clearScreen();
        printf("--- View Entries ---\n");
        printf("\nNo diary entries found for %s.\n", username);
        return;
    }

    clearScreen();
    printf("--- %s's Diary Entries ---\n", username);

    // Read the file line by line and print it
    while (fgets(readBuffer, sizeof(readBuffer), fp) != NULL) {
        printf("%s", readBuffer);
    }

    fclose(fp);
    printf("\n--- End of Entries ---\n");
}