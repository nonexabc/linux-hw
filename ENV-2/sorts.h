#ifndef CMPX_SORT
#define CMPX_SORT
#include <vector>
#include <array>
#include "cn.h"
#include <algorithm>

void sortComplex(std::vector<ComplexNumber>& obj);

template<size_t N> // templates work poorly with impl/decl separation, the "make" command didn't work
void sortComplex(std::array<ComplexNumber, N>& obj){
    std::sort(obj.begin(), obj.end());
}

#endif
