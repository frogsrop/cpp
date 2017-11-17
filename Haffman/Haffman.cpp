#include <Haffman.h>
#include <queue>
#include <iostream>
#include <math.h>

haffman::~haffman()
{
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
    try {
    if(UINT64_MAX - amount[x] < val )
        throw std::runtime_error("File is too big");
    amount[x] += val;
    }
    catch(std::runtime_error e)
    {
        e.what();
    }
}

struct compare
{
    bool operator()(const std::pair<unsigned long long, std::unique_ptr<node>>& l, const std::pair<unsigned long long, std::unique_ptr<node>>& r)
    {
        if(l.first == r.first)
        {
            return l.second.get()->val > l.second.get()->val;
        }
        return l.first > r.first;
    }
};

void haffman::build()
{
    std::priority_queue<unsigned int,std::vector<std::pair<unsigned long long, std::unique_ptr<node>>>, compare> q;
    for(unsigned int i = 0; i < size; i++)
    {
        if(amount[i]>0)
        {
            q.push({amount[i], std::unique_ptr<node>(new node(i))});
        }
    }
    if(q.size() == 1)
    {
        //could not work
        std::unique_ptr<node> temp(std::move(q.top().second.get()));
        q.pop();
        root.reset(temp);
        code[root.get()->val] = byte(0,1);
    }
    int w = 1;
    while(q.size() > 1)
    {
        std::cout<<"started\n"<<w<<std::endl;
        std::pair<unsigned int, std::unique_ptr<node>> x;
        x.first = q.top().first;
        x.second.reset(std::move(q.top().second.get()));
        q.pop();
        std::cout<<'!'<<std::endl;
        std::pair<unsigned int, std::unique_ptr<node>> y;
        y.first = q.top().first;
        y.second.reset(std::move(q.top().second.get()));
        q.pop();
        std::cout<<'?'<<std::endl;
        std::unique_ptr<node> temp(new node(-1, std::move(x.second), std::move(y.second)));
        q.push({x.first + y.first, std::move(temp)});
        std::cout<<"finished"<<std::endl;
        w++;
    }
    if(q.size())
    {
        genCodes(q.top().second.get(), byte(0,0));
        std::unique_ptr<node> temp(std::move(q.top().second.get()));
        q.pop();
        root.reset(temp.release());
    }
}

void haffman::genCodes(node* v, byte b)
{
    if(v->l.get() == nullptr && v->r.get() == nullptr)
    {
        code[v->val] = b;
    }
    byte temp = b;
    if(v->l.get() != nullptr)
    {
        b = b.add(0);
        genCodes(v->l.get(), b);
    }
    b = temp;
    if(v->r.get() != nullptr)
    {
        b = b.add(1);
        genCodes(v->r.get(), b);
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
        v = root.get();
    }
    if(v->r.get() == nullptr && v->l.get() == nullptr)
    {
        return {nullptr, v->val};
    }
    if(num == 1)
    {
        if(v->r.get()->l.get() == v->r.get()->r.get()  && v->r.get()->r.get() == nullptr)
            return {nullptr, v->r.get()->val};
        else
            return {v->r.get(), '.'};
    }
    else
    {
        if(v->l.get()->l.get() == v->l.get()->r.get()  && v->l.get()->r.get() == nullptr)
            return {nullptr, v->l.get()->val};
        else
            return {v->l.get(), '.'};
    }
    
}
