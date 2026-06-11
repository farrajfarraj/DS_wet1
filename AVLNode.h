#pragma once

#include <iostream>

template<class T, class K>
struct AVLNode {
    AVLNode<T, K>* leftNode;
    AVLNode<T, K>* rightNode;
    int count;
    int height;
    T* val;
    K key;

    AVLNode(K valueId, T* value);
    AVLNode(const AVLNode& other);

    ~AVLNode();

    void setLeftNode(AVLNode* left);
    void setRightNode(AVLNode* right);

    int findBalanceFactor() const;
    int updateHeight() const;
    void fixValues();

    T* getVal();

    AVLNode<T, K>* rotateLeft();
    AVLNode<T, K>* rotateRight();
};

template<class T, class K>
AVLNode<T, K>::AVLNode(K valueId, T* value) : val(value) {
    leftNode = nullptr;
    rightNode = nullptr;
    count = 1;
    height = 1;
    key = valueId;
}


template<class T, class K>
AVLNode<T, K>::AVLNode(const AVLNode<T, K>& other) : val(other.val) {
    leftNode = nullptr;
    rightNode = nullptr;
    count = 1;
    height = 1;
    key = other.key;
}


template<class T, class K>
AVLNode<T,K>::~AVLNode() = default; //TODO:make sure

template<class T, class K>
T* AVLNode<T, K>::getVal() {
    return val;
}

template<class T, class K>
void AVLNode<T, K>::setLeftNode(AVLNode* left) {
    leftNode = left;
}

template<class T, class K>
void AVLNode<T, K>::setRightNode(AVLNode* right) {
    rightNode = right;
}

template<class T, class K>
int AVLNode<T, K>::findBalanceFactor() const {
    int left = leftNode != nullptr ? leftNode->height : 0;
    int right = rightNode != nullptr ? rightNode->height : 0;

    return left - right;
}

template<class T, class K>
void AVLNode<T, K>::fixValues() {
    count = (leftNode != nullptr ? leftNode->count : 0) +
            (rightNode != nullptr ? rightNode->count : 0) + 1;

    int left = leftNode != nullptr ? leftNode->height : 0;
    int right = rightNode != nullptr ? rightNode->height : 0;

    height = (left >= right ? left : right) + 1;
}



template<class T, class K>
AVLNode<T, K>* AVLNode<T,K>::rotateLeft() {
    AVLNode<T,K>* right = rightNode;
    this->rightNode = rightNode->leftNode;
    right->leftNode = this;
    this->fixValues();
    right->fixValues();
    return right;
}

template<class T, class K>
AVLNode<T,K>* AVLNode<T,K>::rotateRight() {
    AVLNode<T,K>* left = leftNode;
    this->leftNode = leftNode->rightNode;
    left->rightNode = this;
    this->fixValues();
    left->fixValues();
    return left;
}

// Flatten the AVL tree to a sorted array via in-order traversal
template<typename T, typename K>
void flattenToArray(AVLNode<T, K>* node, AVLNode<T, K> **arr, int& index) {
    if (!node) return;
    flattenToArray(node->leftNode, arr, index);
    arr[index++] = node;
    flattenToArray(node->rightNode, arr, index);
}