#ifndef MY_CMPXN
#define MY_CMPXN
#define _type long double
#define _cref const ComplexNumber&
#include <iostream>

class ComplexNumber{
private:
    _type _r = 0;
    _type _i = 0;
public:
    ComplexNumber()= default;
    ComplexNumber(_type real, _type imaginary): _r(real), _i(imaginary){}
    
    ComplexNumber operator+(_cref other) const;
    ComplexNumber operator-(_cref other) const;
    ComplexNumber operator*(_type q) const;
    _type abs() const;
    bool operator>(_cref other) const;
    bool operator<(_cref other) const;
    _type& real();
    _type& imaginary();
    _type real() const;
    _type imaginary() const;
};

std::ostream& operator<<(std::ostream& os, _cref cn);


ComplexNumber randComplex();
#endif
