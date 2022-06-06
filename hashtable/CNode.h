#pragma once

#include <string>
template<class T>
struct CNode {
    CNode* nextOrder;
    CNode* prevOrder;
    std::string nextKey;
    std::string key;
    T _value;

    CNode(){
        this->nextOrder = nullptr;
        this->prevOrder = nullptr;
        this->nextKey = "";
        this->key = "";
    }
    void setNextKey(const std::string& key);

    explicit CNode(const std::string& key, T value) {
        this->key = key;
        this->_value = value;
        this->nextOrder = nullptr;
        this->prevOrder = nullptr;
    }
    CNode<T> & operator = (const CNode<T>& rhs){
        if (rhs == this)
            return *this;
        nextOrder = rhs.nextOrder;
        prevOrder = rhs.prevOrder;
        nextKey = rhs.nextKey;
        key = rhs.key;
        return *this;
    }

};

template<class T>
void CNode<T>::setNextKey(const std::string &key) {
    nextKey = key;
}

// láďovo hashFunkce
class hashFunc
{
public:
    size_t  operator ()  ( const std::string & str ) const
    {
        size_t h = 5381, len = str . length ();
        if ( len > 32 )
            len = 32;
        for ( size_t i = 0; i < len; i ++ )
            h += (h << 5 ) + str[i];
        return h;
    }
};