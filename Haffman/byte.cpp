#include<byte.h>

byte::byte()
{
    used = 0;
    num = std::vector<uint_fast8_t>(1);
}

byte::byte(const uint_fast8_t &val, uint_fast8_t size)
{
    used = size;
    num.push_back(0);
    num[0] = (unsigned char) val;
}

byte::byte(const std::vector<uint_fast8_t> &val, uint_fast8_t size)
{
    used = size;
    num = val;
}

byte::byte(const byte &x)
{
    used = x.used;
    num = x.num;
}


byte byte::add(uint_fast8_t x)
{
    byte temp(*this);
    if(used == 0)
    {
        temp.num[0] = x;
        temp.used = 1;
        return temp;
    }
    if(used < 8)
    {
        temp.num[temp.num.size() - 1] = (unsigned char) ((temp.num[temp.num.size() - 1] << 1) | (x & 1));
        temp.used++;
    }
    else
    {
        temp.num.push_back(0);
        temp.used  = 1;
        temp.num[temp.num.size() - 1] = x;
    }
    return temp;
}

byte byte::pop()
{
    if(used == 0)
        return *this;
    if(used == 1)
    {
        num.pop_back();
        used = 8;
    }
    else
    {
        num[num.size() - 1]>>=1;
        used--;
    }
}

uint_fast8_t byte::used_()
{
    return used;
}

std::vector<uint_fast8_t> byte::num_()
{
    return num;
}

void byte::sum(byte b)
{
    if(used == 0)
    {
        *this = b;
        return;
    }
    
    auto num2 = b.num;
    auto used2 = b.used;
    
    int pos = num.size();
    num2[num2.size()-1] <<= (8-used2);
    for(int i = 0; i < num2.size(); i++)
    {
        num.push_back(num2[i]);
    }
    for(int i = pos; i < num.size(); i++)
    {
            num[i - 1] <<= (8 - used);
            num[i - 1] += (num[i]>>used);
            num[i] &= (1<<used)-1;
    }
    if(used+used2!=16)
    if(used + used2 <= 8)
    {
        num.pop_back();
        num.back() >>= (8 - used - used2);
    }
    else
    {
        num.back() >>= (8-used2);
    }
    used = (uint_fast8_t) ((used + used2) % 8);
    if(used == 0)
        used = 8;
}

