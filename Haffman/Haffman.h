#ifndef Haffman
#define Haffman

#include <cstdint>
#include <node.h>
#include <byte.h>
#include <string>
#include <vector>

class haffman{
private:
    static const std::size_t size = 512;
    unsigned long long amount[size];
    byte code[size];
    std::unique_ptr<node> root = nullptr;
public:
    ~haffman();
    haffman();
    void incSymbol(uint_fast8_t, unsigned int val = 1);
    byte getCode(unsigned int x);
    void build();
    void strToCode(std::string);
    unsigned long long * getinf();
    std::pair<node *, unsigned char> shiftDown(uint_fast8_t num, node *v = nullptr);

private:
    void genCodes(node*, byte);
};


#endif // Haffman
