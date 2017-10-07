#include "MyVector.h"

MyVector::MyVector()
{}

MyVector::MyVector(MyVector const & x_)
{
    size_ = x_.size_;
    if(size_ == 1)
    {
        x = x_.x;
    }
    else
    {
        new (&y) std::shared_ptr<std::vector<unsigned int>>(new std::vector<unsigned int>);
        y = x_.y;
    }
}

MyVector::MyVector(std::size_t const &x_)
{
    if(x_ == 1)
    {
        size_ = x_;
    }
    else
    {
        new (&y) std::shared_ptr<std::vector<unsigned int>>(new std::vector<unsigned int>);
        (*y.get()).resize(x_);
        size_ = x_;
    }
}

MyVector::~MyVector()
{
    if(size_ > 1)
    {
        y.~__shared_ptr();
    }
}

void MyVector::push_back(unsigned int const &x_)
{
    if (size_ == 0)
    {
        x = x_;
    }
    else
    if (size_ == 1)
    {
        unsigned int cur = x;
        new (&y) std::shared_ptr<std::vector<unsigned int>>(new std::vector<unsigned int>);
        std::vector<unsigned int>& yy = (*y.get());
        yy.resize(2);
        yy[0] = cur;
        yy[1] = x_;
    }
    else
    {
        if(y.use_count()>1)
        {
            std::vector<unsigned int>& u = *y.get();
            y.reset(new std::vector<unsigned int>);
            (*y.get()) = u;
        }
        (*y.get()).push_back(x_);
    }
    size_++;
}

std::size_t MyVector::size() const
{
    return size_;
}

void MyVector::pop_back()
{
    if (size_ == 0) { return; }
    if (size_ == 2)
    {
        unsigned int cur = (*y.get())[0];
        y.~__shared_ptr();
        x = cur;
    }
    else
    {
        if(y.use_count()>1)
        {
            std::vector<unsigned int>& u = *y.get();
            y.reset(new std::vector<unsigned int>);
            (*y.get()) = u;
        }
        (*y.get()).pop_back();
    }
    size_--;
}
void MyVector::resize(size_t const & z, int const & val)
{
    if(z == 0)
    {
        if(size_ > 1)
            y.~__shared_ptr();
        x = 0;
        size_ = 0;
    }
    else
    {
        if(z == 1)
        {
            if(z < size_)
            {
                unsigned int temp = (*y.get())[0];
                y.~__shared_ptr();
                x = temp;
            }
            size_ = z;
        }
        else
        {
            if(size_ == 0)
            {
                new (&y) std::shared_ptr<std::vector<unsigned int>>(new std::vector<unsigned int>);
                (*y.get()).resize(z);
            }
            else
            if(size_ == 1)
            {
                unsigned int temp = x;
                new (&y) std::shared_ptr<std::vector<unsigned int>>(new std::vector<unsigned int>);
                (*y.get()).resize(z);
                (*y.get())[0] = temp;
            }
            else
            {
                if(y.use_count()>1)
                {
                    std::vector<unsigned int>& u = *y.get();
                    y.reset(new std::vector<unsigned int>);
                    (*y.get()) = u;
                }
                (*y.get()).resize(z);
            }
            size_ = z;
        }
    }

}

unsigned int MyVector::back() const
{
    if (size_ == 0)
    {
        throw std::runtime_error("Empty vector");
    }
    if (size_ == 1)
    {
        return x;
    }
    else
    {
        return (*y.get()).back();
    }
}

unsigned int &MyVector::operator[](const int &z)
{
    if(size_ == 1)
        return x;
    else
    {
        //COW
        if(y.use_count()>1)
        {
            std::vector<unsigned int>& u = *y.get();
            y.reset(new std::vector<unsigned int>);
            (*y.get()) = u;
        }
        return (*y.get())[z];
    }
}

unsigned int MyVector::operator[](const int &z) const
{
    if(size_ == 1)
        return x;
    else
    {
        return ((*y.get())[z]);
    }
}

void MyVector::operator=(MyVector const& b)
{
    if(b.size_ <= 1)
    {
        if(size_>1)
            y.~__shared_ptr();
        x = b.x;
    }
    else
    {
        if(size_ <= 1)
        {
            new (&y) std::shared_ptr<std::vector<unsigned int>>(new std::vector<unsigned int>);
        }
        //(*y.get()) = (*(b.y.get()));
        //COW
        if(y.use_count()>1)
        {
            std::vector<unsigned int>& u = *y.get();
            y.reset(new std::vector<unsigned int>);
            (*y.get()) = u;
        }
        y = b.y;
    }
    size_ = b.size_;
}

void MyVector::clear()
{
    x = 0;
    size_ = 0;
}

void swap(MyVector& a, MyVector& b)
{
    std::swap(a.x,b.x);
    std::swap(a.size_,b.size_);
}
