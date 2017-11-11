#ifndef Node
#define Node

#include<node.h>


node::node()
{
    val = -1;
    l = nullptr;
    r = nullptr;
    p = nullptr;
}

node::node(int16_t val1, node *x, node *y, node *pz)
{
    val = val1;
    l = x;
    r = y;
    p = pz;
}

node::~node()
{
    if(l != nullptr)
    {
        delete(l);
    }
    if(r != nullptr)
    {
        delete(r);
    }
}

node* node::unite(node *x, node *y)
{
    node *temp = new node(-1, x, y, nullptr);
    x -> p = temp;
    y -> p = temp;
    return  temp;
}

#endif
