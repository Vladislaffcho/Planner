#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>

typedef char * string;

struct Database {
    int number;
    char type; /* 0 - unsigned; 1 - task; 2 - event; 3 - birthday */
    char status; /* d - deleted; i - invalid recording; 0 - unsigned; 1 - repeat daily; 2 - repeat weekly; 3 - repeat monthly; 4 repeat annually */
    int day;
    int month;
    int year;
    int hours;
    int minutes;
    char name[30];
};

struct Dates {
        int day;
        int month;
        int year;
};

struct Time {
        int minutes;
        int hours;
};

string removeSpacesFromString(string); // function to remove spaces from entered command
int calculateEventsInFile (void); // calculates IDs in file. Required to set start number in case we will be adding recordings
int parceCommand (string, int); // Function to parse user input
int create(string, int); // function to input BD, tasks and events data
int update(string); // function to update a recording
int deleteRecord(string); // function to delete recordings
int showAndCount(string, char); // Function to count data
int numOfRec(char, char, char*, int, char, struct Dates, struct Dates, struct Time); // function to calculate recordings with set parsmeters
int whatYearIs(char *, struct Dates *, struct Dates *); // function sets year to show/count/delete recordings
int whatMonthIs(char *, struct Dates *, struct Dates *); // function sets month to show/count/delete recordings
int whatWeekIs(char *, struct Dates *, struct Dates *); // function sets week to show/count/delete recordings
int whatDateIs(char *, struct Dates *, struct Dates *); // function sets date to show/count/delete recordings
int whatTimeIs(char *, struct Time *); // function sets time to show/count/delete recordings
int countDivider (string); // function counts dividers in string for saving time and date
int monthNo(string); // returns the number of entered month
int monthSize(int, int); // returns number of days in a year
string wordInStr (string); // returns string - parameter to update
struct Database parcing(string); // function to parce a string from file and pass data to structure
char recordingType(string); // determine what recording type entered from the keyboard is
void saveEventsToFile(struct Database); // procedure to save entered data to file
void printRecordingInfo (struct Database, char); // procedure to print what we have done with the recording
int firstDayOfMonth(int, int); // function returns number of week for a month
int checkIfLeapYear(int); // function to define if chosen or current year is Leap year
void printCalendar (int, int, int*, int); // procedure to print calendar

int main(void) {
    int j = calculateEventsInFile(); // check how many IDs we have in file
    int branch = 0;
    string userCommand = malloc(sizeof(char) * 50);

    string temp = malloc(sizeof(char) * 50);

    do {
        if (branch == -1) {
            printf("Invalid command. Repeat input!\n");
        }
        printf("Enter a command: ");
        gets(userCommand); // input command
        strcpy(userCommand, removeSpacesFromString(userCommand));
        branch = parceCommand(userCommand, 0); // check what user wants to do
        if (branch != -1) {
            temp = strtok(userCommand, " ");
            temp = strtok(NULL, "");
        }
        switch(branch) {
            case 1: {
                branch = create(temp, j);
                if (branch != -1) j++;
                break;
            }
            case 2: branch = update(temp); break;
            case 3: branch = deleteRecord(temp); break;
            case 4: branch = showAndCount(temp, '1'); break;
            case 5:  {
                branch = showAndCount(temp, '4');
                if (branch > 0) printf("Recordings matching your request: %d\n", branch);
                break;
            }
            case 6: {
                printf("Commands:\n");
                printf("Create [ task [ Date ] [ Time ] ] [ event Date [ Time ] || birthday Date ] [ repeat ] Name\n");
                printf("Update [ ID num || Name ] [ Date || Time || Name || Type ]\n");
                printf("Delete [ ID num || Name ] [ task || event || birthday ] [ time || date || week || month || year ]\n");
                printf("Show [ ID num || calendar || Name ] [ task || event || birthday ] [ time || date || week || month || year ] [ sort [ ASC || DESC ] ]");
                printf("Count - count [ task || event || birthday || Name ] [ time || date || week || month || year ]\n\n");
                continue;
            }
        }
        if (branch == 0)
            printf("No recordings that match enteret parameter found.\n");
    } while (branch != 7);
    return 0;
}
/* End of Main Function */

/* * * Function to remove spaces * * */
string removeSpacesFromString(string userCommand) {
    string temp = (char*)calloc((strlen(userCommand)+1), sizeof(char));
    int k = 0;
    int j = 0;
    int l = 0;
    if (userCommand[k] == ' ')
        while (userCommand[k] == ' ')
            k++;

    for (k; k < strlen(userCommand); k++) {
        if (userCommand[k] == ' ') {
            j = k+1;
            while (userCommand[k] == userCommand[j]) {
                k++;
                j++;
            }
        }
        temp[l] = userCommand[k];
        l++;
    }
    if(temp[l-1] == ' ') {
        temp[l-1] = '\0';
    }
    return temp;
}
/* * * End of function to remove spaces from a string * * */

/* * * function to calculate recordings in file to set ID for user input * * */
int calculateEventsInFile (void) {
    int i = 0;
    FILE* f;
    f=fopen("data.txt","r");
    if (f == NULL) {
        return i + 1;
    }
    while(! feof(f)) {
        if (fgetc(f) == '~') {
            i++;
        }
    }
    fclose(f);
    return i + 1;
}
/* * * end of calculate recordings in file function * * */

/* * * Function to parse user command * * */
int parceCommand (string userCommand, int wordNum) {
    switch (wordNum) {
        case 0 : {
            if (strncmp(userCommand, "create", 6) == 0) {
                if (strcmp(userCommand, "create") == 0)
                    return -1;
                return 1;
            } else if (strncmp(userCommand, "update", 6) == 0) {
                if (strcmp(userCommand, "update") == 0)
                    return -1;
                return 2;
            } else if (strncmp(userCommand, "delete", 6) == 0) {
                if (strcmp(userCommand, "delete") == 0)
                    return -1;
                return 3;
            } else if (strncmp(userCommand, "show", 4) == 0) {
                if (strcmp(userCommand, "show") == 0)
                    return -1;
                return 4;
            } else if (strncmp(userCommand, "count", 5) == 0) {
                if (strcmp(userCommand, "count") == 0)
                    return -1;
                return 5;
            }
            else if (strncmp(userCommand, "help", 4) == 0) return 6;
            else if (strncmp(userCommand, "quit", 4) == 0) return 7;
            else return -1;
        }
        case 1 : {
            if (strncmp(userCommand, "task", 4) == 0) {
                if (strcmp(userCommand, "task") == 0)
                    return 1;
                return 12;
            }
            else if (strncmp(userCommand, "event", 5) == 0) {
                if (strcmp(userCommand, "event") == 0)
                    return 2;
                return 22;
            }

            else if (strncmp(userCommand, "birthday", 8) == 0) {
                if (strcmp(userCommand, "birthday") == 0)
                    return 3;
                return 32;
            }
            else if (strncmp(userCommand, "ID", 2) == 0) {
                if (strcmp(userCommand, "ID") == 0)
                    return 0;
                return 4;
            }
            else if (strncmp(userCommand, "name", 4) == 0) {
                if (strcmp(userCommand, "name") == 0)
                    return 0;
                return 5;
            }
            else if (strncmp(userCommand, "calendar", 8) == 0) {
                if (strcmp(userCommand, "calendar") == 0)
                    return 0;
                return 6;
            }
            else if (strncmp(userCommand, "all", 3) == 0) {
                if (strcmp(userCommand, "all") != 0)
                    return 0;
                return 7;
            }
            break;
        }
        case 2 : {
            if (strncmp(userCommand, "date", 4) == 0) {
                if (strcmp(userCommand, "date") == 0)
                    return 0;
                return 1;
            } else if (strncmp(userCommand, "time", 4) == 0) {
                if (strcmp(userCommand, "time") == 0)
                    return 0;
                return 2;
            } else if (strncmp(userCommand, "name", 4) == 0) {
                if (strcmp(userCommand, "name") == 0)
                    return 0;
                return 3;
            }
            else if (strncmp(userCommand, "type", 4) == 0) {
                if (strcmp(userCommand, "type") == 0)
                    return 0;
                return 4;
            }
            else if (strncmp(userCommand, "task", 4) == 0) {
                if (strcmp(userCommand, "tesk") == 0)
                    return 0;
                return 5;
            }
            else if (strncmp(userCommand, "event", 5) == 0) {
                if (strcmp(userCommand, "event") == 0)
                    return 0;
                return 6;
            }
            else if (strncmp(userCommand, "birthday", 8) == 0) {
                if (strcmp(userCommand, "birthday") == 0)
                    return 0;
                return 7;
            }
            else if (strncmp(userCommand, "week", 4) == 0) {
                if (strcmp(userCommand, "week") == 0)
                    return 0;
                return 8;
            }
            else if (strncmp(userCommand, "month", 5) == 0) {
                if (strcmp(userCommand, "month") == 0)
                    return 0;
                return 9;
            }
            else if (strncmp(userCommand, "year", 4) == 0) {
                if (strcmp(userCommand, "year") == 0)
                    return 0;
                return 10;
            }
            break;
        }
        case 3 : {
            if (strncmp(userCommand, "date", 4) == 0) return 1;
            else if (strncmp(userCommand, "time", 4) == 0) return 2;
            else if (strncmp(userCommand, "week", 4) == 0) return 3;
            else if (strncmp(userCommand, "month", 5) == 0) return 4;
            else if (strncmp(userCommand, "year", 4) == 0) return 5;
            break;
        }
    }
    return 0;
}
/* * * End of user command parsing function * * */

/* * * Function to create recording * * */
int create(string temp, int i) {

    int l = 0;
    struct Database recTemp;
    recTemp.number = i;
    recTemp.status = '0';
    recTemp.day = 0;
    recTemp.month = 0;
    recTemp.year = 0;
    recTemp.hours = 0;
    recTemp.minutes = 0;
    int wordsNo = 1;
    int createPar = parceCommand(temp, wordsNo);
    if (createPar == 12 || createPar == 22 || createPar == 32)
        recTemp.type = ((createPar - 2) / 10) + '0';
    else {
        printf("Error: ");
        return -1;
    }

    temp = strtok(temp, " ");
    temp = strtok(NULL, "");
    string temp2 = (char*)calloc((strlen(temp)+1), sizeof(char));
    strcpy(temp2, temp);
    temp = strtok(temp, " ");
    if (strcmp(temp, temp2) == 0) {
        if (recTemp.type == '1') {
            strcpy(recTemp.name, temp);
            saveEventsToFile(recTemp);
            printRecordingInfo(recTemp, '1');
            return 1;
        }
        else {
            printf("Error: You cannot create this recording without a date.\n");
            return -1;
        }
    }

    int divider = 0;
    divider = countDivider(temp);

    switch (divider) {
        case 2: {
            temp2 = strtok(temp2, " ");
            temp2 = strtok(NULL, "");
            temp = strtok(temp, ".:");
            recTemp.day = atoi(temp);
            temp = strtok(NULL, ".:");
            recTemp.month = atoi(temp);
            temp = strtok(NULL, "");
            recTemp.year = atoi(temp);
            l++;
            if (recTemp.day == 0 || recTemp.month == 0 || recTemp.year == 0) {
                printf ("Error: ");
                return -1;
            }
            break;
        }
        case 1: {
            temp2 = strtok(temp2, " ");
            temp2 = strtok(NULL, "");
            temp = strtok(temp, ".:");
            recTemp.hours = atoi(temp);
            temp = strtok(NULL, "");
            recTemp.minutes = atoi(temp);
            l++;
            if (recTemp.hours == 0 || recTemp.minutes == 0) {
                printf ("Error: ");
                return -1;
            }
            break;
        }
    }

    if (l > 0) {

        strcpy(temp, temp2);
        temp = strtok(temp, " ");
        if(strcmp(temp, temp2) == 0) {

            if (recTemp.type != '1' && (recTemp.year < 1900 || recTemp.month > 12 || recTemp.month < 1 || recTemp.day > monthSize(recTemp.month, recTemp.year) || recTemp.day < 1))  {
                printf("Error: invalid date.\n");
                return -1;
            }
            strcpy(recTemp.name, temp);
            saveEventsToFile(recTemp);
            printRecordingInfo(recTemp, '1');
            return 1;
        }
        divider = 0;
        divider = countDivider(temp);
        switch (divider) {
            case 2: {
                temp2 = strtok(temp2, " ");
                temp2 = strtok(NULL, "");
                temp = strtok(temp, ".:");
                recTemp.day = atoi(temp);
                temp = strtok(NULL, ".:");
                recTemp.month = atoi(temp);
                temp = strtok(NULL, "");
                recTemp.year = atoi(temp);
                l++;
                if (recTemp.day == 0 || recTemp.month == 0 || recTemp.year == 0) {
                    printf ("Error: ");
                    return -1;
                }
                break;
            }
            case 1: {
                temp2 = strtok(temp2, " ");
                temp2 = strtok(NULL, "");
                temp = strtok(temp, ".:");
                recTemp.hours = atoi(temp);
                temp = strtok(NULL, "");
                recTemp.minutes = atoi(temp);
                l++;
                if (recTemp.hours == 0 || recTemp.minutes == 0) {
                    printf ("Error: ");
                    return -1;
                }
                break;
            }
        }
    }

    if (l == 0 && recTemp.type != '1') {
        printf("Error: You cannot create recording without a date.\n");
        return -1;
    } else if (l == 1 && recTemp.type != '1') {
        printf("Error: You cannot create this recording with time only.\n");
        return -1;
    }


    int j = 3;

    strcpy(temp, temp2); // both == repeat daily 777

    if (strncmp(temp2, "repeat", 6) == 0) {
        temp2 = strtok(temp2, " ");
        printf("1 - %s\n", temp2);
        getchar();
        temp2 = strtok(NULL, " ");
        printf("1 - %s\n", temp2);
        getchar();
        if (temp2 != NULL && (strcmp(temp2, "daily") == 0 || strcmp(temp2, "weekly") == 0 || strcmp(temp2, "monthly") == 0 || strcmp(temp2, "yearly") == 0)) {
            temp2 = strtok(NULL, "");
            if (temp2 != NULL)
                j = 0;
        }
    }

    if (j == 0) {
        temp = strtok(temp, " ");
        temp = strtok(NULL, " ");
        if (strcmp(temp, "daily") == 0) {
            recTemp.status = '1';
        } else if (strcmp(temp, "weekly") == 0) {
            recTemp.status = '2';
        } else if (strcmp(temp, "monthly") == 0) {
            recTemp.status = '3';
        } else if ((strcmp(temp, "yearly") == 0) || (strcmp(temp, "annually") == 0)) {
            recTemp.status = '4';
        }
        temp = strtok(NULL, "");
    }

    strcpy(recTemp.name, temp);

    if (l != 0 && recTemp.type != '1' && (recTemp.year < 1900 || recTemp.month > 12 || recTemp.month < 1 || recTemp.day > monthSize(recTemp.month, recTemp.year) || recTemp.day < 1))  {
        printf("Error: invalid date.\n");
        return -1;
    } else if (l != 0 && recTemp.type != '1' && (recTemp.minutes < 0 || recTemp.minutes > 59 || recTemp.hours < 0 || recTemp.hours > 23))  {
        printf("Error: invalid time.\n");
        return -1;
    }

    saveEventsToFile(recTemp);
    printRecordingInfo(recTemp, '1');
    return 1;
}
/* * * End of enter data function * * */

/* * * Function to update recording * * */
int update(string userInput) {

    struct Database updBy;
    int parameterToUpdate;
    int wordsNo = 1;
    string nameToCompare = malloc(sizeof(char) * 50);
    string tempRec = malloc(sizeof(char) * 50);
    int numberToCompare = 0;
    int i = 0;
    int divider = 0;

    parameterToUpdate = parceCommand(userInput, wordsNo);
    if (parameterToUpdate == 0) return -1;
    wordsNo++;

    userInput = strtok(userInput, " ");

    switch (parameterToUpdate) {
        case 4 : { // update by ID
            userInput = strtok(NULL, " ");
            numberToCompare = atoi(userInput);
            userInput = strtok(NULL, "");
            break;
        }
        case 5 : { // update by name
            userInput = strtok(NULL, "");
            strcpy(tempRec, wordInStr(userInput));
            if (strncmp(tempRec, "no", 2) == 0) return -1;
            for(i = 0; i < strlen(userInput) - strlen(tempRec) - 1; i++) {
                nameToCompare[i] = userInput[i];
            }
            nameToCompare[i] = '\0';
            userInput = strtok(userInput, " ");
            userInput = strtok(NULL, "");
            break;
        }
    }
    parameterToUpdate = parceCommand(userInput, wordsNo);
    if (parameterToUpdate == 0) return -1;
    userInput = strtok(userInput, " ");
    userInput = strtok(NULL, "");
    switch (parameterToUpdate) {
        case 1 : { // update date
            divider = countDivider(userInput);
            if (divider != 2) return -1;
            userInput = strtok(userInput, " .:");
            updBy.day = atoi(userInput);
            userInput = strtok(NULL, " .:");
            updBy.month = atoi(userInput);
            userInput = strtok(NULL, "");
            updBy.year = atoi(userInput);
            break;
        }
        case 2 : { // update time
            divider = countDivider(userInput);
            if (divider != 1) return -1;
            userInput = strtok(userInput, " .:");
            updBy.hours = atoi(userInput);
            userInput = strtok(NULL, " .:");
            updBy.minutes = atoi(userInput);
            break;
        }
        case 3 : { // update name
            strcpy(updBy.name, userInput);
            break;
        }
        case 4 : { // update type
            updBy.type = recordingType(userInput);
            if (updBy.type == '0') return -1;
            break;
        }
    }

    struct Database recTemp2;

    FILE *f, *f3;
    f = fopen ("data.txt", "r+");
    f3 = fopen ("temp.txt", "a++");
    string textToParce = (char*)calloc(50, sizeof(char));
    string textToParce2 = (char*)calloc(50, sizeof(char));

    while ((!feof(f)) && (fgets(textToParce, 50, f))) {
        fprintf(f3, "%s", textToParce);
    }
    fclose(f);
    fclose(f3);
    char filename[] = "data.txt";
    remove(filename);

    f3 = fopen ("temp.txt", "r+");

    while ((!feof(f3)) && (fgets(textToParce, 50, f3))) {
        strcpy(textToParce2, textToParce);
        recTemp2 = parcing(textToParce);
        if ((strcmp(nameToCompare, recTemp2.name) == 0) || (numberToCompare == recTemp2.number)) {
            printf (" Recording ");
            printRecordingInfo(recTemp2, '0');
            switch (parameterToUpdate) {
                case 1: {
                    recTemp2.day = updBy.day;
                    recTemp2.month = updBy.month;
                    recTemp2.year = updBy.year;
                    break;
                }
                case 2: {
                    recTemp2.hours = updBy.hours;
                    recTemp2.minutes = updBy.minutes;
                    break;
                }
                case 3: {
                    strcpy(recTemp2.name, updBy.name);
                    break;
                }
                case 4: {
                    recTemp2.type = updBy.type;
                    break;
                }
            }
            printf (" has been updated: ");
            printRecordingInfo(recTemp2, '0');
            saveEventsToFile(recTemp2);
        }

        else {
            saveEventsToFile(recTemp2);
        }
    }
    fclose(f3);

    char filename2[] = "temp.txt";
    remove(filename2);
    return 1;
}
/* * * End of function to update recording * * */

/* * * function to delete recordings from file * * */
int deleteRecord(string userInput) {

    int delPar = 0;
    int wordsNo = 1;
    struct Database recTemp;
    struct Dates startDate = {1, 1, 1};
    struct Dates endDate = {1, 1, 1};
    struct Time delTime = {100, 100};
    string nameToDel = malloc(sizeof(char) * 50);
    strcpy(nameToDel, "noname");
    int chosenID = 0;

    delPar = parceCommand(userInput, wordsNo);
    if (delPar == 1 || delPar == 2 || delPar == 3)
        recTemp.type = delPar + '0';

    switch (delPar) {
        case 0 : return -1;
        case 12 : {
            wordsNo++;
            recTemp.type = '1';
            userInput = strtok(userInput, " ");
            userInput = strtok(NULL, "");
            break;
        }
        case 22 : {
            wordsNo++;
            recTemp.type = '2';
            userInput = strtok(userInput, " ");
            userInput = strtok(NULL, "");
            break;
        }
        case 32 : {
            wordsNo++;
            recTemp.type = '3';
            userInput = strtok(userInput, " ");
            userInput = strtok(NULL, "");
            break;
        }
        case 4 : { // delete by ID
            userInput = strtok(userInput, " ");
            userInput = strtok(NULL, "");
            chosenID = atoi(userInput);
            break;
        }
        case 5 : { // delete by name
            userInput = strtok(userInput, " ");
            userInput = strtok(NULL, "");
            strcpy(nameToDel, userInput);
            break;
        }
    }

    if (wordsNo == 2) {
        delPar = parceCommand(userInput, wordsNo);
        userInput = strtok(userInput, " ");
        userInput = strtok(NULL, "");
        switch (delPar) {
            case 0 : return -1;
            case 1 : { // delete by date
                if (whatDateIs(userInput, &startDate, &endDate) == -1) // show by date
                    return -1;
                break;
            }
            case 2 : { // delete by time
                if (whatTimeIs(userInput, &delTime) == -1) // check time from a string
                    return -1;
                break;
            }
            case 8 : {
                if (whatWeekIs(userInput, &startDate, &endDate) == -1) // show by week
                    return -1;
                break;
            }
            case 9 : { // delete by month
                if (whatMonthIs(userInput, &startDate, &endDate) == -1) // delete by month
                    return -1;
                break;
            }
            case 10 : {
                if (whatYearIs(userInput, &startDate, &endDate) == -1) // function to set frame to delete by year
                    return -1;
                break;
            }
        }
    }

    FILE *f, *f3;
    f = fopen ("data.txt", "r+");
    f3 = fopen ("temp.txt", "a++");
    string strFromFile = malloc(sizeof(char) * 50);
    string strToParse = malloc(sizeof(char) * 50);

    struct Database fromFile;
    while ((!feof(f)) && (fgets(strFromFile, 50, f))) {
        strcpy(strToParse, strFromFile);
        fromFile = parcing(strFromFile);
        if ((chosenID == fromFile.number) || (recTemp.type == fromFile.type) || (strcmp(nameToDel, fromFile.name) == 0)) { /* if we face required ID to delete*/
            if (wordsNo == 2) {
                if (((delTime.minutes == fromFile.minutes) && (delTime.hours == fromFile.hours)) ||
                   (((startDate.day <= fromFile.day) && (startDate.month <= fromFile.month) && ((startDate.year <= fromFile.year))) && ((endDate.day >= fromFile.day) && (endDate.month >= fromFile.month) && ((endDate.year >= fromFile.year))))) {
                    fromFile.status = 'd';
                } else {
                    fprintf(f3, "%s", strToParse);
                    continue;
                }
            }
            fromFile.status = 'd';
            fprintf(f3, "%d;%c;%c;%d;%d;%d;%d;%d*%s~\n", fromFile.number, fromFile.type, fromFile.status, fromFile.day, fromFile.month, fromFile.year, fromFile.hours, fromFile.minutes, fromFile.name);
            printRecordingInfo (fromFile, '2');
        } else {
            fprintf(f3, "%s", strToParse);
        }
    }

    fclose(f);
    fclose(f3);

    f = fopen ("data.txt", "w+");
    f3 = fopen ("temp.txt", "r");

    while ((!feof(f3)) && (fgets(strFromFile, 50, f3))) {
        fprintf(f, "%s", strFromFile);
    }

    fclose(f);
    fclose(f3);

    char filename[] = "temp.txt";
    remove(filename);
}
/* * * end of delete all recordings by type function * * */

/* * * Show or count recordings function * * */
int showAndCount(string userInput, char command) {

    int chosenID = 0;
    int showPar = 0;
    char type = '0';
    char parameters = '0';
    int wordsNo = 1;
    int sortBy = 0;
    string nameToShow = malloc(sizeof(char) * 30);
    string tempForSort = malloc(sizeof(char) * 50);
    string sortParameter = malloc(sizeof(char) * 20);
    struct Dates startDate = {1, 1, 1};
    struct Dates endDate = {1, 1, 1};
    struct Time showTime = {100, 100};
    int i = 0;
    int l = 0;

    strcpy(tempForSort, userInput); //String to check if there sorting requested

    if (strstr(tempForSort, "sort") != NULL) { //if use soerting parameters
        sortParameter = strstr(tempForSort, "sort");
        for(i = 0; i < (strlen(tempForSort) - strlen(sortParameter)) - 1; i++) {
            userInput[i] = tempForSort[i];
        }
        userInput[i] = '\0';

        sortParameter = strtok(sortParameter, " ");
        sortParameter = strtok(NULL, "");

        if (strcmp(sortParameter, "asc") == 0) {
            sortBy = 0;
        } else if (strcmp(sortParameter, "dec") == 0) {
            sortBy = 1;
        }
    }

    showPar = parceCommand(userInput, wordsNo);
    if (showPar == 1 || showPar == 2 || showPar == 3)
        type = showPar + '0';

    switch (showPar) {
        case 12 : { // show task with parameters
            wordsNo = 3;
            type = '1';
            userInput = strtok(userInput, " ");
            userInput = strtok(NULL, "");
            break;
        }
        case 22 : { // show event with parameters
            wordsNo = 3;
            type = '2';
            userInput = strtok(userInput, " ");
            userInput = strtok(NULL, "");
            break;
        }
        case 32 : { // show birthday with parameters
            wordsNo = 3;
            type = '3';
            userInput = strtok(userInput, " ");
            userInput = strtok(NULL, "");
            break;
        }
        case 4 : { // show ID
            userInput = strtok(userInput, " ");
            userInput = strtok(NULL, "");
            chosenID = atoi(userInput);
            break;
        }
        case 5 : { // show name
            userInput = strtok(userInput, " ");
            userInput = strtok(NULL, "");
            strcpy(nameToShow, userInput);
            parameters = '2';
            break;
        }
        case 6 : { // show calendar
            wordsNo++;
            userInput = strtok(userInput, " ");
            userInput = strtok(NULL, "");
            command = '2';
            break;
        }
    }

    if (wordsNo == 2 && showPar == 6) {
        parameters = '1';
        showPar = parceCommand(userInput, wordsNo);
        userInput = strtok(userInput, " ");
        userInput = strtok(NULL, "");
        switch (showPar) {
            case 5 : { // show task in calendar
                type = '1';
                wordsNo++;
                break;
            }
            case 6 : { // show event in calendar
                type = '2';
                wordsNo++;
                break;
            }
            case 7 : { // show birthday in calendar
                type = '3';
                wordsNo++;
                break;
            }
            case 8 : { // by week
                if (whatWeekIs(userInput, &startDate, &endDate) == -1) // show by week
                    return -1;
                break;
            }
            case 9 : { // show concrete month in calendar
                if (whatMonthIs(userInput, &startDate, &endDate) == -1) // show by month
                    return -1;
                break;
            }
            case 10 : {
                if (whatYearIs(userInput, &startDate, &endDate) == -1) // show by ywar
                    return -1;
                break;
            }
            default : return -1;
        }
    }

    if (wordsNo == 3) {
        parameters = '1';
        showPar = parceCommand(userInput, wordsNo);
        userInput = strtok(userInput, " ");
        userInput = strtok(NULL, "");
        switch (showPar) {
            case 1 : { // show by date
                if (whatDateIs(userInput, &startDate, &endDate) == -1) // show by date
                    return -1;
                break;
            }
            case 2 : { // show by time
                if (whatTimeIs(userInput, &showTime) == -1) // show by time
                    return -1;
                break;
            }
            case 3 : { // by week
                if (whatWeekIs(userInput, &startDate, &endDate) == -1) // show by week
                    return -1;
                break;
            }
            case 4 : { // show concrete month in calendar
                if (whatMonthIs(userInput, &startDate, &endDate) == -1) // show by month
                    return -1;
                break;
            }
            case 5 : {
                if (whatYearIs(userInput, &startDate, &endDate) == -1) // show by year
                    return -1;
                if (command == '1') command = '3';
                break;
            }
            default : return -1;
        }
    }

    int k = 12;
    string stringFromFile = malloc(sizeof(char) * 50);
    int tempEndDate = 0;
    if (startDate.month != endDate.month) {
        if (startDate.month == 1 && endDate.month == 12) {
            k = 0;
            startDate.month = 1;
            endDate.month = startDate.month;
            parameters = '1';
        } else {
            k = 14;
            tempEndDate = endDate.day;
        }
    }

    struct Database *printRecord;
    i = 0;
    do { // cycle to choose required recordings and print calendar along with recordings
        FILE *f;
        f = fopen ("data.txt", "r");
        if (k == 0) {
            endDate.day = monthSize(startDate.month, startDate.year);
        } else if (k == 14) {
            if (startDate.month > endDate.month) {
                endDate.day = monthSize(startDate.month, startDate.year);
                endDate.month = startDate.month;
                endDate.year = startDate.year;
            }
            else if (startDate.month < endDate.month) {
                endDate.day = monthSize(startDate.month, startDate.year);
                endDate.month = startDate.month;
                endDate.year = startDate.year;
            }
        }

        int lastMonthDay = 0;
        struct Database temp;
        int j = 0;
        i = numOfRec(type, parameters, nameToShow, chosenID, command, startDate, endDate, showTime);
        l = l + i;
        if ((i == 0) && (command == '1')) return 0;

        if (command != '4') {
            printRecord = (struct Database*)calloc(i, sizeof(struct Database));

            while ((!feof(f)) && (fgets(stringFromFile, 50, f))) {
                temp = parcing(stringFromFile);
                if (temp.status != 'd') { // If program faces appropriate record in file
                    if ((type == '0' && parameters == '0') && command != '2') { // if no parameters chosen or
                        if (chosenID == temp.number) { // comparison by ID
                            printRecordingInfo (temp, '0');
                            return 1;
                        } else {
                            *printRecord = temp;
                            printRecord++;
                        }
                    }

                    else if (type == temp.type || parameters != '0') {

                        if (parameters == '1') {
                            if (((startDate.day <= temp.day && startDate.month <= temp.month && startDate.year <= temp.year) && (endDate.day >= temp.day && endDate.month >= temp.month && endDate.year >= temp.year))
                                || (showTime.hours == temp.hours && showTime.minutes == temp.minutes)) {
                                if (((temp.status == '0') || (command == '1')) && (type == temp.type || type == '0')) {
                                    *printRecord = temp;
                                    printRecord++;
                                }
                            }
                        }

                        else if (parameters == '0' || (parameters == '2' && strcmp(nameToShow, temp.name) == 0)) {
                            *printRecord = temp;
                            printRecord++;
                            continue;
                        }
                    }

                    if ((startDate.year != 1 && endDate.year != 1) && temp.status != '0' && temp.year <= endDate.year && (type == temp.type || type == '0')) {

                        if (startDate.year == temp.year && startDate.month == temp.month && endDate.day == startDate.day && startDate.day < temp.day) continue;

                        if ((endDate.day - startDate.day) < 8)
                            lastMonthDay = endDate.day;
                        else
                            lastMonthDay = monthSize(startDate.month, startDate.year);
                        switch (temp.status) {
                            case '1': {
                                if (temp.year < startDate.year) {
                                    temp.year = startDate.year;
                                    temp.month = startDate.month;
                                    temp.day = startDate.day;
                                }

                                else if (temp.year == startDate.year) {
                                    if (startDate.month > temp.month) {
                                        temp.month = startDate.month;
                                        temp.day = startDate.day;
                                    } else if (startDate.month < temp.month)
                                        break;
                                }
                                while (temp.day < startDate.day) temp.day++;

                                *printRecord = temp;
                                printRecord++;
                                while (temp.year <= endDate.year && temp.month <= endDate.month && temp.day < lastMonthDay) {
                                    temp.day++;
                                    *printRecord = temp;
                                    printRecord++;
                                }
                                break;
                            }
                            case '2': {
                                if ((temp.year < startDate.year) || (temp.month < startDate.month && temp.year <= startDate.year)) {
                                    temp.day = (temp.day % 7) + 6 - firstDayOfMonth(temp.month, temp.year) + 6 - firstDayOfMonth(startDate.month, startDate.year);
                                    if (temp.day > 7) temp.day = temp.day - 7;
                                    while (temp.day < startDate.day) temp.day+=7;
                                    temp.month = startDate.month;
                                    temp.year = startDate.year;
                                    for ( ; temp.day <= lastMonthDay; ) {
                                        *printRecord = temp;
                                        temp.day+=7;
                                        printRecord++;
                                    }
                                } else if (temp.month <= startDate.month && temp.year == startDate.year) {
                                    for ( ; temp.day <= lastMonthDay; ) {
                                        *printRecord = temp;
                                        temp.day+=7;
                                        printRecord++;
                                    }
                                }
                                break;
                            }
                            case '3': {
                                temp.month = startDate.month;
                                temp.year = startDate.year;
                                *printRecord = temp;
                                printRecord++;
                                break;
                            }
                            case '4': {
                                if (temp.month == startDate.month) {
                                    temp.year = startDate.year;
                                    *printRecord = temp;
                                    printRecord++;
                                }
                                break;
                            }
                        }
                    } else {
                        continue;
                    }
                }
            }

            printRecord-=i;

            fclose(f);

            int * dates;
            dates = (int*)calloc(i, sizeof(int));
            if(i == 0) dates[0] = 0;

            for (j = 0; j < i; j++) { // array to pass dates for chosen month in function
                dates[j] = printRecord->day;
                printRecord++;
            }
            printRecord -= i;

            if (command == '2') { // print calendar
                printCalendar(startDate.month-1, startDate.year, dates, i);
            }

            if (sortBy == 1) {
                int l = 0;
                if (l == 0) {
                    for (l = 0; l < i; l++) {
                      for (j = i - 1; j > l; j--) {
                          if (printRecord[j].number > printRecord[j-1].number) {
                              struct Database temp;
                              temp = printRecord[j - 1];
                              printRecord[j - 1] = printRecord[j];
                              printRecord[j] = temp;
                          }
                      }
                   }
                }
            }

            for (j = 0; j < i; j++) {
                printRecordingInfo(*printRecord, '0');
                printRecord++;
            }
            printRecord -= i;
            free(printRecord);
            if (k < 12) {
                k++;
                startDate.month++;
                endDate.month++;
            }
        }
        else if (k > 12) {
            if (k == 14 && startDate.month == 13) {
                startDate.month = 1;
                endDate.month = 1;
                startDate.year++;
                endDate.year++;
                endDate.day = tempEndDate;
                startDate.day = 1;
            } else {
                startDate.month++;
                endDate.month++;
                endDate.day = tempEndDate;
                startDate.day = 1;
            }

            k--;
        }
    } while (k != 12);
    if (l == 0) return 0;
}
/* * * End of show or count recordings function * * */

/* * * Function to calculate recordings to print * * */
int numOfRec(char type, char parameters, char nameToShow[], int chosenID, char command, struct Dates startDate, struct Dates endDate, struct Time showTime) {

    char stringFromFile[50];
    struct Database temp;
    int i = 0;
    int lastMonthDay = 0;
    FILE *f;
    f = fopen ("data.txt", "r");
    while ((!feof(f)) && (fgets(stringFromFile, 50, f))) {
        temp = parcing(stringFromFile);
        if (temp.status != 'd') { // If program faces appropriate record in file
            if ((type == '0' && parameters == '0') && command != '2') { // if no parameters chosen or
                if (chosenID == temp.number) return ++i; // comparison by ID
                i++;
            }

            else if (type == temp.type || parameters != '0') {

                if (parameters == '1') {
                    if (((startDate.day <= temp.day && startDate.month <= temp.month && startDate.year <= temp.year) && (endDate.day >= temp.day && endDate.month >= temp.month && endDate.year >= temp.year))
                        || (showTime.hours == temp.hours && showTime.minutes == temp.minutes)) {
                        if (((temp.status == '0') || (command == '1')) && (type == temp.type || type == '0')) {
                            i++;
                            continue;
                        }
                    }
                }

                else if (parameters == '0' || (parameters == '2' && strcmp(nameToShow, temp.name) == 0)) {
                    i++;
                    continue;
                }
            }

            if ((startDate.year != 1 && endDate.year != 1) && temp.status != '0' && temp.year <= endDate.year && (type == temp.type || type == '0')) {

                if (startDate.year == temp.year && startDate.month == temp.month && endDate.day == startDate.day && startDate.day < temp.day) continue;

                if ((endDate.day - startDate.day) < 8)
                    lastMonthDay = endDate.day;
                else
                    lastMonthDay = monthSize(startDate.month, startDate.year);

                switch (temp.status) {
                    case '1': {
                        if (temp.year < startDate.year) {
                            temp.year = startDate.year;
                            temp.month = startDate.month;
                            temp.day = startDate.day;
                        }

                        else if (temp.year == startDate.year) {
                            if (startDate.month > temp.month) {
                                temp.month = startDate.month;
                                temp.day = startDate.day;
                            } else if (startDate.month < temp.month)
                                break;
                        }
                        while (temp.day < startDate.day) temp.day++;
                        i++;
                        while (temp.year <= endDate.year && temp.month <= endDate.month && temp.day < lastMonthDay) {
                            temp.day++;
                            i++;
                        }
                        break;
                    }
                    case '2': {
                        if ((temp.year < startDate.year) || (temp.month < startDate.month && temp.year <= startDate.year)) {
                            temp.day = (temp.day % 7) + 6 - firstDayOfMonth(temp.month, temp.year) + 6 - firstDayOfMonth(startDate.month, startDate.year);
                            if (temp.day > 7) temp.day = temp.day - 7;
                            while (temp.day < startDate.day) temp.day+=7;
                            for ( ; temp.day <= lastMonthDay; ++i) {
                                temp.day+=7;
                                }
                        } else if (temp.month == startDate.month && temp.year == startDate.year) {
                            for ( ; temp.day <= lastMonthDay; ++i) {
                                temp.day+=7;
                            }
                        }
                        break;
                    }
                    case '3': {
                        i++;
                        break;
                    }
                    case '4': {
                        if (temp.month == startDate.month) i++;
                        break;
                    }
                }
            }
        } else {
            if (chosenID == temp.number) return 0;
            continue;
        }
    }
    return i;
}
/* * * End of function to calculate recordings to print * * */

/* * * Function to return year from a string * * */
int whatYearIs(char * userInput, struct Dates * startDate, struct Dates *endDate) {
    if (atoi(userInput) == NULL)
        return -1;
    startDate->year = atoi(userInput);
    endDate->year = startDate->year;
    startDate->month = 1;
    endDate->month = 12;
    startDate->day = 1;
    endDate->day = 31;
    return 0;
}
/* * * End of function to return year from a string * * */

/* * * Function to return month from a string * * */
int whatMonthIs(char * userInput, struct Dates * startDate, struct Dates *endDate) {
    userInput = strtok(userInput, " ");
    startDate->month = monthNo(userInput);
    if (startDate->month == 0)
        return -1;
    endDate->month = startDate->month;
    userInput = strtok(NULL, "");
    if (atoi(userInput) == NULL)
        return -1;
    startDate->year = atoi(userInput);
    endDate->year = startDate->year;
    startDate->day = 1;
    endDate->day = monthSize(startDate->month, startDate->year);
    return 0;
}
/* * * End of function to return month from a string * * */

/* * * Function to return date from a string * * */
int whatWeekIs(char * userInput, struct Dates * startDate, struct Dates * endDate) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    int weekNo = atoi(userInput);
    if (weekNo == 0)
        return -1;
    int passes = 0;
    startDate->year = t->tm_year+1900;
    endDate->year = startDate->year;
    if (firstDayOfMonth(1, startDate->year) >= 4) {
        passes = 7;
    }
    endDate->day = weekNo * 7 - firstDayOfMonth(1, startDate->year) + passes;
    endDate->month = 1;
    if (endDate->day > monthSize(1, t->tm_year+1900)) {
        for ( ; endDate->day > monthSize(endDate->month, t->tm_year+1900); endDate->month++) {
            endDate->day = endDate->day - monthSize(endDate->month, t->tm_year+1900);
        }
    }
    startDate->day = endDate->day - 6;
    startDate->month = endDate->month;
    if (startDate->day <= 0) {
        startDate->month = endDate->month - 1;
        if (startDate->month == 0) {
            startDate->month = 12;
            startDate->year--;
        }
        if (endDate->month > 12) {
            endDate->month = 1;
            endDate->year++;
        }
        startDate->day = monthSize(startDate->month, t->tm_year+1900) + startDate->day;
    }
    return 0;
}
/* * * End of function to return date from a string * * */

/* * * Function to return date from a string * * */
int whatDateIs(char * userInput, struct Dates * startDate, struct Dates * endDate) {
    int divider = countDivider(userInput);
    if (divider != 2) return -1;
    userInput = strtok(userInput, ".:");
    startDate->day = atoi(userInput);
    userInput = strtok(NULL, ".:");
    startDate->month = atoi(userInput);
    userInput = strtok(NULL, "");
    startDate->year = atoi(userInput);
    *endDate = *startDate;
    return 0;
}
/* * * End of function to return date from a string * * */

/* * * Function to return time from a string * * */
int whatTimeIs(char * userInput, struct Time * showTime) {
    int divider = countDivider(userInput);
    if (divider != 1) return -1;
    userInput = strtok(userInput, ".:");
    showTime->hours = atoi(userInput);
    userInput = strtok(NULL, "");
    showTime->minutes = atoi(userInput);
    return 0;
}
/* * * End of function to return time from a string * * */

/* * * Count dividers function * * */
int countDivider (string userInput) {
    int divider = 0;
    int i = 0;
    for (i = 0; (i < strlen(userInput)); i++) {
        if ((userInput[i] == ':') || (userInput[i] == '.')) {
            divider++;
        }
    }
    return divider;
}
/* * * End of count dividers in a string function * * */

/* * * Function to return month number * * */
int monthNo (string userInput) {
    int i = 0;
    string monthName[12] = { "January", "February", "March", "April", "May", "June",
                    "July", "August", "September", "October", "November", "December" };
    while (strncmp(userInput, monthName[i], 3) != 0) {
        if ((i == 11) && (strncmp(userInput, monthName[i], 3) != 0)) {
            return 0;
        }
        i++;
    }
    return i+1;
}
/* * * Function to return month number * * */

/* * * Function to return month size * * */
int monthSize (int monthNo, int year) {
    int monthSize[12] = { 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    monthSize[1] = checkIfLeapYear(year);
    return monthSize[monthNo-1];
}
/* * * Function to return month size * * */

/* * * Func to find a word in a string * * */
string wordInStr (string userInput) {
    string tempRecType = malloc(sizeof(char) * 50);
    if(strstr(userInput, "date") != NULL)
        tempRecType = strstr(userInput, "date");
    else if(strstr(userInput, "time") != NULL)
        tempRecType = strstr(userInput, "time");
    else if(strstr(userInput, "name") != NULL)
        tempRecType = strstr(userInput, "name");
    else if(strstr(userInput, "type") != NULL)
        tempRecType = strstr(userInput, "type");
    else
        strcpy(tempRecType, "no");
    return tempRecType;
}
/* * * End of find a word in a string function * * */

/* * * Function to parce string from file * * */
struct Database parcing(string inputText) {
    struct Database recTemp;
    inputText = strtok(inputText, ";");
    recTemp.number = atoi(inputText);
    inputText = strtok(NULL, ";");
    recTemp.type = inputText[0];
    inputText = strtok(NULL, ";");
    recTemp.status = inputText[0];
    inputText = strtok(NULL, ";");
    recTemp.day = atoi(inputText);
    inputText = strtok(NULL, ";");
    recTemp.month = atoi(inputText);
    inputText = strtok(NULL, ";");
    recTemp.year = atoi(inputText);
    inputText = strtok(NULL, ";");
    recTemp.hours = atoi(inputText);
    inputText = strtok(NULL, "*");
    recTemp.minutes = atoi(inputText);
    inputText = strtok(NULL, "~");
    strcpy(recTemp.name, inputText);
    return recTemp;
}
/* * * End of parce from file function * * */

/* * * Function returns char - type of the recording * * */
char recordingType(string temp) {
    char type;
    if (strcmp(temp, "task") == 0) {
        type = '1';
    } else if (strcmp(temp, "event") == 0) {
        type = '2';
    } else if (strcmp(temp, "birthday") == 0) {
        type = '3';
    } else {
        type = '0';
    }
    return type;
}
/* * * End of type of the recording function * * */

/* * * Function to save entered events to .txt file * * */
void saveEventsToFile(struct Database recTemp) {
    FILE* f;
    f = fopen ("data.txt", "a++");
    fprintf(f, "%d;", recTemp.number);
    fprintf(f, "%c;", recTemp.type);
    fprintf(f, "%c;", recTemp.status);
    fprintf(f, "%d;", recTemp.day);
    fprintf(f, "%d;", recTemp.month);
    fprintf(f, "%d;", recTemp.year);
    fprintf(f, "%d;", recTemp.hours);
    fprintf(f, "%d*", recTemp.minutes);
    fprintf(f, "%s~\n", recTemp.name);
    fclose(f);
}
/* * * end of Save events to file function * * */

/* * * Print changed / created / deleted struct function * * */
void printRecordingInfo (struct Database recTemp, char action) {

    if (recTemp.type == '1') {
        printf("  Task     ");
    } else if (recTemp.type == '2') {
        printf("  Event    ");
    } else if (recTemp.type == '3') {
        printf("  Birthday ");
    }

    printf("ID %d ", recTemp.number);

    if (recTemp.day != 0) {
        printf("%d.%d.%d ", recTemp.day, recTemp.month, recTemp.year);
    }

    if (recTemp.hours != 0 || recTemp.minutes != 0) {
        if (recTemp.hours == 0 || recTemp.hours < 10) {
            printf("0");
        }

        printf("%d:", recTemp.hours);

        if (recTemp.minutes == 0 || recTemp.minutes < 10) {
            printf("0");
        }

        printf("%d ", recTemp.minutes);

    }

    printf("%s ", recTemp.name);

    if (action == '1') {
        printf("created\n");
    } else if (action == '2') {
        printf("deleted\n");
    } else if (action == '3') {
        printf("updated\n");
    } else {
        printf("\n");
    }
}
/* * * End of function to print changed / created / deleted record * * */

/* * * Find the first day of a month * * */
int firstDayOfMonth(int forMonth, int forYear) {

    if (forMonth > 2) {
        forMonth -= 2;
    } else {
        forMonth += 10, forYear--;
    }

    int Century = forYear / 100;
    forYear %= 100;

    int startDay = (1 + (13 * forMonth - 1) / 5 + forYear + forYear / 4 - Century * 2 + Century / 4) % 7;

    if (startDay < 0) {
        return startDay += 7;
    } else {
        return startDay;
    }
}
/* * * End of find the first day of a month function * * */

/* * * Check if leap year function * * */
int checkIfLeapYear(int year) {
    if ((year % 4) == 0 && year % 100 != 0 || year % 400 == 0) return 29;
    else return 28;
}
/* * * End of check if leap year function * * */

/* * * Function to print calendar * * */
void printCalendar (int i, int year, int dates[], int arrSize) {

    int j = 0;
    int weekNo = 0;
    int weekspass = 0;
    int monthSize[12] = { 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    monthSize[1] = checkIfLeapYear(year);

    string monthName[12] = { "January", "February", "March", "April", "May", "June",
                    "July", "August", "September", "October", "November", "December" }; // Months' names
    string weekDay[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" }; // Weeks' names
    int t = i;

    for (i = 0; i < arrSize; i++) {
        for (j = arrSize - 1; j > i; j--) {
            if (dates[j] < dates[j-1]) {
                int temp;
                temp = dates[j - 1];
                dates[j - 1] = dates[j];
                dates[j] = temp;
            }
        }
    }

    i = t;


    int dayspass = 0; //eiee?anoai aiae iaaaee, eioi?ua io?ii i?iionoeou

    if (t > 0) { // n?eoaai aaiu ia?aea iaaaee
        for (i = 0; i < t; i++) {
            dayspass = dayspass + monthSize[i];
        }
    }

    weekNo = (dayspass + firstDayOfMonth(1, year)) / 7; // ii?aaaeyai ia?aeuiia cia?aiea iiia?a iaaaee aey aua?aiiai aiaa

    dayspass = firstDayOfMonth(t+1, year); // ii?aaaeyai ia?aue aaiu iaaaee aua?aiiai ianyoa
    int passFirstMonth = firstDayOfMonth(1, year);

    weekspass = (dayspass + monthSize[t]) / 7; // ii?aaaeyai, neieuei iaaaeu auaiaeou ia ye?ai

    printf("\n\t%s ", monthName[i]);
    printf("%d\n\n", year); // Print month name and chosen year
    printf("  #\t");

    i = 0;
    for (i = 0; i < 7; i++) { // print names of weekdays
        printf("%s\t", weekDay[i]);
    }

    printf("\n\n");

    int k = 0;
    i = 0;
    j = 0;
    int l = 0;
    int y = 0;

    for (i = weekNo; i <= weekNo + weekspass && j < monthSize[t]; i++) { // Print month days box
        if (k < monthSize[t]) {
            if (passFirstMonth > 3 && passFirstMonth != 7) {
                if (t == 0) {
                    printf("  53\t");
                } else {
                    printf("%3d\t", i);
                }
                passFirstMonth = 7;
            }
            else if (passFirstMonth == 7) {
                printf("%3d\t", i);
            } else {
                printf("%3d\t", i+1);
            }
        }
            for (j = 0; j < 7 && k < monthSize[t]; ++j) { // Print monthdays
                if (dayspass > 0) {     // Check from which weekday start printing days
                    printf("\t");
                    dayspass--;
                }
                else {
                    if (k+1 == dates[l]) {
                        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                        printf("%3d\t", ++k);
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                        for (y = l+1; y < arrSize; y++) {
                            if (dates[l] == dates[y])
                                l++;
                            else
                                break;
                        }
                        l++;
                    }

                    else {
                        printf("%3d\t", ++k); // print days in usual way
                    }
                }
            }
        printf("\n");
    }
    printf("\n");
}
/* * * End of print calendar function * * */
