/*
TODO:
- Edit function.
*/

// Headers
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --------------------------------------------------------------------------------------------------------------------

// Constants

#define array_size 26
#define kilobyte 1024
#define max_input_characters 200

//  --------------------------------------------------------------------------------------------------------------------

// Custom types

typedef struct book
{
    bool is_root;
    char title[max_input_characters];
    char author[max_input_characters];
    int pages;
    int date;
    struct book *next_book;
    struct book *prev_book;
} book;

typedef struct
{
    bool found;
    struct book *book_pointer;
} search_object;

// --------------------------------------------------------------------------------------------------------------------

// Signatures

int get_int(char *prompt);
int hash(char *book_name);

search_object search(char *search_term, book **book_array, bool search);
search_object search_branch(char *search_term, book *root);

void add_book(book **book_array);
void clear_branch(book *branch);
void clear_memory(book **book_array);
void delete_book(book **book_array);
void get_string(char *message, char *string_array);
void populate_array(book **book_array);
void print_tree(book **book_array);
void print_branch(book *book);

// --------------------------------------------------------------------------------------------------------------------

int main()
{

    // Create the base array.
    book **book_array = malloc(sizeof(book) * array_size);
    if (book_array == NULL)
    {
        printf("There was an error creating the book array");
        return 1;
    }

    // Create roots
    populate_array(book_array);

    // Take input
    add_book(book_array);
    add_book(book_array);

    // char *names[max_input_characters] = {"Apple", "Band", "Oranges"};

    // // Hard coding a few of items for debugging.
    // for (int i = 0; i < 3; i++)
    // {
    //     int index = hash(names[i]);
    //     book *new_book = malloc(sizeof(book));
    //     new_book->title = names[i];
    //     new_book->author = names[i];
    //     new_book->date = 1;
    //     new_book->pages = 1;
    //     new_book->is_root = false;
    //     new_book->next_book = NULL;
    //     new_book->prev_book = book_array[index];

    //     book_array[index]->next_book = new_book;
    // }

    // Print the contents of the tree.
    print_tree(book_array);

    // Search the tree for an object
    char search_string[max_input_characters];
    get_string("\nBook title", search_string);
    search_object search_results = search(search_string, book_array, true);

    // Delete a book
    delete_book(book_array);

    // Print tree after the deletion
    print_tree(book_array);

    // Free memory
    clear_memory(book_array);
    free(book_array);
}

// --------------------------------------------------------------------------------------------------------------------

// Hash function

int hash(char *book_name)
{
    return (int)toupper(book_name[0]) - (int)'A';
}

// --------------------------------------------------------------------------------------------------------------------

// Adds books to the hash tree.

void populate_array(book **book_array)
// Populates arrays with root books.
{

    for (int i = 0; i < array_size; i++)
    {
        book_array[i] = malloc(sizeof(book));
        book_array[i]->is_root = true;
        book_array[i]->next_book = NULL;
        book_array[i]->prev_book = NULL;
    }
}

void add_book(book **book_array)
// Take user input and add a book the array.
{
    // Take input
    char book_name[max_input_characters];
    get_string("Book Title", book_name);

    char book_author[max_input_characters];
    get_string("Author", book_author);

    int book_pages = get_int("Total Pages");
    if (book_pages == -1)
    {
        printf("There was an error taking input\n");
        return;
    }
    int book_date = get_int("Release Year");
    if (book_date == -1)
    {
        printf("There was an error taking input\n");
        return;
    }

    // Get hash
    int index = hash(book_name);

    // Create the new book using the user inputs.
    book *new_book = malloc(sizeof(book));
    if (new_book == NULL)
    {
        printf("There was an error creating a new book\n");
        return;
    }

    // Add data to the new book
    int title_length = strlen(book_name);
    int author_length = strlen(book_author);

    for (int i = 0; i < title_length; i++)
    {
        new_book->title[i] = book_name[i];
    }
    for (int i = 0; i < author_length; i++)
    {
        new_book->author[i] = book_author[i];
    }

    new_book->pages = book_pages;
    new_book->date = book_date;

    // Set pointers
    if (book_array[index]->next_book == NULL)
    {
        book_array[index]->next_book = new_book;
        new_book->prev_book = book_array[index];
    }
    else
    {
        new_book->next_book = book_array[index]->next_book;
        new_book->prev_book = book_array[index];
        new_book->next_book->prev_book = new_book;
        book_array[index]->next_book = new_book;
    }

    return;
}

void get_string(char *message, char string_array[])
// Takes string input from the user.
{

    printf("%s:\n", message);
    if (fgets(string_array, max_input_characters, stdin))
    {
        // remove trailing newline
        string_array[strcspn(string_array, "\n")] = 0;
    }
    return;
}

int get_int(char *prompt)
// Takes integer user input.
{
    int user_input;
    char buffer[kilobyte];

    do
    {
        printf("%s:\n", prompt);

        // Error checking and taking input.
        if (!fgets(buffer, kilobyte, stdin))
        {
            printf("There was an error taking user input\n");
            return -1;
        }

        // Convert input to integer
        user_input = atoi(buffer);

    } while (user_input == 0);

    return user_input;
}

// --------------------------------------------------------------------------------------------------------------------

// Search tree

search_object search(char *search_term, book **book_array, bool search)
// Searches books saved in the tree. for title or author matches. Note the bool allows us to reuse this function in other functions.
{
    int index = hash(search_term);
    search_object results = search_branch(search_term, book_array[index]);

    // Print results
    if (results.found && search)
    {
        printf("\nBook Found:\n");
        for (int i = 0; i < max_input_characters; i++)
        {
            if (results.book_pointer->title[i] == 0)
            {
                break;
            }
            printf("%c", results.book_pointer->title[i]);
        }
        printf("\n");
    }
    else if (search)
    {
        printf("No book found\n");
    }

    return results;
}

search_object search_branch(char *search_term, book *root)
// Searches a branch of the hash tree.
{
    search_object results;

    // Base cases. If we find the book or we don't.
    if (strcmp(root->title, search_term) == 0)
    {
        results.book_pointer = root;
        results.found = true;
        return results;
    }
    else if (root->next_book == NULL)
    {
        results.found = false;
        return results;
    }

    // Search the next branch element.
    results = search_branch(search_term, root->next_book);
    return results;
}

// --------------------------------------------------------------------------------------------------------------------

// Print Tree

void print_tree(book **book_array)
// Iterates over each root of the array and calls the helper function.
{
    for (int i = 0; i < array_size; i++)
    {
        print_branch(book_array[i]);
    }
}

void print_branch(book *book)
// Recursively prints the branches of the tree.
{
    // Skip root which doesn't contain any relevant information.
    if (!book->is_root)
    {
        printf("\nTitle: %s\nAuthor: %s\nPages: %d\nRelease Date: %d\n",
               book->title, book->author, book->pages, book->date);
    }
    // Base case.
    if (book->next_book == NULL)
    {
        return;
    }
    print_branch(book->next_book);
    return;
}

// --------------------------------------------------------------------------------------------------------------------

// Clear memory

void clear_memory(book **book_array)
// Calls helper function to free branches for each root in the array.
{
    for (int i = 0; i < array_size; i++)
    {
        clear_branch(book_array[i]);
    }
}

void clear_branch(book *branch)
// Recursive function that clears a branch of the array.
{
    // Base case.
    if (branch->next_book == NULL)
    {
        free(branch);
        return;
    }
    clear_branch(branch->next_book);
    free(branch);
    return;
}

void delete_book(book **book_array)
// Deletes a singular book from the array
{
    // Get the name of the book the user wants to delete.
    char search_string[max_input_characters];
    get_string("Enter the name of the book you want to delete", search_string);

    search_object result = search(search_string, book_array, false);

    // If we find the book set pointers and free memory.
    if (result.found)
    {
        if (result.book_pointer->next_book == NULL)
        {
            result.book_pointer->prev_book->next_book = NULL;
        }
        else
        {
            result.book_pointer->next_book->prev_book = result.book_pointer->prev_book;
            result.book_pointer->prev_book->next_book = result.book_pointer->next_book;
        }
        // free(result.book_pointer->author);
        // free(result.book_pointer->title);
        free(result.book_pointer);
        return;
    }
    // If we don't notify user.
    else
    {
        printf("No book with that title found\n");
    }
    return;
}