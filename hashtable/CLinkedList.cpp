#include "CLinkedList.h"
#include <iostream>

template<class T>
CLinkedList<T>::~CLinkedList() {
    CNode<T>* now = _root;
    CNode<T>* toBeDeleted;
    while(now != nullptr) {
        toBeDeleted = now;
        now = toBeDeleted->nextOrder;
        delete toBeDeleted;
    }
}

template<class T>
CNode<T> * CLinkedList<T>::addNode(const std::string & key,T value) {
    auto* check = at(key);
    if (check != nullptr)
        throw std::range_error("CLinkedList::addNode key already in set");
    auto temp = _last;
    if(_currentSize == 0){
        temp = new CNode<T>(key, value);
        _root = temp;
        _last = temp;
        _currentSize++;
        return temp;
    }
    _last = _last->nextOrder = new CNode<T>(key, value);
    _last->prevOrder = temp;
    _currentSize++;
    return _last;
}

template<class T>
size_t CLinkedList<T>::size() {
    return _currentSize;
}

// returns nullptr if key not found
template<class T>
const CNode<T> * CLinkedList<T>::at(const std::string &key) {
    auto* temp = _root;

    while(temp != nullptr && temp->key != key)
        temp = temp->nextOrder;

    if (temp != nullptr && temp->key == key)
        return temp;
    return nullptr;
}
// returns nullptr if index not found
template<class T>
const CNode<T> *CLinkedList<T>::at(int index) {
    auto * temp = _root;
    if(temp == nullptr)
        return temp;
    for(int i = 0; i<index; i++)
    {
        if(temp->nextOrder != nullptr)
            temp = temp->nextOrder;
        else break;
    }
    return temp;
}

template<class T>
CNode<T> *CLinkedList<T>::operator[](const std::string &key) {
    auto* temp = _root;

    while(temp != nullptr && temp->key != key)
        temp = temp->nextOrder;

    if (temp != nullptr && temp->key == key)
        return temp;
    return nullptr;
}

template<class T>
CNode<T> *CLinkedList<T>::operator[](int index) {
    auto * temp = _root;
    if(temp == nullptr)
        return temp;
    for(int i = 0; i<index; i++)
    {
        if(temp->nextOrder != nullptr)
            temp = temp->nextOrder;
        else break;
    }
    return temp;
}

template<class T>
bool CLinkedList<T>::addNextKey(const CNode<T> *prvek, const std::string &key) {
    if (prvek == nullptr)
        return false;
    prvek->nextKey = key;
    return true;
}

template<class T>
bool CLinkedList<T>::erase(const std::string &key) {
    auto * found = this[key];
    if ( found == nullptr )
        return false;
    auto * prev = found->prevOrder;
    auto * next = found->nextOrder;
    if(prev != nullptr)
        prev->nextOrder = next;
    if(next != nullptr)
        next->prevOrder = prev;
    delete found;
    return true;
}

template<class T>
CLinkedList<T>::CLinkedList(): _root(nullptr), _last(nullptr), _currentSize(0) {}

