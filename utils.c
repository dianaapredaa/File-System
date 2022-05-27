// Copyright 2021 - 2022 Omer Tarik Ilhan & Preda Diana 314CA
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

void free_node(TreeNode* node)
{
    // if node is null, something went wrong probably
    // or we are at the end of the tree
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

TreeNode* get_path(TreeNode* currentNode, char* path)
{
    if (currentNode->type == FILE_NODE)
        return currentNode;

    // buffer for each folder that we jump into
    char* buff;
    // get len of path and init current len
    int len = strlen(path);
    int curr_len = 0;
    // get the first folder we dive into
    buff = strtok(path, "/\n");
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
            puts("bad folder .. or file?");
            return currentNode;
        }
        ListNode* node = folder_content->children->head;
        if (!node || !node->info)
            return NULL;

        // iterate until we find desired folder
        // or until we run out of folders
        while (node && strcmp(buff, node->info->name))
            node = node->next;

        if (!node || !node->info)
            return NULL;

        // go to next node
        currentNode = node->info;

        if (node->info->type == FILE_NODE &&
            len - curr_len <= (int)strlen(buff))
            return node->info;

        // update current len
        curr_len += strlen(buff) + 1;
        // get next folder
        buff = strtok(NULL, "/");
    }
    return currentNode;
}
