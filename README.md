# Simple Linux File System

**Student 1: Omer Tarik Ilhan**

**Student 2: Preda Diana**

**Group: 314CA**

    “Caaaaaade, dă-te bă că cade!” - about trees, Mihai Mărgineanu (1969 - present)

## Description

This project represents the implementation of a Simple Linux Filesystem, as the name implies.

It was built using mostly trees (that can have multiple children). There were two building blocks
used: File Node and Folder Node, both represented in the TreeNode structure.

The tree node structure contains the following fields:
* Name: string, representing name of the node
* Type: enum, that can either have FILE_NODE or FOLDER_NODE as a value, depending on the type of node
* Content: void pointer, pointing to a structure containing either File Content (a string) or Folder Content
(a list, whose nodes contain children of current folder)
* Parent: pointer to another TreeNode, representing the parent node of current node

The main function contains a string parser, going through commands read from the stdin, until it meets an EOF.

The program contains several commands that are key for any functioning Filesystem command set:
* ls [arg] - print nodes in current folder if there is no argument; if argument is a folder name, print
nodes inside given folder; if argument is a file name, print content of given file
* pwd - print to stdout path to current folder
* tree [path] - print node hierarchy starting from current node if there is no path given or from given path
* touch <filename> [filecontent] - create a file with given name, empty or containing given string
* mkdir <dirname> - create a directory with given name
* cd <path> - change current directoy to given (relative) path
* rm <filename> - remove file with given name (only for files)
* rmdir <dirname> - remove folder with given name (only for empty folders)
* rmrec <resource_name> - remove file / folder, regardless whether it is or is not empty (recursively delete it)
* cp <source_path> <destination_path> - copy file from source to destination (only for files)
* mv <source_path> <destination_path> - move file / folder from copy to destination (for both files and folders)

In the main function, there is a node storing the current folder, that is move around using 'cd' and all
the paths used in other commands are relative to the current folder.

A very important function used is get_path(), which parses through a given path, returning the desired node,
which can be either a file or a folder. It returns NULL if it is encounters errors accessing given path.

The freeing of the FileTree was done recursively, calling the free_node function in every node that is in a folder or
freeing the memory directly if the node is a file.

## Possible improvements:
* A more modular version of the code. We use a very similar snippet of code to the get_path() function
in cp and mv, because we needed to store both the path and the name to the node and also because we had to
exit the function in certain conditions.
* Shorter functions.
* Better function genericity.

## Comments:
* This project enabled us to work with trees, helping us develop a stable understanding of them (one that prior
to this project was fairly low).
* Being a team project, it helped us acknowledge and appreciate how a team communicates and cooperates within a coding project.
Both of us worked in teams before, but not on something related to programming.

## Resources:
1. [DIE Macro] (https://ocw.cs.pub.ro/courses/so/laboratoare/resurse/die)