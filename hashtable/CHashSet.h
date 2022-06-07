#pragma once
#include "CLinkedList.h"
#include "CNode.h"
#include <string>
#include <iostream>



template <class T>
class CHashSet {
private:
    CLinkedList<T>* _table = nullptr;
    size_t _size;
    CNode<T>* idLastAdded = nullptr;
public:
    explicit CHashSet(int size);
    ~CHashSet();
    int hash(const std::string& key);
    bool insert(std::string key, T value);
    bool remove(std::string key);
    const CNode<T> & at(const std::string & key);
    CNode<T> & operator[](const std::string & key);
    bool isInSet(std::string key);
    bool print(std::string key);
    void printAll();
};

template<class T>
CHashSet<T>::CHashSet(int size):_size(size) {
    this->_table = new CLinkedList<T>[size]();
}

template<class T>
int CHashSet<T>::hash(const std::string& key) {
    hashFunc hashFunction;
    return  hashFunction(key)% this->_size;
}

template<class T>
CHashSet<T>::~CHashSet() {
    delete [] _table;

}
// returns false if this key is already in the set
template<class T>
bool CHashSet<T>::insert(std::string key, T value) {
    size_t index = hash(key);
    auto* temp = _table[index].addNode(key,value);
    if(temp == nullptr)
        throw std::out_of_range("CHashSet::insert");
    if(idLastAdded != nullptr)
        idLastAdded->nextKey=key;
    idLastAdded = temp;
    return true;
}

template<class T>
bool CHashSet<T>::remove(std::string key) {
    size_t index = hash(key);
    if(_table[index]->erase(key))
        return true;
    return false;
}

template<class T>
const CNode<T> & CHashSet<T>::at(const std::string &key) {
    int index = hash(key);
    auto* found = _table[index][key];
    if(found == nullptr)
        throw std::out_of_range("CHashSet::at()");
    return *found;
}

template<class T>
CNode<T> & CHashSet<T>::operator[](const std::string &key) {
    int index = hash(key);
    auto* found = _table[index][key];
    if(found == nullptr)
        throw std::out_of_range("CHashSet::operator[]()");
    return *found;
}

template<class T>
bool CHashSet<T>::isInSet(std::string key) {
    int index = hash(key);
    auto* found = _table[index][key];
    if (found == nullptr)
        return false;
    return true;
}
