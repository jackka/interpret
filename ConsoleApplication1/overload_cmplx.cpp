#include <iostream>
#include <cstdio>


using namespace std;

class complex
{
    double re, im;
public:
    complex(double r, double i) : re(r), im(i) {}

    const complex  operator+(const complex& ri) const {
        cout << re << endl;
        return complex(re + ri.re, im + ri.im);
    }
};

int main() {

    complex a = complex(1, 2.5);
    complex b = complex(4.1, 9);
    complex c = b;
    
    c = a + b;

}