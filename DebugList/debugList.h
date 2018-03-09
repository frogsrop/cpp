#ifndef DEBUGLIST
#define DEBUGLIST

#include <cassert>
#include <memory>

template <typename T> class debugList {
private:
    class node;
public:

    class iterator {
    public:
        T &operator*() const;

        iterator &operator++();

        iterator operator++(int);

        iterator &operator--();

        iterator operator--(int);


        bool operator!= (iterator const&) const;
        bool operator== (iterator const&) const;

    private:
        friend class debugList;

        iterator(std::weak_ptr<node>const& x, debugList<T> *i);

        std::weak_ptr<node> val;
        debugList *my;
    };


    debugList();

    void push_back(T const& x);
    void push_front(T const& x);
    void pop_back();
    void pop_front();

    iterator begin();

    iterator end();

    void insert(iterator const& it,T const & val);

    iterator erase(iterator const& it);

    void splice(iterator const& before, debugList<T>& list2, iterator it1, iterator const& it2);

private:
    class node {
    public:
        node(){
            prev = std::weak_ptr<node>();
            next = nullptr;
        }
        node(T const& x, std::weak_ptr<node>const& prv, std::shared_ptr<node>const& nxt)
        {
            obj = x;
            prev = prv;
            next = nxt;
        }

        T obj;
        std::shared_ptr<node> next;
        std::weak_ptr<node> prev;
    };

    int size;
    std::shared_ptr<node> head;
    std::shared_ptr<node> tail;
};

template<typename T>
debugList<T>::debugList(){
    head = std::make_shared<node>(node());
    tail = head;
    size = 0;
}

template<typename T>
void debugList<T>::push_back(const T &x)
{
    insert(end(), x);
}

template<typename T>
void debugList<T>::push_front(const T &x)
{
    insert(begin(), x);
}

template<typename T>
void debugList<T>::pop_back()
{
    assert(size >= 1);
    iterator temp = end();
    temp--;
    erase(temp);
}

template<typename T>
void debugList<T>::pop_front()
{
    erase(begin());
}

template<typename T>
typename debugList<T>::iterator debugList<T>::begin() {
    return iterator(head, this);
}

template<typename T>
typename debugList<T>::iterator debugList<T>::end() {
    return iterator(tail, this);
}
template<typename T>
void debugList<T>::insert(const iterator &it, const T &val)
{
    assert(it.my == this);
    assert(!it.val.expired());

    if(it.val.lock() == tail)
    {
        tail->obj = val;
        tail->next= std::shared_ptr<node>(new node(val, tail, nullptr));
        tail = tail->next;
        size++;
        return;
    }
    if(it.val.lock() == head)
    {
        std::shared_ptr<node> nhead(new node(val, std::weak_ptr<node>(), head));
        head->prev = nhead;
        head = nhead;
        size++;
        return;
    }
    std::shared_ptr<node> next = it.val.lock();
    std::weak_ptr<node> prev = next->prev;
    std::shared_ptr<node> nnode(new node(val,prev,next));
    prev.lock()->next = nnode;
    next->prev = nnode;
    size++;
}
template<typename T>
typename debugList<T>::iterator debugList<T>::erase(const iterator &it)
{
    assert(it.my == this);
    assert(!it.val.expired());
    assert(size > 0);
    assert(it.val.lock() != tail);

    std::shared_ptr<node> next = it.val.lock()->next;
    if(next == tail)
    {
        head = tail;
        size--;
        return end();
    }

    if(it.val.lock() == head)
    {
        head = next;
        head->prev = std::weak_ptr<node>();
        size--;
        return begin();
    }
    std::weak_ptr<node> prev = it.val.lock()-> prev;
    prev.lock() -> next = next;
    next -> prev = prev;
    iterator ret = it;
    ret++;
    size--;
    return ret;
}

template<typename T>
void debugList<T>::splice(const iterator &before, debugList<T> &list2, debugList<T>::iterator it1, const iterator &it2)
{
    assert(it1.my == &list2);
    assert(it2.my == &list2);
    assert(before.my == this);
    assert(!before.val.expired());
    assert(!it1.val.expired());
    assert(!it2.val.expired());
    iterator check = it1;
    while(check.val.lock() != list2.tail && check != it2)
    {
        check++;
    }
    assert(check == it2);
    assert(it1 != it2);
    while(it1.val.lock() != it2.val.lock())
    {
        T temp = it1.val.lock()->obj;
        it1 = list2.erase(it1);
        insert(before, temp);
    }
}

//ITERATOR
template<typename T>
debugList<T>::iterator::iterator(std::weak_ptr<node>const &x, debugList<T> *i) {
    val = x;
    my = i;
}

template<typename T>
T &debugList<T>::iterator::operator*() const
{
    assert(!val.expired());
    assert(val.lock() != my->tail);
    return val.lock()->obj;

}

template<typename T>
typename debugList<T>::iterator &debugList<T>::iterator::operator++()
{
    assert(!val.expired());
    *this = iterator((val.lock()->next), my);
    assert(!val.expired());
    return *this;
}

template<typename T>
typename debugList<T>::iterator debugList<T>::iterator::operator++(int)
{
    assert(!val.expired());
    iterator it = *this;
    ++*this;
    return it;
}

template<typename T>
typename debugList<T>::iterator &debugList<T>::iterator::operator--()
{
    assert(!val.expired());
    *this = iterator(val->prev.lock(), my);
    assert(!val.expired());
    return *this;
}

template<typename T>
typename debugList<T>::iterator debugList<T>::iterator::operator--(int)
{
    assert(!val.expired());
    iterator it = *this;
    --*this;
    return it;
}


template<typename T>
bool debugList<T>::iterator::operator!=(debugList<T>::iterator const &p) const
{
    assert(!val.expired());
    assert(!p.val.expired());
    assert(p.my == this->my);
    return val.lock() != p.val.lock();
}
template<typename T>
bool debugList<T>::iterator::operator==(debugList<T>::iterator const &p) const
{
    return !(val.lock() != p.val.lock());
}


#endif
