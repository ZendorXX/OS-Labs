#include "BalancedBinaryTree.h"

Node* createNode(int data) {
    Node *newNode = (Node*)malloc(sizeof(Node));

    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

Node* buildBalancedTree(int arr[], int start, int end) {
    if (start > end) {
        return NULL;
    }

    int mid = (start + end) / 2;

    Node *root = createNode(arr[mid]);

    root->left = buildBalancedTree(arr, start, mid - 1);
    root->right = buildBalancedTree(arr, mid + 1, end);

    return root;
}

void printInOrder(Node *node) {
    if (node == NULL) {
        return;
    }

    printInOrder(node->left);
    printf("%d ", node->data);
    printInOrder(node->right);
}

void inOrderTraversal(Node *node, int height) {
    if (node == NULL) {
        return;
    }

    inOrderTraversal(node->left, height + 1);
    printf("%*s%d\n", 4 * height, "", node->data);
    inOrderTraversal(node->right, height + 1);
}

void printTree(Node *node) {
    if (node == NULL) {
        return;
    }

    inOrderTraversal(node, 0);
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

Node* findMinValueNode(Node *node) {
    Node *current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

Node* deleteNode(Node *root, int key) {
    if (root == NULL) {
        return NULL;
    }

    if (key < root->data) {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->data) {
        root->right = deleteNode(root->right, key);
    }
    else {
        if (root->left == NULL) {
            Node *tmp = root->right;
            free(root);
            return tmp;
        }
        else if (root->right == NULL) {
            Node *tmp = root->left;
            free(root);
            return tmp;
        }

        Node *tmp = findMinValueNode(root->right);
        root->data = tmp->data;
        root->right = deleteNode(root->right, tmp->data);
    }

    return balanceTree(root);
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