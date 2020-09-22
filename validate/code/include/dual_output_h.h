#ifndef DUAL_OUTPUT_H
#define DUAL_OUTPUT_H

#include <streambuf>

class teebuf
    : public std::streambuf
{
    std::streambuf* sb1_;
    std::streambuf* sb2_;

    int overflow(int c);
    int sync();
public:
    teebuf(std::streambuf* sb1, std::streambuf* sb2)
        : sb1_(sb1), sb2_(sb2) {
    }
};

class oteestream
    : private virtual teebuf
    , public std::ostream {
public:
    oteestream(std::ostream& out1, std::ostream& out2)
        : teebuf(out1.rdbuf(), out2.rdbuf())
        , std::ostream(this) {
        this->init(this);
    }
};


#endif 
