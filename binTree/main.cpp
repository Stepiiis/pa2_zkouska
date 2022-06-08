#include <iostream>
#include <vector>
#include <cassert>

template<typename A,typename T>
class CBinTree{
    struct CNode{
        CNode* _parent = nullptr;
        CNode* _right = nullptr;
        CNode* _left = nullptr;
        A _key{};
        T _value;
        CNode & operator=(const CNode & rhs){if(this == rhs){return *this;} _parent = rhs._parent; _right = rhs._parent; _left =rhs._left; _key = rhs._key; _value = rhs._value; return *this;}
    };
    class Iterator {
    private:
        CNode* m_ptr = nullptr;
    public:
        explicit Iterator(CNode* pointer);
        Iterator operator++();
        Iterator operator++(int);
        Iterator operator--();
        Iterator operator--(int);
        Iterator operator=(const Iterator & rhs);
        friend bool operator!=(const Iterator& lhs,const Iterator &rhs){return lhs.m_ptr!=rhs.m_ptr;}
        friend bool operator==(const Iterator& lhs,const Iterator &rhs){return lhs.m_ptr==rhs.m_ptr;}
        CNode * move(CNode* ptr);
        T * operator->(); // returns *this
        T operator *(); // returns this
    };
public:
    CBinTree() = default;
    ~CBinTree();
    bool add(A key,T value);
    bool isInTree(A key);
    T& find(A key)const;
    bool remove(A key);
    void clear();
    A getSmallest() const;
    A getBiggest() const;
    Iterator begin(){return Iterator(_root);}
    Iterator end(){ return Iterator(nullptr);}
private:
    CNode* _root = nullptr;
    void removeNode(CNode* ptr);
};

template<typename A, typename T>
CBinTree<A, T>::Iterator::Iterator(CNode *pointer) {
    m_ptr = pointer;
    if(m_ptr == nullptr)
        return;
    while(m_ptr->_left!=nullptr) {
        m_ptr = m_ptr->_left;
    }
    visited.add(m_ptr->_key,m_ptr->_value);
}

template<typename A, typename T>
typename CBinTree<A,T>::Iterator CBinTree<A, T>::Iterator::operator++(int) {
    auto temp = this;
    if(visited.isInTree(m_ptr->_key)) {
        m_ptr = m_ptr->_parent;
        return *temp;
    }
    if(m_ptr->_parent==nullptr) // jsme na vrcholu
    {
        m_ptr = m_ptr->_right;
        visited.add(m_ptr->_key,m_ptr->_value);
    }
    else if(m_ptr->_left != nullptr && !visited.isInTree(m_ptr->_left->_key)){
        m_ptr = m_ptr->_left;
        m_ptr = move(m_ptr);
        return *temp;
    }
    else if (m_ptr->_left==nullptr && m_ptr->_right!=nullptr && !visited.isInTree(m_ptr->_right->_key)){
        m_ptr=m_ptr->_right;
        m_ptr = move(m_ptr);
        return *temp;
    }
    else if(m_ptr->_left == nullptr && m_ptr->_right == nullptr){
        visited.add(m_ptr->_key,m_ptr->_value);
        m_ptr = m_ptr->_parent;
        return *temp;
    }
    return *temp;
}

template<typename A, typename T>
typename CBinTree<A,T>::CNode * CBinTree<A, T>::Iterator::move(CNode* ptr){
    auto temp = ptr;
    if(visited.isInTree(temp->_key)) {
        temp = temp->_parent;
        return temp;
    }
    if(temp->_parent==nullptr) // jsme na vrcholu
    {
        temp = temp->_right;
    }
    else if(temp->_left != nullptr && !visited.isInTree(temp->_left->_key)){
        temp = temp->_left;
        if(temp->_left != nullptr)
            temp = move(temp);
        return temp;
    }
    else if (temp->_left==nullptr && temp->_right!=nullptr && !visited.isInTree(temp->_right->_key)){
        temp=temp->_right;
        return move(temp);
    }
    else if(temp->_left == nullptr && temp->_right == nullptr){
        temp = temp->_parent;
        return temp;
    }
    return temp;
}

template<typename A, typename T>
typename CBinTree<A,T>::Iterator CBinTree<A, T>::Iterator::operator++() {
    if(visited.isInTree(m_ptr->_key)) {
        m_ptr = m_ptr->_parent;
        return *this;
    }
    if(m_ptr->_parent==nullptr) // jsme na vrcholu
    {
        m_ptr = m_ptr->_right;
        visited.add(m_ptr->_key,m_ptr->_value);
        return *this;
    }
    else if(m_ptr->_left != nullptr && !visited.isInTree(m_ptr->_left->_key)){
        m_ptr = m_ptr->_left;
        if(m_ptr->_left != nullptr)
            m_ptr = move(m_ptr);
        return *this;
    }
    else if (m_ptr->_right!=nullptr && !visited.isInTree(m_ptr->_right->_key)){
        m_ptr=m_ptr->_right;
        if(m_ptr->_left!=nullptr)
            m_ptr=move(m_ptr);
        return *this;
    }
    else if(m_ptr->_right == nullptr){
        visited.add(m_ptr->_key,m_ptr->_value);
        m_ptr = m_ptr->_parent;
        return *this;
    }
    return *this;
}

template<typename A, typename T>
T CBinTree<A, T>::Iterator::operator*() {
    return (m_ptr->_value);
}

template<typename A, typename T>
T *CBinTree<A, T>::Iterator::operator->() {
    return m_ptr;
}


template<typename A, typename T>
bool CBinTree<A, T>::add(A key, T value) {
    auto temp = _root;
    auto parent = _root;
    while(temp!=nullptr)
    {
        parent = temp;
        if(!(key<temp->_key)&&!(temp->_key<key))
        {
            temp->_value = value;
            return false;
        }
        if(key<temp->_key)
            temp=temp->_left;
        else if(temp->_key<key)
            temp=temp->_right;
    }

    temp = new CBinTree::CNode;
    if(parent != nullptr) {
        if (parent->_key < key)
            parent->_right = temp;
        else {
            parent->_left = temp;
        }
    }
    if(_root == nullptr) _root = temp;
    temp->_parent = parent;
    temp->_key = key;
    temp->_value = value;

    return true;
}

template<typename A, typename T>
T &CBinTree<A, T>::find(A key) const {
    auto temp = _root;
    while(temp!=nullptr)
    {
        if(!(key<temp->_key) && !(temp->_key<key)){
            return temp->_value;
        }
        else if(key<temp->_key)
            temp=temp->_left;
        else if(temp->_key<key)
            temp=temp->_right;
    }
    throw std::range_error("Key was not in tree");
}

template<typename A, typename T>
bool CBinTree<A, T>::isInTree(A key) {
    auto temp = _root;
    while(temp!=nullptr)
    {
        if(!(key<temp->_key) && !(temp->_key<key))
            return true;
        if(key<temp->_key)
            temp=temp->_left;
        else if(temp->_key<key)
            temp=temp->_right;
    }
    return false;
}

template<typename A, typename T>
bool CBinTree<A, T>::remove(A key) {
    CNode * temp = _root;
    CNode* parent = _root;
    while((temp)!=nullptr)
    {
        if(!(key<temp->_key) && !(temp->_key<key))
            break;
        else if(key<(temp)->_key) {
            parent = temp;
            (temp) = (temp)->_left;
        }
        else if((temp)->_key<key) {
            parent = temp;
            (temp) = (temp)->_right;
        }
    }

    if( temp != nullptr)  {
        auto toRemove = temp;
        auto move = temp;
        if (move->_left != nullptr && move->_right != nullptr) {
            move = move->_left;
            while (move->_right != nullptr) {
                parent = move;
                move = move->_right;
            }
            toRemove->_value = move->_value;
            toRemove->_key = move->_key;
            CNode *toDelete = move;
            if(parent!=toRemove) parent->_right = toDelete->_left;
            else parent->_left = nullptr;
            delete toDelete;
            return true;
        }else if(move->_left == nullptr && move->_right == nullptr){
            if(move->_key<parent->_key)
                parent->_left = nullptr;
            else
                parent->_right = nullptr;
            delete move;
        }
        else if( move->_left == nullptr) {
            if(move->_key<parent->_key)
                parent->_left = move->_right;
            else
                parent->_right = move->_right;
            toRemove->_key = move->_key;
            toRemove->_value = move->_value;
            delete move;
            return true;
        }else if( move->_right == nullptr){
            if(move->_key<parent->_key)
                parent->_left = move->_left;
            else
                parent->_right = move->_left;
            toRemove->_key = move->_key;
            toRemove->_value = move->_value;
            delete move;
            return true;
        }
    }
    return false; // key was not in tree, could not remove
}

template<typename A, typename T>
void CBinTree<A, T>::removeNode(CBinTree::CNode *ptr) {
if(ptr->_left != nullptr)
    removeNode(ptr->_left);
if(ptr->_right != nullptr)
    removeNode(ptr->_right);
delete ptr;
}

template<typename A, typename T>
CBinTree<A, T>::~CBinTree() {
    removeNode(_root);
}

template<typename A, typename T>
A CBinTree<A, T>::getSmallest() const {
    auto temp = _root;
    while(temp->_left!= nullptr)
        temp = temp->_left;
    if(temp!= nullptr){
        return temp->_key;
    }
    return A{};
}
// returns empty initialized value if empty
template<typename A, typename T>
A CBinTree<A, T>::getBiggest() const {
    auto temp = _root;
    while(temp->_right!= nullptr)
        temp = temp->_right;
    if(temp != nullptr){
        return temp->_key;
    }
    return A{};

}

template<typename A, typename T>
void CBinTree<A, T>::clear() {
    removeNode(_root);
}


int main(){
    CBinTree<int, int> tree;
    tree.add(8,8);
    tree.add(4,4);
    tree.add(12,12);
    tree.add(18,18);
    tree.add(3,3);
    tree.add(5,5);
    tree.add(6,6);
    tree.add(10,10);
    tree.add(11,1234);
    assert(tree.isInTree(11));
    assert(!tree.isInTree(16));
    std:: cout << tree.find(11)<<std::endl;
    std:: cout << tree.remove(11) << std::endl;
    std:: cout << tree.getSmallest() << std::endl;
    std:: cout << tree.getBiggest() << std::endl;
    for(int it: tree){
        std::cout << it;
    }
    return EXIT_SUCCESS;
}
