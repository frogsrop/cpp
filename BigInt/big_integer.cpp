#include "big_integer.h"

#include <cstring>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <climits>
#include <algorithm>
#include <ctime>
#include <cstdint>

big_integer::big_integer():mpz(1),sign(0){}

big_integer::big_integer(big_integer const& other):mpz(1)
{
    mpz = other.mpz;
    sign = other.sign;
}

big_integer::big_integer(int a):mpz(1)
{
    sign = a < 0;
    if(sign)
    {
        a = ~a;
    }
    mpz[0] = a;
    if(sign)
        mpz[0]++;
}


big_integer::big_integer(std::string const& str)
{
    if (str.size() == 0)
        throw std::runtime_error("empty string");
    sign = 0;
    if (str[0] == '-')
    {
        if (str.size() == 1)
            throw std::runtime_error("empty string");
        sign = 1;
    }
    mpz.push_back(0);
    for (size_t i = sign; i < str.size(); i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            mpz.clear();
            sign = 0;
            throw std::runtime_error("invalid string !");
        }
        unsigned int curNumber = (str[i] - '0');
        unsignedMulLongShort(*this, 10);
        unsignedAddLongShort(*this, curNumber);
    }
    this->normilize();
}

big_integer::~big_integer()
{}

big_integer& big_integer::operator=(big_integer const& other)
{
    mpz = other.mpz;
    sign = other.sign;
    return *this;
}

big_integer& big_integer::operator+=(big_integer const& rhs)
{
    if(sign == rhs.sign)
    {
        if(rhs.mpz.size() == 1)
        {
            unsignedAddLongShort(*this, rhs.mpz[0]);
        }
        else
        {
            unsignedSum(*this, rhs);
        }
    }
    else
        if (sign == 0)
        {
            if(unsignedCompare(mpz, rhs.mpz) >= 0)
            {
                unsignedSub(mpz, rhs.mpz);
            }
            else
            {
                MyVector buf = rhs.mpz;
                unsignedSub(buf, mpz);
                mpz = buf;
                sign = 1;
            }
        }
        else
        {
            if(unsignedCompare(mpz, rhs.mpz) > 0)
            {
                unsignedSub(mpz, rhs.mpz);
            }
            else
            {
                MyVector buf = rhs.mpz;
                unsignedSub(buf, mpz);
                mpz = buf;
                sign = 0;
            }
        }
    this->normilize();
    return *this;
}

big_integer& big_integer::operator-=(big_integer const& rhs)
{
    sign ^= 1;
    *this += rhs;
    sign ^= 1;
    normilize();
    return *this;
}

big_integer& big_integer::operator*=(big_integer const& rhs)
{
    bool sSign = this->sign ^ rhs.sign;
    big_integer A = *this;
    big_integer B = rhs;
    if(!unsignedCompare(A.mpz, B.mpz))
    {
        swap(B.mpz,A.mpz);
    }
    if(B.mpz.size() == 1)
    {
        unsignedMulLongShort(A, B.mpz[0]);
        A.sign = sSign;
        *this = A;
        normilize();
        return *this;
    }
    big_integer C = 0;
    C.mpz.resize(A.mpz.size() + B.mpz.size());
    unsigned long long carry = 0;
    for(size_t i = 0; i < A.mpz.size(); i++)
    {
        for(size_t j = 0; j < B.mpz.size() || carry; j++)
        {
            unsigned long long mul = C.mpz[i + j] + A.mpz[i] * 1ull * (j < B.mpz.size() ? B.mpz[j] : 0) + carry;
            C.mpz[i + j] = mul & 0xFFFFFFFF;
            carry = mul>>32;
        }
    }
    *this = C;
    this->sign = sSign;
    normilize();
    return *this;
}

big_integer& big_integer::operator/=(big_integer const& rhs)
{
    if(rhs.mpz.size() == 1)
    {
        unsignedDivLongShort(*this, rhs.mpz[0]);
        this->sign = sign ^ rhs.sign;
        return *this;
    }
    big_integer A = *this;
    big_integer B = rhs;
    this->sign = A.sign ^ B.sign;
    A.sign = 0;
    B.sign = 0;
    if(B > A)
    {
        *this = 0;
        return *this;
    }
    unsigned int k = 0x100000000ull / (B.mpz.back() + 1);
    unsignedMulLongShort(B, k);
    unsignedMulLongShort(A, k);
    unsigned int n = B.mpz.size();
    unsigned int m = A.mpz.size() - n;
    MyVector q;
    q.resize(m + 1, 0);
    if(!B.check(A, m))
    {
        q[m] = 1;
        A.cut(B, m);
    }
    for(size_t j = m; j--;)
    {
        unsigned long long tempQ = 0;
        if(n + j < A.mpz.size())
            tempQ = A.mpz[n + j] * 0x100000000ull;
        if(n + j < A.mpz.size() + 1 && n + j >= 1)
            tempQ += A.mpz[n + j - 1];
        tempQ /= B.mpz[n - 1];
        if(tempQ > 0xFFFFFFFFull)
            tempQ = 0xFFFFFFFFull;
        big_integer cur = B;
        unsignedMulLongShort(cur, tempQ);
        while(cur.check(A, j))
        {
            cur -= B;
            tempQ--;
        }
        A.cut(cur, j);
        A.normilize();
        q[j]=tempQ;
    }
    mpz = q;
    normilize();
    return *this;
}
big_integer& big_integer::operator%=(big_integer const& rhs)
{
    if(rhs.mpz.size() == 1)
    {
        unsigned int ans = unsignedDivLongShort(*this, rhs.mpz[0]);
        mpz.clear();
        mpz.push_back(ans);
        normilize();
        return *this;
    }
    big_integer A = *this;
    big_integer B = rhs;
    bool tempSign = A.sign;
    A.sign = 0;
    B.sign = 0;
    if(B > A)
    {
        normilize();
        return *this;
    }
    unsigned int k = 0x100000000ull / (B.mpz.back() + 1);
    unsignedMulLongShort(B, k);
    unsignedMulLongShort(A, k);
    unsigned int n = B.mpz.size();
    unsigned int m = A.mpz.size() - n;
    if(!B.check(A, m))
    {
        A.cut(B, m);
    }
    for(size_t j = m; j--;)
    {
        unsigned long long tempQ = 0;
        if(n + j < A.mpz.size())
            tempQ = A.mpz[n + j] * 0x100000000ull;
        if(n + j < A.mpz.size() + 1 && n + j >= 1)
            tempQ += A.mpz[n + j - 1];
        tempQ /= B.mpz[n - 1];
        if(tempQ > 0xFFFFFFFFull)
            tempQ = 0xFFFFFFFFull;
        big_integer cur = B;
        unsignedMulLongShort(cur, tempQ);
        while(cur.check(A, j))
        {
            cur -= B;
        }
        A.cut(cur, j);
        A.normilize();
    }
    *this = A;
    sign = tempSign;
    normilize();
    return *this;
}

big_integer& big_integer::operator&=(big_integer const& rhs)
{
    MyVector t = this->getNumber();
    MyVector tr = rhs.getNumber();
    for(size_t i = 0;i<t.size();i++)
    {
        t[i] &= (i<tr.size()?tr[i]:(sign*1u*0xFFFFFFFF));
    }
    sign &= rhs.sign;
    mpz = t;
    normilize();
    if(sign)
    {
        for(size_t i=0;i<mpz.size();i++)
        {
            mpz[i]=~mpz[i];
        }
        (*this)--;
    }
    normilize();
    return *this;
}

big_integer& big_integer::operator|=(big_integer const& rhs)
{
    MyVector t = (*this).getNumber();
    MyVector tr = rhs.getNumber();
    t.resize(std::max(t.size(),tr.size()),sign*1u*0xFFFFFFFF);
    for(size_t i = 0; i < std::max(t.size(),tr.size());i++)
    {
        t[i] |= (i<tr.size()?tr[i]:(rhs.sign*1u*0xFFFFFFFF));
    }
    sign |= rhs.sign;
    mpz = t;
    normilize();

    if(sign)
    {
        for(size_t i=0;i<mpz.size();i++)
        {
            mpz[i]=~mpz[i];
        }
        (*this)--;
    }
    normilize();
    return *this;
}

big_integer& big_integer::operator^=(big_integer const& rhs)
{
    MyVector t = this->getNumber();
    MyVector tr = rhs.getNumber();
    t.resize(std::max(t.size(),tr.size()),sign*1u*0xFFFFFFFF);
    for(size_t i = 0;i<std::max(t.size(),tr.size());i++)
    {
        t[i] ^= (i<tr.size()?tr[i]:(rhs.sign*1u*0xFFFFFFFF));
    }
    sign ^= rhs.sign;
    mpz = t;
    normilize();
    if(sign)
    {
        for(size_t i=0;i<mpz.size();i++)
        {
            mpz[i]=~mpz[i];
        }
        (*this)--;
    }
    normilize();
    return *this;
}

big_integer& big_integer::operator<<=(int rhs)
{
    if(rhs < 0)
    {
        *this>>=-rhs;
        return *this;
    }
    mpz = getNumber();
    int full = rhs / 32;
    int part = rhs % 32;
    if(part)
    {
        unsigned int carry = 0;
        for(size_t i = 0; i < mpz.size() || carry; i++)
        {
            if(mpz.size()==i)
            {
                mpz.push_back(0);
            }
            unsigned int buf = mpz[i];
            mpz[i] <<= part;
            mpz[i] += carry;
            carry = buf>>(32-part);
        }
    }
    if(sign)
    {
        mpz = inv();
        unsignedAddLongShort(*this, 1);
    }
    if(full)
    {
        mpz.resize(mpz.size() + full, 0);
        rotate(mpz, mpz.size() - full, full);
        //rotate(mpz.begin(), mpz.begin() + mpz.size() - full, mpz.end());
    }
    normilize();
    return *this;
}
big_integer& big_integer::operator>>=(int rhs)
{
    if(rhs < 0)
    {
        *this<<=-rhs;
        return *this;
    }
    mpz = getNumber();
    int full = rhs / 32;
    int part = rhs % 32;
    rotate(mpz, full, mpz.size() - full);
    //rotate(mpz.begin(), mpz.begin() + full % mpz.size(), mpz.end());
    if(full >= (int)mpz.size())
    {
        mpz.clear();
        mpz.push_back(0);
        return *this;
    }
    else
        mpz.resize(mpz.size() - full, 0);
    if(sign)
        mpz.push_back(0xFFFFFFFFu);
    if(part)
    {
        unsigned int carry = 0;
        for(size_t i = mpz.size(); i--;)
        {
            unsigned int buf = mpz[i];
            mpz[i]>>=part;
            mpz[i]+=carry;
            carry = buf<<(32-part);
        }
    }
    if(sign)
    {
        mpz = inv();
        unsignedAddLongShort(*this, 1);
        mpz.pop_back();
    }
    else
    {
        sign = 0;
    }
    normilize();
    return *this;
}

big_integer big_integer::operator+() const
{
    return *this;
}

big_integer big_integer::operator-() const
{
    big_integer temp = *this;
    if(temp != 0)
        temp.sign ^= 1;
    return temp;
}

big_integer big_integer::operator~() const
{
    big_integer r = *this;
    if(r.sign)
    {
        MyVector q(1);
        q[0] = 1;
        big_integer::unsignedSub(r.mpz, q);
    }
    else
        big_integer::unsignedSum(r, 1);
    r.sign^=1;
    r.normilize();
    return r;
}

big_integer& big_integer::operator++()
{
    *this+=1;
    return *this;
}

big_integer big_integer::operator++(int)
{
    big_integer r = *this;
    ++*this;
    return r;
}

big_integer& big_integer::operator--()
{
    *this-=1;
    return *this;
}

big_integer big_integer::operator--(int)
{
    big_integer r = *this;
    --*this;
    return r;
}

void big_integer::unsignedMulLongShort(big_integer &num1, unsigned int const &b)
{
    MyVector& a = num1.mpz;
    unsigned long long carry = 0;
    a.push_back(0);
    for (size_t i = 0; i < a.size() || carry; i++)
    {
        unsigned long long add = a[i] * 1ull * b + carry;
        a[i] = add;
        carry = add >> 32;
    }
     while (a.size() > 1 && a.back() == 0)
        a.pop_back();
}



void big_integer::unsignedAddLongShort(big_integer &num1, const unsigned int &b)
{
    MyVector & a = num1.mpz;
    unsigned long long add = a[0]*1ull + b;
    bool carry = add>>32;
    a[0] = add;
    a.push_back(0);
    for(size_t i = 1; carry; i++)
    {
        add = a[i]*1ull + carry;
        a[i] = add;
        carry = add>>32;
    }
    if(a.back()==0)
    {
        a.pop_back();
    }
}


unsigned int big_integer::unsignedDivLongShort(big_integer &num1, const unsigned int &b)
{
    unsigned int carry = 0;
    MyVector &a = num1.mpz;
    for (size_t i = a.size(); i--;) {
        unsigned long long cur = a[i] + carry * 1ull * 0x100000000;
        a[i] = (unsigned int)(cur / b);
        carry = (unsigned int)(cur % b);
    }
    num1.normilize();
    return carry;
}

int big_integer::unsignedCompare(MyVector const&a, MyVector const&b)
{
    if(a.size() > b.size())
        return 1;
    if(a.size() < b.size())
        return -1;
    for(size_t i = a.size(); i--;)
    {
        if(a[i] != b[i])
            return (a[i] > b[i]) * 2 - 1;
    }
    return 0;
}

void big_integer::unsignedSum(big_integer &num1, big_integer const &num2)
{
    bool carry = 0;
    MyVector &a = num1.mpz;
    MyVector const &b = num2.mpz;
    a.resize(std::max(b.size(), a.size()) + 1, 0);
    for (size_t i = 0; i < b.size() || carry; i++)
    {
        unsigned long long sum = a[i]*1ull +  (i < b.size() ? b[i] : 0) + carry;
        a[i] = sum;
        carry =  sum >> 32;
    }
    num1.normilize();
}

void big_integer::unsignedSub(MyVector &a, MyVector const&b)
{
    unsigned int carry = 0;
    for (size_t i=0; i < b.size() || carry; ++i)
    {
        long long res = a[i] * 1ll - carry - (i < b.size() ? b[i] : 0);
        a[i] = res + 0x100000000ll * (carry = res < 0);
    }
    while (a.size() > 1 && a.back() == 0)
        a.pop_back();
}

MyVector big_integer::inv() const
{
    big_integer t = *this;
    for(size_t i = 0;i < t.mpz.size(); i++)
    {
        t.mpz[i] = ~t.mpz[i];
    }
    return t.mpz;
}

MyVector big_integer::getNumber() const
{
    big_integer t = *this;
    if(t == 0)
        return t.mpz;
    if(t.sign)
    {
        t.mpz = inv();
        t--;
    }
    t.normilize();
    return t.mpz;

}

void big_integer::normilize()
{
    int j = mpz.size() - 1;
    while (j>0 && mpz[j] == 0)
        j--;
    mpz.resize(j + 1);
    if(mpz.size()==1 && mpz[0] == 0)
        sign = 0;
    //MyVector(mpz).swap(mpz);
}

void big_integer::cut(const big_integer &b, size_t index)
{
    bool borrow = 0;
    big_integer &a = *this;
    a.mpz.resize(std::max(a.mpz.size(),b.mpz.size()));
    for (size_t i = 0; i < b.mpz.size() || borrow; i++)
    {
        unsigned int x = (index + i < a.mpz.size() ? a.mpz[index + i] : 0);
        unsigned int y = (i < b.mpz.size() ? b.mpz[i] : 0);
        int64_t c = (long long) x - y - borrow;
        if (c < 0) {
            c += 0xFFFFFFFFu + 1;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }
        a.mpz[index + i] = (unsigned int) c;
    }
    a.normilize();
}

bool big_integer::check(const big_integer &b, size_t index)
{
    big_integer& a = *this;
    if(a.mpz.size() + index < b.mpz.size())
    {
        return 0;
    }
    for(size_t i = a.mpz.size(); i--;)
    {
        unsigned int cmpTo = ((i + index) < b.mpz.size() ? b.mpz[i + index] : 0);
        if(a.mpz[i] != cmpTo)
            return a.mpz[i] > cmpTo;
    }
    return 0;
}

void big_integer::reverse(MyVector &a, int l, int r)
{
    for(int i = 0; i<(l+r)/2; i++)
        std::swap(a[l + i], a[r - i]);
}

void big_integer::rotate(MyVector &a, int l, int zeros)
{
    reverse(a, 0, l - 1);
    reverse(a, l, l + zeros - 1);
    reverse(a, 0, l + zeros - 1);
}


big_integer operator+(big_integer a, big_integer const& b)
{
    return a += b;
}

big_integer operator-(big_integer a, big_integer const& b)
{
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const& b)
{
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const& b)
{
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const& b)
{
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const& b)
{
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const& b)
{
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const& b)
{
    return a ^= b;
}

big_integer operator<<(big_integer a, int b)
{
    return a <<= b;
}

big_integer operator>>(big_integer a, int b)
{
    return a >>= b;
}

bool operator==(big_integer const& a, big_integer const& b)
{
    if(a.sign == b.sign)
    {
        return big_integer::unsignedCompare(a.mpz, b.mpz) == 0;
    }
    return 0;
}

bool operator!=(big_integer const& a, big_integer const& b)
{
    if(a.sign == b.sign)
    {
        return big_integer::unsignedCompare(a.mpz, b.mpz) != 0;
    }
    return 1;
}

bool operator<(big_integer const& a, big_integer const& b)
{
    if(a.sign == b.sign)
    {
        if(a.sign)
            return big_integer::unsignedCompare(a.mpz, b.mpz)==1;
        else
            return big_integer::unsignedCompare(a.mpz, b.mpz)==-1;
    }
    else
    {
        return a.sign > b.sign;
    }
}

bool operator>(big_integer const& a, big_integer const& b)
{
    if(a.sign == b.sign)
    {
        if(a.sign)
            return big_integer::unsignedCompare(a.mpz, b.mpz)==-1;
        else
            return big_integer::unsignedCompare(a.mpz, b.mpz)==1;
    }
    else
    {
        return a.sign < b.sign;
    }
}

bool operator<=(big_integer const& a, big_integer const& b)
{
    if(a.sign == b.sign)
    {
        if(a.sign)
            return big_integer::unsignedCompare(a.mpz, b.mpz)>=0;
        else
            return big_integer::unsignedCompare(a.mpz, b.mpz)<=0;
    }
    else
    {
        return a.sign > b.sign;
    }


}

bool operator>=(big_integer const& a, big_integer const& b)
{
    if(a.sign == b.sign)
    {
        if(a.sign)
            return big_integer::unsignedCompare(a.mpz, b.mpz)<=0;
        else
            return big_integer::unsignedCompare(a.mpz, b.mpz)>=0;
    }
    else
    {
        return a.sign > b.sign;
    }
}

std::string to_string(big_integer const& a)
{
    std::string res;
    big_integer v = a;
    while(!(v.mpz.size() == 1 && v.mpz[0] == 0))
    {
        unsigned int i = big_integer::unsignedDivLongShort(v, 10);
        res+=char(i + '0');
    }
    if(a.sign == 1)
        res+='-';
    if(res.size()==0)
        res = "0";
    std::reverse(res.begin(),res.end());
    return res;
}

std::ostream& operator<<(std::ostream& s, big_integer const& a)
{
    return s << to_string(a);
}

