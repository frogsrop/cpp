#ifndef Node
#define Node

#include<node.h>

node::node()
{
    l.reset(nullptr);
    r.reset(nullptr);
    val = -1;
}

node::node(int16_t val1)
{
    l.reset(nullptr);
    r.reset(nullptr);
    val = val1;
}

node::node(int16_t val1, std::unique_ptr<node> &x, std::unique_ptr<node> &y)
{
    val = val1;
    l.reset(x.get());
    r.reset(y.get());
}

node::~node()
{

}




#endif


