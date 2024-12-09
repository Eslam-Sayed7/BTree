
#include "BTree.h"
#include <cstring>  // For memcpy

BTreeNode::BTreeNode(int degree, bool isLeaf)
    : degree(degree), keys_size(0), isLeaf(isLeaf) {
    keys = new int[degree - 1];  // Maximum degree-1 keys
    children = new BTreeNode*[degree];  // Maximum degree children
    for (int i = 0; i < degree; i++) {
        children[i] = nullptr;
    }
}

void BTreeNode::insertNonFull(int key) {
    int i = keys_size - 1;

    if (isLeaf) {
        // Shift keys greater than key to the right
        while (i >= 0 && keys[i] > key) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = key;
        keys_size++;
    } else {
        // Find the child which will hold the new key
        while (i >= 0 && keys[i] > key) {
            i--;
        }
        i++;

        // Split the child if it's full
        if (children[i]->keys_size == degree - 1) {
            splitChild(i, children[i]);
            if (keys[i] < key) {
                i++;
            }
        }
        children[i]->insertNonFull(key);
    }
}

void BTreeNode::splitChild(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->degree, y->isLeaf);
    z->keys_size = (degree - 1) / 2;

    // Copy the last (degree-1)/2 keys of y to z
    memcpy(z->keys, y->keys + (degree / 2), z->keys_size * sizeof(int));

    // Copy the last (degree)/2 children of y to z
    if (!y->isLeaf) {
        memcpy(z->children, y->children + (degree / 2), (z->keys_size + 1) * sizeof(BTreeNode*));
    }

    y->keys_size = (degree - 1) / 2;

    // Insert z as a child of this node
    for (int j = keys_size; j >= i + 1; j--) {
        children[j + 1] = children[j];
    }
    children[i + 1] = z;

    // Move the middle key of y to this node
    for (int j = keys_size - 1; j >= i; j--) {
        keys[j + 1] = keys[j];
    }
    keys[i] = y->keys[(degree - 1) / 2];

    keys_size++;
}

void BTreeNode::printNode() {
    for (int i = 0; i < keys_size; i++) {
        std::cout << keys[i] << " ";
    }
    std::cout << "| ";
}

BTreeNode::~BTreeNode() {
    delete[] keys;
    for (int i = 0; i < degree; i++) {
        delete children[i];
    }
    delete[] children;
}

BTree::BTree(int degree) : root(nullptr), degree(degree) {}

void BTree::insert(int key) {
    if (!root) {
        root = new BTreeNode(degree, true);
        root->keys[0] = key;
        root->keys_size = 1;
    } else {
        if (root->keys_size == degree - 1) {
            BTreeNode* s = new BTreeNode(degree, false);
            s->children[0] = root;
            s->splitChild(0, root);

            int i = 0;
            if (s->keys[0] < key) {
                i++;
            }
            s->children[i]->insertNonFull(key);

            root = s;
        } else {
            root->insertNonFull(key);
        }
    }
}

void BTree::printLevelOrder() {
    if (!root) return;

    const int MAX_NODES = 100;
    BTreeNode* queue[MAX_NODES];
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        int nodeCount = rear - front;

        while (nodeCount > 0) {
            BTreeNode* node = queue[front++];
            node->printNode();

            if (!node->isLeaf) {
                for (int i = 0; i <= node->keys_size; i++) {
                    if (node->children[i]) {
                        queue[rear++] = node->children[i];
                    }
                }
            }
            nodeCount--;
        }
        std::cout << std::endl;  // New line after each level
    }
}
