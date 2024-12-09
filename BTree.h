#ifndef BTREE_H
#define BTREE_H

#include <iostream>

// Forward declaration for BTreeNode
struct BTreeNode;

struct BTree {
    BTreeNode* root;
    int degree;

    BTree(int degree);
    void insert(int key);
    void printLevelOrder();
};

struct BTreeNode {
    int degree;
    int keys_size;
    int* keys;
    bool isLeaf;
    BTreeNode** children;

    BTreeNode(int degree, bool isLeaf);
    void insertNonFull(int key);
    void splitChild(int i, BTreeNode* y);
    void printNode();  // Helper function to print node keys
    ~BTreeNode();
};

#endif  // BTREE_H
