#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."


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
    // TODO
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
                ListNode* trash_node = list_node;
                content->children->head = trash_node->next;
                free_node(trash_node->info);
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
    // TODO
}


void pwd(TreeNode* treeNode) {
    // TODO
}


TreeNode* cd(TreeNode* currentNode, char* path) {
    // TODO
}


void tree(TreeNode* currentNode, char* arg) {
    // TODO
}


void mkdir(TreeNode* currentNode, char* folderName) {
    // TODO
    // get the content of current folder
    FolderContent* content = currentNode->content;
    List* content_list = content->children;
    // maloc new folder node
    ListNode* new_folder_node = malloc(sizeof(ListNode));
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
    // TODO
}


void cp(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}

void mv(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}

