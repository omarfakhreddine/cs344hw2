// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

// Instructions on how to run this program is in the README.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* struct for movie information */
struct movie
{
    char* title;
    int year;
    char* languages;
    double rating;
    struct movie* next;
};

/* Parse the current line which is space delimited and create a
*  movie struct with the data in this line
*/
struct movie* createMovie(char* currLine)
{
    struct movie* currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char* saveptr;

    // The first token is the title
    char* token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // The next token is the year
    token = strtok_r(NULL, ",", &saveptr);
    int year = atoi(token);
    currMovie->year = year;

    // The next token is the languages
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->languages = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->languages, token);

    // The last token is the rating
    token = strtok_r(NULL, "\n", &saveptr);
    double rating = atof(token);
    currMovie->rating = rating;

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;

    return currMovie;
}

/*
* Return a linked list of movies by parsing data from
* each line of the specified file.
*/
struct movie* processFile(char* filePath, int *quantity)
{
    // Open the specified file for reading only
    FILE* movieFile = fopen(filePath, "r");

    char* currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    // char* token;

    // The head of the linked list
    struct movie* head = NULL;
    // The tail of the linked list
    struct movie* tail = NULL;

    // Skip the headers of the csv
    getline(&currLine, &len, movieFile);

    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        // Get a new movie node corresponding to the current line
        struct movie* newNode = createMovie(currLine);

        // Is this the first node in the linked list?
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
            *quantity = 1;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
            *quantity = *quantity + 1;
        }
    }
    free(currLine);
    fclose(movieFile);
    return head;
}

/*
* Print data for the given Movie
*/
void printMovie(struct movie* aMovie) {
    printf("%s, %d, %s, %.1f\n", aMovie->title,
        aMovie->year,
        aMovie->languages,
        aMovie->rating);
}
/*
* Print the linked list of Movie
*/
void printMovieList(struct movie* list)
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}

/*
* Print titles of movies in the year specified
*/
void moviesInYear(struct movie* list, int year)
{
    bool match = false;

    // Iterate through movies and check if year matches
    while (list != NULL) {
        if (list->year == year) {
            printf("%s\n", list->title);
            match = true;
        }
        list = list->next;
    }

    // If no matches were found
    if (!match) {
        printf("No data about movies released in the year %d\n", year);
    }
}

/*
* Print titles/rating/year of movies with the highest rating in their year
*/
void highestRated(struct movie* list)
{
    // Store unique year movies
    struct movie* highest[121];

    for (int year = 1900; year <= 2021; year++) {
        struct movie* head = list;
        // Iterate through movies and check if year matches
        while (head != NULL) {
            if (year == head->year) {
                if (highest[1] != NULL) {
                    if (head->rating > highest[year - 1900]->rating) {
                        highest[year - 1900] = head;
                    } 
                } else {
                    highest[year - 1900] = head;
                }
            }
            head = head->next;
        }
    }
    for (int i = 0; i <= 121; i++) {
        if (highest[i] != NULL) {
            printf("%d %.1f %s\n", highest[i]->year,
                highest[i]->rating,
                highest[i]->title);
        }
    }

}

/*
* Provide selection of files to process
*/
void selectFile()
{
    int selection = 0;
    char* selection1 = "\nEnter 1 to pick the largest file";
    char* selection2 = "Enter 2 to pick the smallest file";
    char* selection3 = "Enter 3 to specify the name of a file";
    char* selectionError = "You entered an incorrect selection, try again by selecting 1, 2, or 3";

    while (selection == 0) {
        printf("%s\n%s\n%s\n\n", selection1, selection2, selection3);
        printf("Which kind of file do you want to process?");
        scanf("%d", &selection);

        switch (selection) {
        case 1:;
            printf("1");
            break;
        case 2:
            printf("2");
            break;
        case 3:
            printf("3");
            break;
        default:
            // Invalid selection entered, retry selection entry
            printf("%s\n\n", selectionError);
            selection = 0;
            break;
        }
    }
}

/*
*   Provide a selection of files to process 
*   Compile the program as follows:
*       gcc --std=gnu99 -o movies_by_year main.c
*/

int main()
{
    //static int quantity = 0;
    static int choice = 0;
    char* option1 = "\n1. Select file to process";
    char* option2 = "2. Exit from the program";
    char* optionError = "You entered an incorrect choice, try again by selecting 1 or 2";

    //if (argc < 2)
    //{
    //    // If no file provided
    //    printf("You must provide the name of the file to process\n");
    //    printf("Example usage: ./movie_by_year movie_sample.csv\n");
    //    return EXIT_FAILURE;
    //}
    //struct movie* list = processFile(argv[1], &quantity);
    //printf("Processed file %s and parsed data for %d movies\n", argv[1], quantity);

    // Option loop to ask user what information they want to see
    while (choice != 2) {
        printf("%s\n%s\n\n", option1, option2);
        printf("Enter a choice between 1 or 2: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:;
            selectFile();
            break;
        case 2:
            return EXIT_SUCCESS;
            break;
        default:
            // Invalid option entered, retry option entry
            printf("%s\n\n", optionError);
            break;
        }
    }
    return EXIT_SUCCESS;
}
