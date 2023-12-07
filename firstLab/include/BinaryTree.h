//
// Created by meteo on 07.12.2023.
//
#include "MyNode.h"
#include "iostream"
#include "MyProcess.h"

#ifndef FIRSTLAB_BINARYTREE_H
#define FIRSTLAB_BINARYTREE_H


template<typename T>
class BinaryTree {
private:
    MyNode<T> *root;

    void addHelper(MyNode<T> *node, T data) {
        if (data < node->data) {
            if (node->left == nullptr) {
                node->left = new MyNode<MyProcess>(MyProcess(data));
            } else {
                addHelper(node->left, data);
            }

        } else {
            if (node->right == nullptr) {
                node->right = new MyNode<MyProcess>(MyProcess(data));
            } else {
                addHelper(node->right, data);
            }
        }
    }

    void printHelper(MyNode<T> *node) {
        if (node != nullptr) {
            printHelper(node->left);
            std::cout << node->data << ' ';
            printHelper(node->right);
        }
    }

    bool containHelper(MyNode<T> *node, T data) {
        auto ptr = node;
        while (ptr != nullptr) {
            if (ptr->data == data) {
                return true;
            } else if (ptr->data > data) {
                ptr = ptr->left;
            } else if (ptr->data < data) {
                ptr = ptr->right;
            }
        }
        return false;
    }

public:
    BinaryTree() : root(nullptr) {}

    void addProcess(int data) {
        if (this->contain(data)){
            throw std::invalid_argument("Error: Already Exist");
        }
        if (root == nullptr) {
            root = new MyNode<MyProcess>(MyProcess(data));
        } else {
            addHelper(root, data);
        }
    }

    bool contain(int data) {
        return containHelper(root, data);
    }

    void print() {
        printHelper(root);
    }

    T* findById(int id){
        auto ptr = root;
        while (ptr != nullptr) {
            if (ptr->data == id) {
                return &ptr->data;
            } else if (ptr->data > id) {
                ptr = ptr->left;
            } else if (ptr->data < id) {
                ptr = ptr->right;
            }
        }
        return nullptr;
    }
};

#endif //FIRSTLAB_BINARYTREE_H
