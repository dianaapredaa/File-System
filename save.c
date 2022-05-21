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

TreeNode* get_path(TreeNode* currentNode, char* path, int isDisplay) {
    TreeNode* pastNode = currentNode;
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
            puts("bad folder");
            return currentNode;
        }
        ListNode* node = folder_content->children->head;
        // iterate until we find correct folder to switch into
        if ((!node || !node->info) && isDisplay == 0) {
            // case: empty folder
            printf("cd: no such file or directory: %s\n", path);
            return pastNode;
        } else if ((!node || !node->info) && isDisplay == 1)   {
            // case: empty folder, but for displaying
            printf("%s [error opening dir]\n\n0 directories, 0 files\n", path);
            return pastNode;
        }
        // iterate until we find desired folder
        // or until we run out of folders
        while (node && strcmp(buff, node->info->name)) {
            if (node == NULL && isDisplay == 0) {
                printf("cd: no such file or directory: %s\n", path);
                return pastNode;
            } else if (node == NULL && isDisplay == 1) {
                printf("%s [error opening dir]\n\n0 directories, 0 files\n",
                       path);
                return pastNode;
            }
            node = node->next;
        }
        // if node is node Is NULL, then there's no such directory
        if (node == NULL && isDisplay == 0) {
            printf("cd: no such file or directory: %s\n", path);
            return pastNode;
        } else if (node == NULL && isDisplay == 1) {
            printf("%s [error opening dir]\n\n0 directories, 0 files\n", path);
            return pastNode;
        }
        // at this point, in 'node' we have the folder we must go into
        if (node->info->type == FOLDER_NODE) {
            currentNode = node->info;
        } else if (isDisplay == 1){
            // found node is a file
            printf("%s [error opening dir]\n\n0 directories, 0 files\n", path);
            return pastNode;
        } else {
            return pastNode;
        }
        // update current len
        curr_len += strlen(buff) + 1;
        // get next folder
        buff = strtok(NULL, "/");
    }
    return currentNode;
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
    // malloc path string
    char* path = malloc(MAX_STRING_SIZE);
    // get to root, get path in reverse
    memcpy(path, treeNode->name, strlen(treeNode->name) + 1);
    while (treeNode->parent) {
        treeNode = treeNode->parent;
        memcpy(path + strlen(treeNode->name) + 1, path, strlen(path) + 2);
        memcpy(path, treeNode->name, strlen(treeNode->name));
        memcpy(path + strlen(treeNode->name), "/", 1);
    }
    printf("%s\n", path);
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
        currentNode = get_path(currentNode, path, 0);
        return currentNode;
    }
}

void displayNode(TreeNode* currentNode, int tabs, int fd[2])
{
    if (currentNode == NULL || currentNode->content == NULL) {
        printf("Bad node\n");
        return;
    }

    if (currentNode->type == FILE_NODE) {
        printf("%s\n", currentNode->name);
    }

    if (currentNode->type == FOLDER_NODE) {
        FolderContent* folder_content = currentNode->content;
        if (folder_content == NULL || folder_content->children == NULL) {
            printf("Hmm..\n");
            return;
        }
        List* children = folder_content->children;
        ListNode* child = children->head;
        while (child) {
            for (int i = 0; i < tabs; i++) {
                printf("\t");
            }
            printf("%s\n", child->info->name);
            if (child->info->type == FILE_NODE) {
                fd[0]++;
            }
            if (child->info->type == FOLDER_NODE) {
                fd[1]++;
                displayNode(child->info, tabs + 1, fd);
            }
            child = child->next;
        }
    }
}

void tree(TreeNode* currentNode, char* arg)
{
    // if we have an argument, we move currentNode to given path
    // from there we can start displaying the tree hierarchy
    if (strcmp(arg, NO_ARG) != 0) {
        TreeNode* pastNode = currentNode;
        currentNode = get_path(currentNode, arg, 1);
        // if currentNode is the same as pastNode
        // folder probably doesn't exist / is a file
        // message was already display in get_path() function
        if (currentNode == pastNode)
            return;
    }
    int fd[2] = {0, 0};
    displayNode(currentNode, 0, fd);
    printf("%d directories, %d files\n", fd[1], fd[0]);
}


void mkdir(TreeNode* currentNode, char* folderName)
{
    // get the content of current folder
    FolderContent* content = currentNode->content;
    List* content_list = content->children;

    // check existence
    ListNode* it = content_list->head;
    while (it) {
        if (!strcmp(folderName, it->info->name)) {
            // folder already exists
            printf("mkdir: cannot create directory '%s': File exists\n",
                   folderName);
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


void rmrec(TreeNode* currentNode, char* resourceName)
{
    // check for NULLs
    if (currentNode == NULL) {
        printf("Bad node.\n");
        return;
    }

    FolderContent* folder_content = currentNode->content;

    if (folder_content->children == NULL) {
        printf("Bad folder.\n");
        return;
    }

    // get list, node to iterate and prev node
    List* children = folder_content->children;
    ListNode* child = children->head;
    ListNode* prev = NULL;

    // interate until end of the list or until we find desired node
    while (child) {
        if (strcmp(child->info->name, resourceName) == 0) {
            break;
        }
        prev = child;
        child = child->next;
    }

    // if node is NULL, desired node does not exist
    if (child == NULL) {
        printf("rmrec: failed to remove '%s': No such file or directory\n",
               resourceName);
        return;
    }

    TreeNode* Tree_Node = child->info;
    // free folder content
    free_node(Tree_Node);

    // if child is the only node in the list
    if (prev == NULL && child->next == NULL) {
        children->head = NULL;
    // if child is the first node
    } else if (prev == NULL) {
        children->head = child->next;
        child->next = NULL;
    // if child is anywhere else
    } else {
        prev->next = child->next;
        child->next = NULL;
    }

    free(child);
}


void rm(TreeNode* currentNode, char* fileName)
{
    // check for NULLs
    if (currentNode == NULL) {
        printf("Bad node.\n");
        return;
    }

    FolderContent* folder_content = currentNode->content;

    if (folder_content->children == NULL) {
        printf("Bad folder.\n");
        return;
    }

    // get list, node to iterate and prev node
    List* children = folder_content->children;
    ListNode* child = children->head;
    ListNode* prev = NULL;

    // interate until end of the list or until we find desired node
    while (child) {
        if (strcmp(child->info->name, fileName) == 0) {
            break;
        }
        prev = child;
        child = child->next;
    }

    // if node is NULL, desired node does not exist
    if (child == NULL) {
        printf("rm: failed to remove '%s': No such file or directory\n",
               fileName);
        return;
    }

    TreeNode* Tree_Node = child->info;

    // command is for files only
    if (Tree_Node->type == FOLDER_NODE) {
        printf("rm: cannot remove '%s': Is a directory\n", fileName);
        return;
    }

    // free file content
    free_node(Tree_Node);

    // if child is the only node in the list
    if (prev == NULL && child->next == NULL) {
        children->head = NULL;
    // if child is the first node
    } else if (prev == NULL) {
        children->head = child->next;
        child->next = NULL;
    // if child is anywhere else
    } else {
        prev->next = child->next;
        child->next = NULL;
    }

    free(child);
}


void rmdir(TreeNode* currentNode, char* folderName)
{
    // check for NULLs
    if (currentNode == NULL) {
        printf("Bad node.\n");
        return;
    }

    FolderContent* folder_content = currentNode->content;

    if (folder_content->children == NULL) {
        printf("Bad folder.\n");
        return;
    }

    // get list, node to iterate and prev node
    List* children = folder_content->children;
    ListNode* child = children->head;
    ListNode* prev = NULL;

    // interate until end of the list or until we find desired node
    while (child) {
        if (strcmp(child->info->name, folderName) == 0) {
            break;
        }
        prev = child;
        child = child->next;
    }

    // if node is NULL, desired node does not exist
    if (child == NULL) {
        printf("rmdir: failed to remove '%s': No such file or directory\n",
               folderName);
        return;
    }

    TreeNode* Tree_Node = child->info;

    // command is for folders only
    if (Tree_Node->type == FILE_NODE) {
        printf("rmdir: failed to remove '%s': Not a directory\n", folderName);
        return;
    }

    FolderContent *fc = Tree_Node->content;
    if (fc->children->head != NULL) {
        printf("rmdir: failed to remove '%s': Directory not empty\n",
               folderName);
        return;
    }

    // free folder content
    free_node(Tree_Node);

    // if child is the only node in the list
    if (prev == NULL && child->next == NULL) {
        children->head = NULL;
    // if child is the first node
    } else if (prev == NULL) {
        children->head = child->next;
        child->next = NULL;
    // if child is anywhere else
    } else {
        prev->next = child->next;
        child->next = NULL;
    }

    free(child);
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


void cp(TreeNode* currentNode, char* source, char* destination)
{
    // check for NULLs
    if (currentNode == NULL) {
        printf("Bad node.\n");
        return;
    }

    FolderContent* folder_content = currentNode->content;

    if (folder_content->children == NULL) {
        printf("Bad folder.\n");
        return;
    }

    // get list, node to iterate and prev node
    List* children = folder_content->children;
    ListNode* child = children->head;
    ListNode* prev = NULL;

    // interate until end of the list or until we find desired node
    while (child) {
        if (strcmp(child->info->name, source) == 0) {
            break;
        }
        prev = child;
        child = child->next;
    }

    // if node is NULL, desired node does not exist
    if (child == NULL) {
        printf("cp: failed to copy '%s': No such file or directory\n", source);
        return;
    } else if (child->info->type == FOLDER_NODE) {
        printf("cp: -r not specified; omitting directory '%s'", source);
        return;
    }

    TreeNode* source_node = child->info;

    char* newFileName =  source;

    char* buff = malloc(MAX_STRING_SIZE);
    buff = strtok(destination, "/");

    child = children->head;

    if (!strcmp(buff, "..")) {
        newFileName = source;
    }

    while (child && strcmp(child->info->name, buff)) {
        child = child->next;
    }

    int isFinal = 0;
    if (child && child->info->type == FOLDER_NODE) {
        currentNode = child->info;
    } else if (child == NULL) {
            newFileName = buff;
        isFinal = 1;
    }
    while ((buff = strtok(NULL, "/")) && (isFinal == 0)) {
        FolderContent* fc = currentNode->content;
        children = fc->children;
        child = children->head;

        if (!strcmp(buff, "..") && currentNode->parent) {
            newFileName = source;
            currentNode = currentNode->parent;
            continue;
        }

        ListNode* prev = NULL;
        while (child && strcmp(child->info->name, buff)) {
            prev = child;
            child = child->next;
        }

        if (child && child->info->type == FILE_NODE) {
            newFileName = source;

            FileContent* file = child->info->content;
            free(file->text);
            FileContent* file2 = source_node->content;
            file->text = strdup(file2->text);

            return;
        }

        if (child && child->info->type == FOLDER_NODE) {
            currentNode = child->info;
        } else if (child == NULL && strcmp(buff, "..")) {
            isFinal = 1;
            newFileName = buff;
        } else if (child == NULL && !strcmp(buff, "..") && currentNode->parent) {
            newFileName = source;
            currentNode = currentNode->parent;
        }
    }

    printf("folder name: %s\n", currentNode->name);
    printf("name: %s\n", newFileName);

    child = children->head;

    FileContent* file_content = source_node->content;
    touch(currentNode, strdup(newFileName), strdup(file_content->text));
}

void mv(TreeNode* currentNode, char* source, char* destination) {
}

