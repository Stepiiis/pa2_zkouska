
#include "CNode.h"

template<class T>
CNode<T>::CNode() {
    this->nextOrder = nullptr;
    this->prevOrder = nullptr;
    this->nextKey = "";
    this->key = "";
}

template<class T>
CNode<T>::CNode(const std::string &key, T value) {
    this->key = key;
    this->_value = value;
    this->nextOrder = nullptr;
    this->prevOrder = nullptr;
}

template<class T>
CNode<T>::CNode(const CNode<T>& rhs){
    nextOrder=rhs.nextOrder;
    prevOrder=rhs.prevOrder;
    key=rhs.key;
    nextKey=rhs.nextKey;
}


template<class T>
CNode<T> &CNode<T>::operator=(const CNode<T> &rhs) {
    {
        if (rhs == this)
            return *this;
        nextOrder = rhs.nextOrder;
        prevOrder = rhs.prevOrder;
        nextKey = rhs.nextKey;
        key = rhs.key;
        return *this;
    }
}

template<class T>
void CNode<T>::setNextKey(const std::string &key) {
    nextKey = key;
}