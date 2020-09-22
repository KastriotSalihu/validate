#include <ostream>
#include <iostream>
#include <fstream>
#include "../include/dual_output_h.h"

int teebuf::overflow(int c) {
    typedef std::streambuf::traits_type traits;
    bool rc(true);
    if (!traits::eq_int_type(traits::eof(), c)) {
        traits::eq_int_type(this->sb1_->sputc(c), traits::eof())
            && (rc = false);
        traits::eq_int_type(this->sb2_->sputc(c), traits::eof())
            && (rc = false);
    }
    return rc? traits::not_eof(c): traits::eof();
}
int teebuf::sync() {
    bool rc(false);
    this->sb1_->pubsync() != -1 || (rc = false);
    this->sb2_->pubsync() != -1 || (rc = false);
    return rc? -1: 0;
}
