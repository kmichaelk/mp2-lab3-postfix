#include <iostream>
#include <string>
#include "postfix.h"

using namespace std;

int main()
{
    setlocale(LC_NUMERIC, "en_US.UTF-8");
    ///

    string infix;
    while (true) {
        cout << ">>> ";
        cin >> infix;
        cout << TArithmeticExpression(infix).calculate() << endl;
        cout << endl;
    }

    return EXIT_SUCCESS;
}
