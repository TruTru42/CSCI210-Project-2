#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){

    // Error when no path is provided
    if (strcmp(pathName, "") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[128];
    char dirName[128];
    struct NODE* parent = splitPath(pathName, baseName, dirName);

    // Error when directory can't be found
    if (parent == NULL) {
        return;
    }

    // Child to check if the directory already exists
    struct NODE* child = parent->childPtr;
    while (child != NULL) {
        // Error when a matching name is found
        if (strcmp(child->name, baseName) == 0 && child->fileType == 'D') {
            printf("MKDIR ERROR: directory '%s' already exists\n", baseName);
            return;
        }
        child = child->siblingPtr;
    }

    // New Node
    struct NODE* newNode = malloc(sizeof(struct NODE));
    strcpy(newNode->name, baseName);
    newNode->fileType = 'D';
    newNode->parentPtr = parent;
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;

    // Checks if the parent directory has any children
    if (parent->childPtr == NULL) {
        parent->childPtr = newNode;
    }
    // If the parent directory already does, loops through the children until the last child and adds the new node there
    else {
        struct NODE* last = parent->childPtr;
        while (last->siblingPtr != NULL) {
            last = last->siblingPtr;
        }
        last->siblingPtr = newNode;
    }
    // Success Message
    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);

    // Function complete
    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){
    // Temporary buffer
    char temp[128];
    strcpy(temp, pathName);

    // Find last slash
    char *lastslash = strrchr(temp, '/');

    // Edge case where pathName is in form of '/abc'
    if (lastslash == temp) {
        strcpy(baseName, lastslash + 1);
        strcpy(dirName, "/");
    }
    // Sets baseName and dirName
    else if (lastslash != NULL) {
        strcpy(baseName, lastslash + 1);
        *lastslash = '\0';
        strcpy(dirName, temp);
    }
    // Edge case where pathName is only target
    else {
        strcpy(baseName, temp);
        strcpy(dirName, "");
    }

    // Node structure to return
    struct NODE* current;
    if (pathName[0] == '/') {
        current = root;
    }
    else {
        current = cwd;
    }

    // Token
    char *token = strtok(dirName, "/");

    // Traverses dirName
    while (token != NULL) {
        // Child pointer to traverse with
        struct NODE* child = current->childPtr;

        // Stores whether or not we have found the child-token pair
        int found = 0;

        // Traverses through the children until it finds the token
        while (child != NULL) {
            if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                current = child;
                found = 1;
                break;
            }
            child = child->siblingPtr;
        }

        // Error when there is no matching token
        if (!found) {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }

        // Gets the next token
        token = strtok(NULL, "/");
    }

    // Function complete
    return current;
}
