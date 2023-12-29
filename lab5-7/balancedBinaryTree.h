#ifndef BALANCED_BINARY_TREE_H
#define BALANCED_BINARY_TREE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

struct Node{
    int data;
    Node *left;
    Node *right;
};

Node* createNode(int data);
Node* rotateLeft(Node *node);
Node* rotateRight(Node *node);
Node* balanceTree(Node *root);
Node* insert(Node *root, int data);
Node* search(Node *root, int data);

void printInOrder(Node *root, int height);
void printTree(Node *root);
void deleteTree(Node *root);

int getHeight(Node *node);
int isBalanced(Node *node);
int pathToNode(Node *root, int data, int *path, int index);

#endif