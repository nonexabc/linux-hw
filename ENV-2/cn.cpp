#include "cn.h"
#include <cmath>
#include <random>

ComplexNumber ComplexNumber::operator+(_cref other) const {
    return ComplexNumber(_r+other.real(), _i+other.imaginary());
}

ComplexNumber ComplexNumber::operator-(_cref other) const {
    return ComplexNumber(_r-other.real(), _i-other.imaginary());
}

ComplexNumber ComplexNumber::operator*(_type q) const {
    return ComplexNumber(_r*q, _i*q);
}

_type ComplexNumber::abs() const {
    return sqrt(_r*_r + _i*_i);    
}

bool ComplexNumber::operator>(_cref other) const {
    return ((_r == other.real()) ? (_i > other.imaginary()) : (_r > other.real()));
}

bool ComplexNumber::operator<(_cref other) const {
    return ((_r == other.real()) ? (_i < other.imaginary()) : (_r < other.real()));
}

_type& ComplexNumber::real() {
    return _r;
}

_type& ComplexNumber::imaginary() {
    return _i;
}

_type ComplexNumber::real() const {
    return _r;
}

_type ComplexNumber::imaginary() const {
    return _i;
}

std::ostream& operator<<(std::ostream& os, _cref cn){
    os << '(' << cn.real() << ", " << cn.imaginary() << ')';
    return os;
}

ComplexNumber randComplex() {
    static std::random_device rd;  
    static std::mt19937 gen(rd()); 
    
    static std::uniform_real_distribution<_type> dis(-100.0L, 100.0L);
    
    return ComplexNumber(dis(gen), dis(gen));
}


