#include <cstdint>
#include <iostream>
#include <vector>

class byte {
private:
    uint_fast8_t used;
    std::vector<uint_fast8_t> num;
public:
    byte();
    
    byte(uint_fast8_t const& val, uint_fast8_t size = 1);
    byte(std::vector<uint_fast8_t> const& val, uint_fast8_t size = 1);
    
    byte(byte const& x);
    ~byte(){}
    byte add(uint_fast8_t x);
    byte pop();
    uint_fast8_t used_();
    std::vector<uint_fast8_t> num_();
    void sum(byte);/*
    void out()
    {
        std::cout<<"{"<<num.size()<<"} ";
        for(int j = 0; j < num.size(); j++)
        {
            if(j != num.size() - 1)
            {
                for(std::size_t i = 8; i>=1; i--)
                {
                    std::cout<<((num[j]>>(i-1))&1)<<' ';
                }
            }
            else
                for(std::size_t i = used; i>=1; i--)
                {
                    std::cout<<((num[j]>>(i-1))&1)<<' ';
                }
            std::cout<<',';
        }
    }*/
};
