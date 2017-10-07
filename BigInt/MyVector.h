#ifndef TRYVECTOR_LIBRARY_H
#define TRYVECTOR_LIBRARY_H

#include<cstdio>
#include<iostream>
#include<memory>
#include<vector>

class MyVector
{
public:
    MyVector();

    MyVector(MyVector const&);

    MyVector(size_t const &x_);
    
    ~MyVector();
    
    void push_back(unsigned int const &);
    
    std::size_t size() const;
    
    void pop_back();
    
    void resize(size_t const&,int const& val = 0);

    unsigned int back() const;

    unsigned int& operator[](int const&);
    unsigned int operator[](int const&) const;

    void operator=(MyVector const&);

    void clear();


    friend void swap(MyVector&, MyVector&);

private:
    std::size_t size_ = 0;
    union
    {
        unsigned int x = 0;
        std::shared_ptr<std::vector<unsigned int>> y;
    };
};

#endif
