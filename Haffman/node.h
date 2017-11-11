#include <cstdint>

class node {
public:
    node *l = nullptr, *r = nullptr, *p = nullptr;
    int16_t val = -1;
    node();
    ~node();
    static node* unite(node* x, node* y);
    node(int16_t val1, node *x, node *y, node *pz);
};
