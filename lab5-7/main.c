#include <stdio.h>
#include "BalancedBinaryTree.h"

int main() {
    Node *root = createNode(6);
    root = insert(root, 4);
    root = insert(root, 1);
    root = insert(root, 3);
    root = insert(root, 5);
    root = insert(root, 2);
    root = insert(root, 45);
    root = insert(root, -1);
    for (int i = 50; i < 60; ++i) {
        root = insert(root, i);
    }

    if (isBalanced(root)) {
        printf("Balance\n");
    } 
    else {
        printf("Not balanced\n");
    }

    printTree(root);
    printf("\n%d\n", root->data);

    int arr[] = {4, 1, 3, 5, 2};
    int n = sizeof(arr) / sizeof(arr[0]);
  
    Node* root2 = buildBalancedTree(arr, 0, n - 1);
    if (isBalanced(root)) {
        printf("Balance\n");
    } 
    else {
        printf("Not balanced\n");
    }
  
    printTree(root2);

    free(root);
    free(root2);   
    return 0;
}