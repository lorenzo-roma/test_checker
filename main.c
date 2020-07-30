#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 1024
#define OUTPUT_TEMP "C:/Users/Master/Documents/Development/api-proj/tests/tmp.txt"

typedef struct node {
    char inputPath[MAX_LENGTH + 1];
    char outputPath[MAX_LENGTH + 1];
    struct node *prev;
} Test;

void executeCommand();

Test *getTest();

void pushTest(Test *test);

void executeTests();

void executeTest(Test *test);

int compareFiles(char*path1, char*path2);
int compareFile(FILE * fPtr1, FILE * fPtr2, int * line, int * col);

Test *testList;
char toExecute[MAX_LENGTH + 1];

int main() {
    testList = NULL;
    //printf("Insert path to program to execute:\n");
    fgets(toExecute, MAX_LENGTH, stdin);
    strtok(toExecute, "\n");
    for (int i = 0; i < 99; i++) {
        executeCommand();
    }

    return 0;
}

void executeCommand() {
    //printf("Do you want to add a test [y/n]?\n");
    int c = getchar();
    getchar();
    if (c=='y') {
        pushTest(getTest());
    } else {
        executeTests();
    }
}

Test *getTest() {
    Test *newTest = (Test *) malloc(sizeof(Test));
    //printf("Insert path to input test: \n");
    fgets(newTest->inputPath, MAX_LENGTH, stdin);
    //printf("Insert path to output test: \n");
    fgets(newTest->outputPath, MAX_LENGTH, stdin);
    strtok(newTest->inputPath, "\n");
    strtok(newTest->outputPath, "\n");
    return newTest;
}

void pushTest(Test *test) {
    if (testList == NULL) {
        testList = test;
        test->prev = NULL;
    } else {
        test->prev = testList;
        testList = test;
    }
}

void executeTests() {
    Test *currentTest = testList;
    while (currentTest != NULL) {
        executeTest(currentTest);
        currentTest = currentTest->prev;
    }
    getchar();
    exit(1);
}

void executeTest(Test *test) {
    char* command = NULL;
    command = toExecute;
    strcat(command, " < ");
    strcat(command, test->inputPath);
    strcat(command, " > ");
    strcat(command, OUTPUT_TEMP);
    system(command);
    printf("Testing %s : ", test->inputPath);
    compareFiles(test->outputPath, OUTPUT_TEMP);
}

int compareFiles(char*path1, char*path2){

        /* File pointer to hold reference of input file */
        FILE * fPtr1;
        FILE * fPtr2;

        int diff;
        int line, col;

        /*  Open all files to compare */
        fPtr1 = fopen(path1, "r");
        fPtr2 = fopen(path2, "r");

        /* fopen() return NULL if unable to open file in given mode. */
        if (fPtr1 == NULL || fPtr2 == NULL)
        {
            /* Unable to open file hence exit */
            printf("\nUnable to open file.\n");
            printf("Please check whether file exists and you have read privilege.\n");
            exit(EXIT_FAILURE);
        }
        /* Call function to compare file */
        diff = compareFile(fPtr1, fPtr2, &line, &col);
        if (diff == 0)
        {
            printf("\033[1;32m");
            printf(" Test Passed [✓]\n");
            printf("\033[0m");
            char command[1024] = "rm ";
            strcat(command, OUTPUT_TEMP);
            system(command);
        }
        else
        {
            printf("\033[1;31m");
            printf(" \nTest Failed [✗] at ");
            printf("Line: %d, col: %d\n", line, col);
            printf("\033[0m");
            char  command[1024] = "rm ";
            strcat(command, OUTPUT_TEMP);
            system(command);
        }
        /* Finally close files to release resources */
        fclose(fPtr1);
        fclose(fPtr2);
}

/**
 * Function to compare two files.
 * Returns 0 if both files are equivalent, otherwise returns
 * -1 and sets line and col where both file differ.
 */
int compareFile(FILE * fPtr1, FILE * fPtr2, int * line, int * col)
{
    char ch1, ch2;

    *line = 1;
    *col  = 0;

    do
    {
        // Input character from both files
        ch1 = fgetc(fPtr1);
        ch2 = fgetc(fPtr2);

        // Increment line
        if (ch1 == '\n')
        {
            *line += 1;
            *col = 0;
        }

        // If characters are not same then return -1
        if (ch1 != ch2)
            return -1;

        *col  += 1;

    } while (ch1 != EOF && ch2 != EOF);


    /* If both files have reached end */
    if (ch1 == EOF && ch2 == EOF)
        return 0;
    else
        return -1;
}