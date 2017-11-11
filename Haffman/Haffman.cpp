#include <Haffman.h>
#include <queue>
#include <iostream>
#include <math.h>

haffman::~haffman()
{
    delete(root);
}

haffman::haffman()
{
    for(int i = 0; i<size; i++)
    {
        amount[i] = 0;
    }
}

byte haffman::getCode(unsigned int x)
{
    return code[x];
}

void haffman::incSymbol(uint8_t x, unsigned  val)
{
    if(UINT64_MAX - amount[x] < val )
        std::cout<<'!';
    amount[x] += val;
}

struct compare
{
    bool operator()(const std::pair<unsigned int, node*>& l, const std::pair<unsigned int, node*>& r)
    {
        if(l.first == r.first)
        {
            return l.second->val > l.second->val;
        }
        return l.first > r.first;
    }
};

void haffman::build()
{
    std::priority_queue<unsigned int,std::vector<std::pair<unsigned long long, node*>>, compare> q;
    for(unsigned int i = 0; i < size; i++)
    {
        if(amount[i]>0)
        {
            node* temp = new node(i, nullptr, nullptr, nullptr);
            q.push({amount[i], temp});
        }
    }
    if(q.size() == 1)
    {
        node* temp = q.top().second;
        q.pop();
        root = temp;
        code[root->val] = byte(0,1);
    }
    while(q.size() > 1)
    {
        std::pair<unsigned int, node*> x = q.top();
        q.pop();
        std::pair<unsigned int, node*> y = q.top();
        q.pop();
        node* temp = node::unite(x.second, y.second);
        q.push({x.first + y.first, temp});
    }
    if(q.size())
    {
        genCodes(q.top().second, byte(0,0));
        node* temp = q.top().second;
        q.pop();
        root = temp;
    }
}

void haffman::genCodes(node* v, byte b)
{
    if(v->l == nullptr && v->r == nullptr)
    {
        code[v->val] = b;
    }
    byte temp = b;
    if(v->l != nullptr)
    {
        b = b.add(0);
        genCodes(v->l, b);
    }
    b = temp;
    if(v->r != nullptr)
    {
        b = b.add(1);
        genCodes(v->r, b);
    }
}


void haffman::strToCode(std::string s)
{
    for(int i = 0; i < s.size(); i++)
    {
        incSymbol(s[i]);
    }
    build();

    for(int i = 0; i < s.size(); i++)
    {
        //code[s[i]].out();
    }
}

unsigned long long* haffman::getinf()
{
    return amount;
}

std::pair<node*, unsigned char> haffman::shiftDown(uint_fast8_t num, node *v)
{
    if(v == nullptr)
    {
        v = root;
    }
    if(v->r == nullptr && v->l == nullptr)
    {
        return {nullptr, v->val};
    }
    if(num == 1)
    {
        if(v->r->l == v->r->r  && v->r->r == nullptr)
            return {nullptr, v->r->val};
        else
            return {v->r, '.'};
    }
    else
    {
        if(v->l->l == v->l->r  && v->l->r == nullptr)
            return {nullptr, v->l->val};
        else
            return {v->l, '.'};
    }
    
}
