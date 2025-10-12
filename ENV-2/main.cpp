#include <iostream>
#include "cn.h"
#include "sorts.h"
#include <array>
#include <vector>

using namespace std;

#define repeat(n) for(size_t i=0; i<n; ++i)

int main(){
    ComplexNumber cn(256.23, -2394.5);
    ComplexNumber cn2(-256.23, 2394.5);

    cout << cn << endl;
    cout << cn2 << endl;
    cout << cn + cn2 << endl;
    cout << cn - cn2 << endl;
    cout << cn*5.5 << endl;
    cout << cn.abs() << endl;
    cout << (cn > cn2) << endl;
    
    array<ComplexNumber, 5> arr;
    repeat(5){
        arr[i] = randComplex();
    }
    
    vector<ComplexNumber> vec(5);
    repeat(5){
        vec[i] = randComplex();
    }
    cout << "{ ";
    repeat(5){
        cout << arr[i] << ' ';
    }
    cout << '}' << endl;
    
    cout << "{ ";
    repeat(5){
        cout << vec[i] << ' ';
    }
    cout << '}' << endl;
    
    sortComplex(arr); // works with anything that has .begin() and .end()
    sortComplex(vec);
    
    cout << "{ ";
    repeat(5){
        cout << arr[i] << ' ';
    }
    cout << '}' << endl;
    
    cout << "{ ";
    repeat(5){
        cout << vec[i] << ' ';
    }
    cout << '}' << endl;
    
}
