#include <iostream>
#include <string>
#include <sstream>
#include "postfix.h"

using namespace std;

string get_postfix_as_string(const TArithmeticExpression& expr)
{
    const auto& postfix = expr.get_postfix();
    const size_t size = postfix.size();

    stringstream ss;
    for (size_t i = 0; i < size - 1; i++)
    {
        ss << postfix[i] << ' ';
    }
    ss << postfix[size - 1];

    return ss.str();
}

int main()
{
    string raw = "(a+(b*c)+((4*j)+7)/sin(8*x))+func(2*a)";
    TArithmeticExpression expr(raw);

    setlocale(LC_ALL, "Russian");
    //cout << "Введите арифметическое выражение: ";
    //cin >> expression;
    //cout << expression << endl;
    cout << "Арифметическое выражение: " << expr.get_infix() << endl;
    cout << "Постфиксная форма: '" << get_postfix_as_string(expr) << "'" << endl;

    double res = expr.calculate({
                                        { "a", 5 },
                                        { "b", 7 },
                                        { "c", 9 },
                                        { "j", 13 },
                                        { "x", 54 },
        },  {
            { "sin", std::make_shared<TComputedArithmeticExpressionFunction>([](double x) { return sin(x); })},
            { "func", std::make_shared<TExplicitArithmeticExpressionFunction>(TArithmeticExpression("5*x"))},
    });
    cout << res << endl;

    return 0;
}
