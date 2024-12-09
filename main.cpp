#include <iostream>
#include <queue>
using namespace std;

template <typename T, int DEGREE>
struct BTreeNode {
    int keys_size;
    T keys[DEGREE];
    BTreeNode* children[DEGREE + 1];
    BTreeNode* parent; // Pointer to the parent node
    bool isLeaf;

    BTreeNode(bool isLeaf, BTreeNode* parent = nullptr) 
        : isLeaf(isLeaf), keys_size(0), parent(parent) {
        for (int i = 0; i <= DEGREE; ++i) children[i] = nullptr;
    }

    void insertNonFull(T key);
    void splitChild(int i, BTreeNode* y);
    int findChildIndex(BTreeNode<T, DEGREE>* child);
};

template <typename T, int DEGREE>
struct BTree {
    BTreeNode<T, DEGREE>* root;

    BTree() : root(nullptr) {}

    void Insert(T key);
    void Print(string indentation = " ", int level = 0);
};

template <typename T, int DEGREE>
void BTree<T, DEGREE>::Insert(T key) {
    if (root == nullptr) {
        // If the tree is empty, create a new root
        root = new BTreeNode<T, DEGREE>(true);
        root->keys[0] = key;
        root->keys_size = 1;
    } else {
        // Insert recursively and handle splits dynamically
        if (root->keys_size == DEGREE) {
            // Root is full; create a new root and split
            BTreeNode<T, DEGREE>* newRoot = new BTreeNode<T, DEGREE>(false);
            newRoot->children[0] = root;
            root->parent = newRoot;
            newRoot->splitChild(0, root);
            root = newRoot;
        }
        // Insert key in the appropriate node
        root->insertNonFull(key);
    }
}

template <typename T, int DEGREE>
void BTreeNode<T, DEGREE>::insertNonFull(T key) {
    int i = keys_size - 1;

    if (isLeaf) {
        // Insert the key in the correct position
        while (i >= 0 && keys[i] > key) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = key;
        keys_size++;
    } else {
        // Find the child to insert into
        while (i >= 0 && keys[i] > key) i--;
        i++;

        // If the child is full, split it before inserting
        if (children[i]->keys_size == DEGREE) {
            splitChild(i, children[i]);
            // After splitting, decide which child to proceed with
            if (keys[i] < key) i++;
        }
        children[i]->insertNonFull(key);
    }
}

template <typename T, int DEGREE>
void BTreeNode<T, DEGREE>::splitChild(int i, BTreeNode<T, DEGREE>* child) {
    BTreeNode<T, DEGREE>* sibling = new BTreeNode<T, DEGREE>(child->isLeaf);
    sibling->keys_size = DEGREE / 2;

    // Copy the latter half of the child's keys to the sibling
    for (int j = 0; j < DEGREE / 2; ++j)
        sibling->keys[j] = child->keys[j + DEGREE / 2 + 1];

    // Copy the child pointers (if not a leaf)
    if (!child->isLeaf) {
        for (int j = 0; j <= DEGREE / 2; ++j)
            sibling->children[j] = child->children[j + DEGREE / 2 + 1];
    }

    child->keys_size = DEGREE / 2;

    // Insert the new sibling into the children array of the current node
    for (int j = keys_size; j >= i + 1; --j)
        children[j + 1] = children[j];
    children[i + 1] = sibling;

    // Move the middle key to the current node
    for (int j = keys_size - 1; j >= i; --j)
        keys[j + 1] = keys[j];
    keys[i] = child->keys[DEGREE / 2];
    keys_size++;
}


template <typename T, int DEGREE>
void BTree<T, DEGREE>::Print(string indentation, int level) {
    if (!root) return;

    // Queue for level-order traversal
    queue<BTreeNode<T, DEGREE>*> q;
    q.push(root);

    while (!q.empty()) {
        int nodeCount = q.size();

        while (nodeCount > 0) {
            BTreeNode<T, DEGREE>* node = q.front();
            q.pop();

            // Print the current node keys with the current indentation
            cout << indentation;
            for (int i = 0; i < node->keys_size; i++) {
                cout << node->keys[i];
                if (i < node->keys_size - 1) cout << ",";
            }
            cout << '\n'; // Newline after printing the current node

            // Add child nodes to the queue if it's not a leaf
            if (!node->isLeaf) {
                for (int i = 0; i <= node->keys_size; i++) {
                    if (node->children[i]) q.push(node->children[i]);
                }
            }
            nodeCount--;
        }

        // Increase indentation for the next level
        indentation += "  "; // Add two spaces for each level
    }
}

int main() {
    BTree<int, 3> t0;
    t0.Insert(1);
    t0.Insert(5);
    t0.Insert(0);
    t0.Insert(4);
    t0.Insert(3);
    t0.Insert(2);
    t0.Print("", 0);

    cout << '\n' << '\n';

    BTree<char, 5> t1;

    t1.Insert('G');
    t1.Insert('I');
    t1.Insert('B');
    t1.Insert('J');
    t1.Insert('C');
    t1.Insert('A');
    t1.Insert('K');
    t1.Insert('E');
    t1.Insert('D');
    t1.Insert('S');
    t1.Insert('T');
    t1.Insert('R');
    t1.Insert('L');
    t1.Insert('F');
    t1.Insert('H');
    t1.Insert('M');
    t1.Insert('N');
    t1.Insert('P');
    t1.Insert('Q');

    t1.Print("", 0);
    return 0;
}
