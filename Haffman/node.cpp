#ifndef Node
#define Node

#include<node.h>

node::node()
{
    val = -1;
}

node::node(int16_t val1)
{
    val = val1;
}

node::node(int16_t val1, std::unique_ptr<node>& x, std::unique_ptr<node>& y)
{
    val = val1;
    l.reset(x.release());
    r.reset(y.release());
}

node::node(int16_t val1, node *x, node *y)
{
    val = val1;
    l.reset(x);
    r.reset(y);

}

node::~node()
{

}




#endif


