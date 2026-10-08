#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>

struct books{
    int id;
    char bookName[100];
    char authorName[100];
    char date[20];
}b;

struct student{
    int id;
    char sName[100];
    char sDepartment[100];
    int sRoll;
    char bookName[100];
    char date[20];
}s;

FILE *fp;

void addBook();
void booksList();
void del();
void issueBook();
void issueList();
void delIssue(); // new function prototype

int main(){

    int ch;

    while(1){
        system("cls");
        printf("<== Book Wallet ==>\n");
        printf("1. Add Book\n");
        printf("2. Books List\n");
        printf("3. Remove Book\n");
        printf("4. Issue Book\n");
        printf("5. Issued Book List\n");
        printf("6. Remove Issued Book\n");
        printf("0. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch(ch){
        case 0:
            exit(0);

        case 1:
            addBook();
            break;

        case 2:
            booksList();
            break;

        case 3:
            del();
            break;

        case 4:
            issueBook();
            break;

        case 5:
            issueList();
            break;

        case 6:
            delIssue();
            break;

        default:
            printf("Invalid Choice...\n\n");
        }
        printf("\nPress Any Key To Continue...");
        getch();
    }

    return 0;
}


void addBook(){
    char myDate[20];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year + 1900);
    strcpy(b.date, myDate);

    fp = fopen("books.txt", "a"); // text append mode

    printf("Enter book id: ");
    scanf("%d", &b.id);

    printf("Enter book name: ");
    fflush(stdin);
    gets(b.bookName);

    printf("Enter author name: ");
    fflush(stdin);
    gets(b.authorName);

    printf("Book Added Successfully\n");

    // write record as plain text (tab separated)
    fprintf(fp, "%d\t%s\t%s\t%s\n", b.id, b.bookName, b.authorName, b.date);

    fclose(fp);
}


void booksList(){
    system("cls");
    printf("<== Available Books ==>\n\n");
    printf("%-10s %-30s %-20s %s\n\n", "Book id", "Book Name", "Author", "Date");

    fp = fopen("books.txt", "r");

    struct books bookArr[100];
    int count = 0;

    // Read line by line
    while(fscanf(fp, "%d\t%49[^\t]\t%49[^\t]\t%11[^\n]\n",
                 &bookArr[count].id,
                 bookArr[count].bookName,
                 bookArr[count].authorName,
                 bookArr[count].date) == 4){
        count++;
    }
    fclose(fp);

    // Sort by ID
    for(int i=0; i<count-1; i++){
        for(int j=i+1; j<count; j++){
            if(bookArr[i].id > bookArr[j].id){
                struct books temp = bookArr[i];
                bookArr[i] = bookArr[j];
                bookArr[j] = temp;
            }
        }
    }

    // Print sorted list
    for(int i=0; i<count; i++){
        printf("%-10d %-30s %-20s %s\n",
               bookArr[i].id, bookArr[i].bookName,
               bookArr[i].authorName, bookArr[i].date);
    }
}


void del(){
    int id, f=0;
    system("cls");
    printf("<== Remove Books ==>\n\n");
    printf("Enter Book id to remove: ");
    scanf("%d", &id);

    FILE *ft;

    fp = fopen("books.txt", "r");
    ft = fopen("temp.txt", "w");

    while(fscanf(fp, "%d\t%49[^\t]\t%49[^\t]\t%11[^\n]\n",
                 &b.id, b.bookName, b.authorName, b.date) == 4){
        if(id == b.id){
            f=1;
        }else{
            fprintf(ft, "%d\t%s\t%s\t%s\n", b.id, b.bookName, b.authorName, b.date);
        }
    }

    if(f==1){
        printf("\n\nDeleted Successfully.");
    }else{
        printf("\n\nRecord Not Found !");
    }

    fclose(fp);
    fclose(ft);

    remove("books.txt");
    rename("temp.txt", "books.txt");
}


void issueBook(){
    char myDate[20];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year + 1900);
    strcpy(s.date, myDate);

    int f=0;
    system("cls");
    printf("<== Issue Books ==>\n\n");

    printf("Enter Book id to issue: ");
    scanf("%d", &s.id);

    // check if book exists
    fp = fopen("books.txt", "r");
    while(fscanf(fp, "%d\t%49[^\t]\t%49[^\t]\t%11[^\n]\n",
                 &b.id, b.bookName, b.authorName, b.date) == 4){
        if(b.id == s.id){
            strcpy(s.bookName, b.bookName);
            f=1;
            break;
        }
    }
    fclose(fp);

    if(!f){
        printf("No book found with this id\n");
        return;
    }

    fp = fopen("issue.txt", "a");

    printf("Enter Student Name: ");
    fflush(stdin);
    gets(s.sName);

    printf("Enter Student Department: ");
    fflush(stdin);
    gets(s.sDepartment);

    printf("Enter Student Roll: ");
    scanf("%d", &s.sRoll);

    printf("Book Issued Successfully\n");

    // write record as plain text
    fprintf(fp, "%d\t%s\t%s\t%d\t%s\t%s\n",
            s.id, s.sName, s.sDepartment, s.sRoll, s.bookName, s.date);

    fclose(fp);
}


void issueList(){
    system("cls");
    printf("<== Book Issue List ==>\n\n");
    printf("%-10s %-30s %-20s %-10s %-30s %s\n\n",
           "S.id", "Name", "Department", "Roll", "Book Name", "Date");

    fp = fopen("issue.txt", "r");

    struct student stuArr[100];
    int count = 0;

    while(fscanf(fp, "%d\t%49[^\t]\t%49[^\t]\t%d\t%49[^\t]\t%11[^\n]\n",
                 &stuArr[count].id,
                 stuArr[count].sName,
                 stuArr[count].sDepartment,
                 &stuArr[count].sRoll,
                 stuArr[count].bookName,
                 stuArr[count].date) == 6){
        count++;
    }
    fclose(fp);

    // sort by ID
    for(int i=0; i<count-1; i++){
        for(int j=i+1; j<count; j++){
            if(stuArr[i].id > stuArr[j].id){
                struct student temp = stuArr[i];
                stuArr[i] = stuArr[j];
                stuArr[j] = temp;
            }
        }
    }

    // print
    for(int i=0; i<count; i++){
        printf("%-10d %-30s %-20s %-10d %-30s %s\n",
               stuArr[i].id, stuArr[i].sName, stuArr[i].sDepartment,
               stuArr[i].sRoll, stuArr[i].bookName, stuArr[i].date);
    }
}


void delIssue(){
    int id, f=0;
    system("cls");
    printf("<== Remove Issued Book ==>\n\n");
    printf("Enter Issued Book id to remove: ");
    scanf("%d", &id);

    FILE *ft;

    fp = fopen("issue.txt", "r");
    if(fp == NULL){
        printf("No issue records found!\n");
        return;
    }

    ft = fopen("temp.txt", "w");

    while(fscanf(fp, "%d\t%49[^\t]\t%49[^\t]\t%d\t%49[^\t]\t%11[^\n]\n",
                 &s.id, s.sName, s.sDepartment, &s.sRoll, s.bookName, s.date) == 6){
        if(id == s.id){
            f = 1; // found, skip writing
        } else {
            fprintf(ft, "%d\t%s\t%s\t%d\t%s\t%s\n",
                    s.id, s.sName, s.sDepartment, s.sRoll, s.bookName, s.date);
        }
    }

    fclose(fp);
    fclose(ft);

    remove("issue.txt");
    rename("temp.txt", "issue.txt");

    if(f == 1){
        printf("\n\nIssued book record deleted successfully.");
    } else {
        printf("\n\nRecord not found!");
    }
}


