#include <iostream>
#include "polynom.h"

int main() {
    polinom p1("2x^2+3x+1");
    polinom p2("x^2-2x+1");
    polinom p3("x^2");
    polinom p4("-3.5x^2");
    

    p1.print();
    p2.print();
    p3.print();
    p4.print();
    

    polinom p5(p1+p2);
    polinom p6(p3 *p4);
    p5.print();
    p6.print();
    return 0;
}