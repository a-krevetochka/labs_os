//
// Created by meteo on 07.12.2023.
//

#ifndef FIRSTLAB_NODE_H
#define FIRSTLAB_NODE_H

template <typename T>
class MyNode {
public:
    T data;
    MyNode* left;
    MyNode* right;
    MyNode() = default;
    MyNode(T data) : data(data), left(nullptr), right(nullptr) {}
};



#endif //FIRSTLAB_NODE_H
