#ifndef __OPERATORS_H__
#define __OPERATORS_H__

#include <map>
#include <functional>

struct TArithmeticOperator
{
    int priority;
    std::function<double (double, double)> handler;
};

class Operators
{
public:
    Operators() = delete;

    static const std::map<char, TArithmeticOperator> LIST;

    static int priority(const char c)
    {
        if (is_bracket(c)) return 0;
        return LIST.at(c).priority;
    }

    static bool is_operator(char c)
    {
        return LIST.find(c) != LIST.end();
    }

    static bool is_bracket(char c)
    {
        return c == '(' || c == ')';
    }

    static bool is_service_symbol(char c)
    {
        return is_bracket(c) || is_operator(c);
    }
};

#endif // __OPERATORS_H__