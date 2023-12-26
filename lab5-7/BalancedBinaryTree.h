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
Node* buildBalancedTree(int arr[], int start, int end);

void printInOrder(Node *node);
void inOrderTraversal(Node *node, int height);
void printTree(Node *node);

int getHeight(Node *node);
int isBalanced(Node *node);

Node* rotateLeft(Node *node);
Node* rotateRight(Node *node);

Node* balanceTree(Node *root);

Node* findMinValueNode(Node *node);

Node* deleteNode(Node *root, int key);
Node* insert(Node *root, int data);

#endif