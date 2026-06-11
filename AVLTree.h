#pragma once
#include "AVLNode.h"
#include <iostream>


template<class T, class K>
class AVLTree {
protected:
    AVLNode<T,K>* root; // Root node of the AVL tree
    int size;         // Number of nodes in the tree


    // Recursive helper for insertion
    AVLNode<T,K>* insertHelper(AVLNode<T,K>* current, AVLNode<T,K>* node) {
        if (!current) return node;

        if (node->key < current -> key) {
            current->leftNode = insertHelper(current->leftNode, node);
        } else {
            current->rightNode = insertHelper(current->rightNode, node);
        }

        current->fixValues();
        return balance(current);
    }

    // Recursive helper for finding a node by key
    AVLNode<T, K>* findHelper(AVLNode<T, K>* current, const K& key) const {
        if (!current) return nullptr;
        if (key < current->key) return findHelper(current->leftNode, key);
        if (key > current->key) return findHelper(current->rightNode, key);
        return current;
    }

    // Balances the given node based on its balance factor
    AVLNode<T, K>* balance(AVLNode<T, K>* node) {
        int balanceFactor = node->findBalanceFactor();
        if (balanceFactor > 1) {
            if (node->leftNode->findBalanceFactor() >= 0) {
                return node->rotateRight(); // Left-Left case
            } else {
                node->leftNode = node->leftNode->rotateLeft();
                return node->rotateRight(); // Left-Right case
            }
        }
        if (balanceFactor < -1) {
            if (node->rightNode->findBalanceFactor() <= 0) {
                return node->rotateLeft(); // Right-Right case
            } else {
                node->rightNode = node->rightNode->rotateRight();
                return node->rotateLeft(); // Right-Left case
            }
        }
        return node; // Already balanced
    }



    AVLNode<T, K>* eraseHelper(AVLNode<T, K>* current, const K& key) {
        // Base cases: Empty tree or key not found
        if (current == nullptr) {
            return nullptr; // Key not found
        }

        // Traverse based on key comparison
        if (key < current->key) {
            current->leftNode = eraseHelper(current->leftNode, key);
        } else if (key > current->key) {
            current->rightNode = eraseHelper(current->rightNode, key);
        } else {
            // Node found, handle deletion

            // Case 1: Node has
            // Case 1: Node has no children (leaf node)
            if (current->leftNode == nullptr && current->rightNode == nullptr) {

                delete current; // Manually deallocate memory for the leaf node
                return nullptr;
            }

            // Case 2: Node has one child
            if (current->leftNode == nullptr) {
                // One child on the right
                AVLNode<T, K>* temp = current;
                current = current->rightNode;
                delete temp; // Delete the current node
                return current;
            } else if (current->rightNode == nullptr) {
                // One child on the left
                AVLNode<T, K>* temp = current;
                current = current->leftNode;
                delete temp; // Delete the current node
                return current;
            }

            // Case 3: Node has two children (choose successor)
            // Find the in-order successor (smallest node in the right subtree)
            AVLNode<T, K>* successor = current->rightNode;
            AVLNode<T, K>* successorParent = current;
            while (successor->leftNode != nullptr) {
                successorParent = successor;
                successor = successor->leftNode;
            }

            // Copy the value and key of the successor to the current node
            current->key = successor->key;
            if (successorParent != current) {
                successorParent->leftNode = successor->rightNode;
                successor->rightNode = current->rightNode;}
            successor->leftNode = current->leftNode;

            // Recursively delete the successor (backtracking)
            delete current;
            current = successor;
        }

        // Update height after deletion (may have changed)
        if(current != nullptr){
            current->fixValues();
        }

        // Perform rotations for balance (implementation details omitted)
        return balance(current);
    }

    AVLNode<T, K>* eraseHelperNoDelete(AVLNode<T, K>* current, const K& key) {
        // Base cases: Empty tree or key not found
        if (current == nullptr) {
            return nullptr; // Key not found
        }

        // Traverse based on key comparison
        if (key < current->key) {
            current->leftNode = eraseHelperNoDelete(current->leftNode, key);
        } else if (key > current->key) {
            current->rightNode = eraseHelperNoDelete(current->rightNode, key);
        } else {
            // Node found, handle deletion

            // Case 1: Node has
            // Case 1: Node has no children (leaf node)
            if (current->leftNode == nullptr && current->rightNode == nullptr) {

                return nullptr;
            }

            // Case 2: Node has one child
            if (current->leftNode == nullptr) {
                // One child on the right
                AVLNode<T, K>* temp = current;
                current = current->rightNode;
                return current;
            } else if (current->rightNode == nullptr) {
                // One child on the left
                AVLNode<T, K>* temp = current;
                current = current->leftNode;
                return current;
            }

            // Case 3: Node has two children (choose successor)
            // Find the in-order successor (smallest node in the right subtree)
            AVLNode<T, K>* successor = current->rightNode;
            AVLNode<T, K>* successorParent = current;
            while (successor->leftNode != nullptr) {
                successorParent = successor;
                successor = successor->leftNode;
            }

            // Copy the value and key of the successor to the current node
            current->key = successor->key;
            if (successorParent != current) {
                successorParent->leftNode = successor->rightNode;
                successor->rightNode = current->rightNode;}
            successor->leftNode = current->leftNode;

            // Recursively delete the successor (backtracking)
            current = successor;
        }

        // Update height after deletion (may have changed)
        if(current != nullptr){
            current->fixValues();
        }

        // Perform rotations for balance (implementation details omitted)
        return balance(current);
    }
    // Recursive helper for clearing the tree
    void clear(AVLNode<T, K>* node) {
        if (!node) return;
        clear(node->leftNode);
        clear(node->rightNode);
        delete node;
    }

public:
    // Constructors and Destructor
    AVLTree() : root(nullptr), size(0) {}

    AVLTree(AVLNode<T, K>* root) : root(root) {
        if (root == nullptr) {
            size = 0;
        }
        else {
            size = root->count;
        }
    };

    // Helper function for destructor
    void deleteHelperWithValues(AVLNode<T, K>* current) {
        if (current == nullptr) {
            return;
        }
        deleteHelperWithValues(current->leftNode);
        deleteHelperWithValues(current->rightNode);
        delete (current->val);

    }

    void deleteHelperOnlyNodes(AVLNode<T, K>* current) {
        if (current == nullptr) {
            return;
        }
        deleteHelperOnlyNodes(current->leftNode);
        deleteHelperOnlyNodes(current->rightNode);
        delete current; // Delete the current node
    }

    void clearValues() {
        deleteHelperWithValues(root);
    }

    virtual ~AVLTree() {
        //std::cout << "destroy" << std::endl;
        if(root == nullptr){
            return;
        }

        // Recursive helper function to delete all nodes in the tree
        deleteHelperOnlyNodes(root);
    }


    AVLNode<T, K>* lowerBound(const K& target) const {
        AVLNode<T, K>* curr = root;
        AVLNode<T, K>* candidate = nullptr;

        while (curr) {
            // if cur->val >= target
            if (curr->key >= target) {
                candidate = curr;
                curr = curr->leftNode;
            } else {
                curr = curr->rightNode;
            }
        }
        return candidate;
    }


    AVLNode<T, K>* getRoot() {
        return root;
    }

    bool empty() const {
        return root == nullptr;
    }

    int getSize() const {
        return size;
    }

    int getHeight() const {
        return root ? root->height : 0;
    }

    AVLNode<T, K>* getMaximum(AVLNode<T, K>* node = nullptr) const {
        AVLNode<T, K>* current = node ? node : root;
        while (current && current->rightNode) {
            current = current->rightNode;
        }
        return current;
    }

    AVLNode<T, K>* getMinimum(AVLNode<T, K>* node = nullptr) const {
        AVLNode<T, K>* current = node ? node : root;
        while (current && current->leftNode) {
            current = current->leftNode;
        }
        return current;
    }



    void remove(const K& key) {
        root = eraseHelper(root, key);
    }

    AVLNode<T, K>* find(const K& key) const {
        return findHelper(root, key);
    }

    void erase(const K& key) {
        if (!this->find(key)) {
            return;
        }
        root = eraseHelper(root, key);
        size--;
    }

    void insert(AVLNode<T, K>* node) {
        // Recursive helper function for insertion
        /*
        if(this->contains(node->key)){
          return;
        }
        */
        if(root != nullptr){
            root = insertHelper(root, node);
            size++;
        }
        else{
            root = node;
            size++;
        }
        // Increment size after successful insertion
    }
};

// Helper to build a complete binary tree of height h from arr[]
// The tree structure is built first, and actual values from arr are assigned during reverse in-order traversal
template<typename T, typename K>
AVLNode<T, K> *buildAlmostCompleteTree(int h, AVLNode<T, K> **arr,int size, int &index)
{
    if (h == 0 || index == size) return nullptr;

    // Build left subtree
    AVLNode<T, K>* left = buildAlmostCompleteTree(h - 1, arr, size, index);
    if (index == size) {
        return left;
    }

    // Create current node with actual value from arr
    AVLNode<T, K>* node = new AVLNode<T, K>(*arr[index]);
    index++; // move to the next array element

    // Build right subtree
    node->leftNode = left;
    node->rightNode = buildAlmostCompleteTree(h - 1, arr, size, index);

    // Fix height and size
    node->fixValues();

    return node;
}
