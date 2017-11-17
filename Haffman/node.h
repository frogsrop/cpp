#include <cstdint>
#include <memory>
class node {
public:
    std::unique_ptr<node> l, r;
    int16_t val = -1;
    node();
    node(int16_t v);
    node(int16_t val1, std::unique_ptr<node>& x, std::unique_ptr<node>& y);
    ~node();
};
