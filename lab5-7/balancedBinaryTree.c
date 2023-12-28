#include "balancedBinaryTree.h"

Node* createNode(int data) {
    Node *newNode = (Node*)malloc(sizeof(Node));

    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

void printInOrder(Node *root, int height) {
    if (root == NULL) {
        return;
    }

    printInOrder(root->left, height + 1);
    printf("%*s%d\n", 4 * height, "", root->data);
    printInOrder(root->right, height + 1);
}

void printTree(Node *root) {
    if (root == NULL) {
        return;
    }

    printInOrder(root, 0);
}

void deleteTree(Node *root) {
    if (root == NULL) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);

    free(root);
}

int getHeight(Node *node) {
    if (node == NULL) {
        return 0;
    }

    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);

    return (leftHeight > rightHeight) ? (leftHeight + 1) : (rightHeight + 1);
}

int isBalanced(Node *node) {
    if (node == NULL) {
        return 1;
    }

    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);

    if (abs(leftHeight - rightHeight) <= 1 &&
        isBalanced(node->left) &&
        isBalanced(node->right)) {
        return 1;
    }

    return 0;
}

Node* rotateLeft(Node *node) {
    Node *newRoot = node->right;
    Node *tmp = newRoot->left;

    newRoot->left = node;
    node->right = tmp;

    return newRoot;
}

Node* rotateRight(Node *node) {
    Node *newRoot = node->left;
    Node *tmp = newRoot->right;

    newRoot->right = node;
    node->left = tmp;

    return newRoot;
}

Node* balanceTree(Node *root) {
    if (root == NULL) {
        return NULL;
    }

    int balanceFactor = getHeight(root->left) - getHeight(root->right);

    if (balanceFactor > 1) {
        if (getHeight(root->left->left) >= getHeight(root->left->right)) {
            root = rotateRight(root);
        } 
        else {
            root->left = rotateLeft(root->left);
            root = rotateRight(root);
        }
    }
    else if (balanceFactor < -1) {
        if (getHeight(root->right->right) >= getHeight(root->right->left)) {
            root = rotateLeft(root);
        } 
        else {
            root->right = rotateRight(root->right);
            root = rotateLeft(root);
        }
    }

    return root;
}
 

Node* insert(Node *root, int data) {
    if (root == NULL) {
        return createNode(data);
    }

    if (data < root->data) {
        root->left = insert(root->left, data);
    }
    else if (data > root->data) {
        root->right = insert(root->right, data);
    }

    return balanceTree(root);
}

Node* search(Node *root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }

    if (data < root->data) {
        return search(root->left, data);
    }
    else {
        return search(root->right, data);
    }
}