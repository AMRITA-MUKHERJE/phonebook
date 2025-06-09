#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for contact
typedef struct Contact {
    char name[50];
    char phone[15];
    struct Contact *left, *right;
} Contact;

// Function to create a new contact node
Contact *createContact(char *name, char *phone) {
    Contact *newContact = (Contact *)malloc(sizeof(Contact));
    strcpy(newContact->name, name);
    strcpy(newContact->phone, phone);
    newContact->left = newContact->right = NULL;
    return newContact;
}

// Function to insert a new contact into the phonebook
Contact *insert(Contact *root, char *name, char *phone) {
    if (root == NULL) {
        return createContact(name, phone);
    }

    if (strcmp(name, root->name) < 0) {
        root->left = insert(root->left, name, phone);
    } else if (strcmp(name, root->name) > 0) {
        root->right = insert(root->right, name, phone);
    }

    return root;
}

// Function to search for a contact by name
Contact *search(Contact *root, char *name) {
    if (root == NULL || strcmp(root->name, name) == 0) {
        return root;
    }

    if (strcmp(name, root->name) < 0) {
        return search(root->left, name);
    } else {
        return search(root->right, name);
    }
}

// Function to perform inorder traversal of the phonebook
void display(Contact *root) {
    if (root != NULL) {
        display(root->left);
        printf("%s: %s\n", root->name, root->phone);
        display(root->right);
    }
}

// Function to find the minimum node in a BST (used for deleting a node with two children)
Contact *findMin(Contact *node) {
    Contact *current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

// Function to delete a contact from the phonebook
Contact *deleteContact(Contact *root, char *name) {
    if (root == NULL) {
        return root;
    }

    if (strcmp(name, root->name) < 0) {
        root->left = deleteContact(root->left, name);
    } else if (strcmp(name, root->name) > 0) {
        root->right = deleteContact(root->right, name);
    } else {
        if (root->left == NULL) {
            Contact *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Contact *temp = root->left;
            free(root);
            return temp;
        }

        Contact *temp = findMin(root->right);
        strcpy(root->name, temp->name);
        strcpy(root->phone, temp->phone);
        root->right = deleteContact(root->right, temp->name);
    }
    return root;
}

// Function to save contacts to a file
void saveToFile(Contact *root, FILE *file) {
    if (root != NULL) {
        saveToFile(root->left, file);
        fprintf(file, "%s %s\n", root->name, root->phone);
        saveToFile(root->right, file);
    }
}

// Function to load contacts from a file
Contact *loadFromFile(FILE *file) {
    Contact *root = NULL;
    char name[50];
    char phone[15];

    while (fscanf(file, "%s %s\n", name, phone) != EOF) {
        root = insert(root, name, phone);
    }

    return root;
}

int main() {
    Contact *root = NULL;
    FILE *file = fopen("phonebook.txt", "a");
    if (file != NULL) {
        root = loadFromFile(file);
        fclose(file);
    }

    int choice;
    char name[50], phone[15];

    do {
        printf("\nPhonebook Menu:\n");
        printf("1. Add Contact\n");
        printf("2. Search Contact\n");
        printf("3. Delete Contact\n");
        printf("4. Display All Contacts\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter contact name: ");
                scanf("%s", name);
                printf("Enter contact phone number: ");
                scanf("%s", phone);
                root = insert(root, name, phone);

                file = fopen("phonebook.txt", "a");
                if (file != NULL) {
                    saveToFile(root, file);
                    fclose(file);
                    printf("Contacts saved to file.\n");
                } else {
                    printf("Error saving contacts to file.\n");
                }
                break;
            case 2:
                printf("Enter contact name to search: ");
                scanf("%s", name);
                Contact *found = search(root, name);
                if (found != NULL) {
                    printf("Contact found: %s: %s\n", found->name, found->phone);
                } else {
                    printf("Contact not found.\n");
                }
                break;
            case 3:
                printf("Enter contact name to delete: ");
                scanf("%s", name);
                root = deleteContact(root, name);
                printf("After deletion:\n");
                display(root);
                break;
            case 4:
                printf("Phonebook entries:\n");
                display(root);
                break;
               
            case 5:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}
