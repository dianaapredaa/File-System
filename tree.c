#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."
#define MAX_STRING_SIZE 64

FileTree createFileTree(char* rootFolderName) {
    // malloc file tree struct
    FileTree file_tree;
    file_tree.root = malloc(sizeof(TreeNode));
    // root has no parents
    file_tree.root->parent = NULL;
    // root name is given as a parameter
    file_tree.root->name = rootFolderName;
    // root is a folder
    file_tree.root->type = FOLDER_NODE;
    // malloc size of folder content
    file_tree.root->content = malloc(sizeof(FolderContent));
    // when created, root folder content is empty
    FolderContent* content = file_tree.root->content;
    content->children = malloc(sizeof(List));
    content->children->head = NULL;

    return file_tree;
}

void free_node(TreeNode* node);

void freeTree(FileTree fileTree) {
    // get root node
    TreeNode* node = fileTree.root;
    // if empty, we are done
    if (node == NULL || node->content == NULL)
        return;
    free_node(node);
}

void free_node(TreeNode* node)
{
    // if node is null, something went wrong probably
    if (node == NULL)
        return;
    // if node is a file
    if (node->type == FILE_NODE && node->content != NULL) {
        // free name, text and content
        FileContent* content = node->content;
        free(content->text);
    }
    // if node is a folder
    if (node->type == FOLDER_NODE && node->content != NULL) {
        // get the content
        FolderContent* content = node->content;
        if (content->children != NULL) {
            // if not empty, start removing and freeing every tree node
            ListNode* list_node = content->children->head;
            // remove and free until list is empty, iterative + recursive
            while (content->children->head != NULL) {
                ListNode* trash_node = content->children->head;
                content->children->head = content->children->head->next;
                trash_node->next = NULL;
                free_node(trash_node->info);
                free(trash_node);
            }
            // free the children list itself, content and name
            free(content->children);
        }
    }
    // free tree node itself
    free(node->content);
    free(node->name);
    free(node);
}


void ls(TreeNode* currentNode, char* arg) {
    // if currentNode is NULL, something went wrong
    if (currentNode == NULL) {
        puts("Oops.. Something went horribly wrong.");
        return;
    }

    if (currentNode->type == FILE_NODE) {
        puts("tf?");
        return;
    }

    FolderContent* children_content = currentNode->content;
    // check for NULL pointers
    if (children_content == NULL) {
        puts("Damn bro.. that sucks..");
        return;
    }
    List* children = children_content->children;
    if (children == NULL) {
        puts("Pls stop..");
        return;
    }
    ListNode* node = children->head;
    if (node == NULL && !strcmp(arg, NO_ARG)) {
        return;
    } else if (node == NULL) {
        printf("ls: cannot access '%s': No such file or directory", arg);
        return;
    }

    // if command has no arguments, print everything in current folder
    if (strcmp(arg, NO_ARG) == 0) {
        // iterate through Folder node children, print name of all children
        while (node) {
            TreeNode* treenode = node->info;
            // if node is NULL, something is wrong
            if (treenode == NULL) {
                puts(":/");
                return;
            }
            if (children == NULL) {
                puts("Problems");
                return;
            }   

            printf("%s\n", treenode->name);
            // get next node
            node = node->next;
        }
    } else {
        // iterate until given file / folder
        while (node && node->info && strcmp(arg, node->info->name)) {
            node = node->next;
        }
        // if node is NULL, the file / folder does not exist
        if (node == NULL) {
            printf("ls: cannot access '%s': No such file or directory", arg);
            return;
        }
        // if we get here, file / folder exists
        TreeNode* treenode = node->info;

        if (treenode->type == FILE_NODE) {
            // is a file
            FileContent* file_content = treenode->content;
            // check for bad file
            if (!file_content || !file_content->text) {
                puts("Bad file");
                return;
            }
            printf("%s: %s\n", node->info->name, file_content->text);
        } else if (treenode->type == FOLDER_NODE) {
            // is a folder
            FolderContent* folder_content = treenode->content;
            List* children = folder_content->children;
            // check for bad folder
            if (children == NULL) {
                puts("Bad folder");
                return;
            }
            ListNode* node = children->head;
            // iterate through all children of given folder
            while (node && node->info && node->info->name) {
                printf("%s\n", node->info->name);
                node = node->next;
            }
        }
    }
}


void pwd(TreeNode* treeNode) {
    // TODO
}


TreeNode* cd(TreeNode* currentNode, char* path) {
    // keep current node
    TreeNode* pastNode = currentNode;
    // check for NULLs
    if (!currentNode || !currentNode->content) {
        puts("hmm");
        return currentNode;
    }
    if (!strcmp(path, "..")) {
        // change directory to parent
        // check for NULLs
        if (!currentNode->parent) {
            puts("You are in root");
            return currentNode;
        }
        return currentNode->parent;
    } else {
        // buffer for each folder that we jump into
        char* buff;
        // get len of path and init current len
        int len = strlen(path);
        int curr_len = 0;
        // get the first folder we dive into
        buff = strtok(path, "/");
        // we track current len, so it matches with path len
        // so we know when to stop, without crashing bcs of strtok
        while (curr_len < len) {
            // change folder
            if (!strcmp(buff, "..")) {
                currentNode = currentNode->parent;
                curr_len += 3;
                buff = strtok(NULL, "/\n");
                continue;
            }
            // check for bad folder
            FolderContent* folder_content = currentNode->content;
            if (folder_content->children == NULL) {
                puts("bad folder");
                return currentNode;
            }
            ListNode* node = folder_content->children->head;
            // iterate until we find correct folder to switch into
            if (node == NULL) {
                // case: empty folder
                printf("cd: no such file or directory: %s\n", path);
                return pastNode;
            }
            while (strcmp(buff, node->info->name)) {
                if (node == NULL) {
                    printf("cd: no such file or directory: %s\n", path);
                    return pastNode;
                }
                node = node->next;
            }
            // at this point, in 'node' we have the folder we must go into
            if (node->info->type == FOLDER_NODE) {
                currentNode = node->info; 
            } else {
                // found node is a file
                puts("That's a file bro");
                return pastNode;
            }
            // update current len
            curr_len += strlen(buff) + 1;
            // get next folder
            buff = strtok(NULL, "/");
        }
        return currentNode;
    }
}


void tree(TreeNode* currentNode, char* arg) {
    // TODO
}


void mkdir(TreeNode* currentNode, char* folderName) {
    // get the content of current folder
    FolderContent* content = currentNode->content;
    List* content_list = content->children;

    // check existence
    ListNode* it = content_list->head;
    while (it) {
        if (!strcmp(folderName, it->info->name)) {
            // folder already exists
            printf("mkdir: cannot create directory '%s': File exists\n", folderName);
            return;
        }
        it = it->next;
    }

    // maloc new folder node
    ListNode* new_folder_node = malloc(sizeof(ListNode));
    new_folder_node->next = NULL;

    // malloc new folder
    new_folder_node->info = malloc(sizeof(TreeNode));
    TreeNode* info = new_folder_node->info;

    // set info about new folder (name, parent node, type)
    info->name = folderName;
    info->parent = currentNode;
    info->type = FOLDER_NODE;
    
    // malloc folder content (list that stores children nodes)
    info->content = malloc(sizeof(FolderContent));
    FolderContent* f_content = info->content;
    f_content->children = malloc(sizeof(List));
    f_content->children->head = NULL;

    // add folder to children of current node
    if (content_list->head == NULL) {
        content_list->head = new_folder_node;
    } else {
        new_folder_node->next = content_list->head;
        content_list->head = new_folder_node;
    }
}


void rmrec(TreeNode* currentNode, char* resourceName) {
    // TODO
}


void rm(TreeNode* currentNode, char* fileName) {
    // TODO
}


void rmdir(TreeNode* currentNode, char* folderName) {
    // TODO
}


void touch(TreeNode* currentNode, char* fileName, char* fileContent) {
    // check for NULLs
    if (!currentNode || !currentNode->content) {
        puts("smth is wrong");
        return;
    }
    // check for bad folder
    FolderContent* folder_content = currentNode->content;
    if (!folder_content->children) {
        puts("Bad folder");
        return;
    }
    
    // check existence
    List* children = folder_content->children;
    ListNode* it = children->head;
    while (it) {
        if (!strcmp(fileName, it->info->name)) {
            // folder already exists
            return;
        }
        it = it->next;
    }

    // create new file node
    // node part
    ListNode* new_file_node = malloc(sizeof(ListNode));
    new_file_node->info = malloc(sizeof(TreeNode));
    // filename and file details part
    new_file_node->info->name = fileName;
    new_file_node->info->parent = currentNode;
    new_file_node->info->type = FILE_NODE;
    // file content part
    FileContent* cont_struct = malloc(sizeof(FileContent));
    cont_struct->text = fileContent;
    new_file_node->info->content = cont_struct;


    if (children->head == NULL) {
        children->head = new_file_node;
        new_file_node->next = NULL;
    } else {
        new_file_node->next = children->head;
        children->head = new_file_node;
    }
}


void cp(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}

void mv(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}

