/*
Description: Command line program to hold and 
modify student records in
a text file. 
Name: Kelly Olsson
Date: July 2021
*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define LINESIZE 1024

/* Structure to hold student records */
typedef struct record record;
struct record {
    char id[LINESIZE];
    int score;
};

void printMenu();
int getRecordInfo(char id[], int *scrPtr);
int validateID(const char arr[]);
int validateScore(const char numArr[]);
void printRecords(FILE **filep, size_t size);
void modifyRecord(record arr[], size_t size, int record);
void writeRecord(FILE **filep, const char arr[], const int num);

/* Program driver */
int main(int argc, char*argv[]) 
{
    char userChoice[LINESIZE];
    int finalChoice;
    char st_id[LINESIZE];
    int st_score;
    char st_score_arr[LINESIZE];
    int valid;
    size_t arraySize = 0;
    FILE *fp;

    /* User must enter a text file name and the program file name */
    if(argc > 1){
        if ( ( fp = fopen(argv[1], "wb+") ) == NULL) {
            printf("File could not be opened\n");
        } else {
            
            /* Displays user choices and obtains user's choice and validates it */
            while (finalChoice != -2) {
                printMenu();
                printf("Please type the number of your choice: ");
                if (fgets(userChoice, LINESIZE - 1, stdin) == NULL) break;

                for (int i = 0; userChoice[i + 1] != '\0'; i++) {
                    if ( (userChoice[i] == '-') && (i == 0) ) {
                        continue;
                    } else if ( (i > 0) && (isdigit(userChoice[i]) == 0) ) {
                        valid = -1;
                        break;
                    }
                }

                if (valid == -1) {
                    valid = 0;
                    continue;
                }

                finalChoice = atoi(userChoice);

                if (finalChoice == -1) {
                    valid = getRecordInfo(st_id, &st_score);
                    if ( valid != -1 ) {
                        fseek( fp, 0, SEEK_END);
                        writeRecord(&fp, st_id, st_score);
                        arraySize++;
                    }
                } else if (finalChoice == 0) { 
                    printRecords(&fp, arraySize);
                } else if (finalChoice > 0) {
                    if (finalChoice > arraySize) {
                        continue;
                    }
                    valid = getRecordInfo(st_id, &st_score);
                    if ( valid != -1 ) {
                        rewind( fp );
                        fseek(fp, (finalChoice - 1) * sizeof(record), SEEK_SET);
                        writeRecord(&fp, st_id, st_score);
                    }
                }
                valid = 0;
            }

        puts("Thank you for using the Student Scorekeeper. Goodbye.");

        fclose( fp );
        }
    } else {
        puts("Please enter a file name.");
    }

    return 0;
}

/* Displays user choices */
void printMenu() 
{
    puts("\nMenu of Options");
    printf("%10d%80s\n", -2, "quit");
    printf("%10d%80s\n", -1, "append a record");
    printf("%10d%80s\n", 0, "display all records");
    printf("%10s%80s\n", "n", "(a positive integer) modify record number n (if it exists)");
    puts("");
}

/* Drives obtaining a new student ID and score */
int getRecordInfo(char id[], int *scrPtr) 
{
    int valid = -1;
    char scr[LINESIZE];
    int finalScore;

    while ( valid == -1 ) {
        printf("%s", "Let's add a student record! Please first enter a valid student ID (please enter EOF or -1 to quit): ");
        if (fgets(id, LINESIZE - 1, stdin) == NULL) return -1; 
        
        id[strcspn(id, "\n")] = 0;

        if ( strcmp(id,"-1") == 0) {
            return -1;
        }
        valid = validateID(id);
    }

    valid = -1;

    while ( valid == -1) {
        printf("%s", "Please add a student score! Please enter a score between 0-100 (please enter EOF or -1 to quit): ");
        if (fgets(scr, LINESIZE - 1, stdin) == NULL) return -1; 

        scr[strcspn(scr, "\n")] = 0;

        if (strcmp(scr,"-1") == 0) {
            return -1;
        }
        valid = validateScore(scr);
    }

    finalScore = atoi(scr);
    *scrPtr = finalScore;
}

/* Checks student ID to BCIT's ID standards */
int validateID(const char arr[]) 
{
    int i = 0;

    while (arr[i] != '\0') {
        if ( ( arr[i] == 'a' ) && ( i == 0 ) ) {
            i++;
            continue;
        } else if ( i == 0 ) {
            return -1;
        } else if ( isdigit(arr[i]) == 0 ) {
            return -1;
        }
        i++;
    }
 
    if ( i > 9 || i < 9 ) {
        return -1;
    } else {
        return 0;
    }
}

/* Checks if the score is within 0-100 */
int validateScore(const char numArr[]) 
{
    int temp;

    for (int i = 0; numArr[i] != '\0'; i++) {
        if ( isdigit(numArr[i]) == 0 ) {
        return -1;
        }
    }

    temp = atoi(numArr);

    if ( temp < 0 || temp > 100 ) {
        return -1;
    } else {
        return 0;
    }
}

/* Prints the current stored student records */
void printRecords(FILE **filep, size_t size) 
{
    char arr[LINESIZE];
    int num;

    rewind( *filep );

    if ( size != 0 ) {

        fprintf(stderr, "\nStudent Records:\n\n");

        for (int i = 0; i < size; i++) {
            fread(arr, sizeof(arr), 1, *filep);
            fread(&num, sizeof(int), 1, *filep);
            fprintf(stderr, "%d %s %d\n", i+1, arr, num);
        }
    }
}

/* Writes student records to the desired text file */ 
void writeRecord(FILE **filep, const char arr[], const int num)
{
    record record1;

    strcpy(record1.id, arr);
    record1.score = num;
    fwrite(record1.id, sizeof(record1.id), 1, *filep);
    fwrite(&record1.score, sizeof(record1.score), 1, *filep);
}
