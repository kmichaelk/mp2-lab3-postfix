#include "operators.h"
#include <cmath>

const std::map<char, TArithmeticOperator> Operators::LIST = {
        { '+', { 2, std::plus<>{} } },
        { '-', { 2, std::minus<>{} } },
        { '*', { 3, std::multiplies<>{} } },
        { '/', { 3, std::divides<>{} } },
        { '%', { 3, [](double a, double b) { return (long)a % (long)b; } } },
        { '~', { 4, [](double _, double x) { return -x; }, TArithmeticOperator::Type::UnaryPrefix } },
        { '!', { 4, [](double x, double _) {
            const long lim = (long)x;
            double res = 1;
            for (long i = 1; i <= lim; ++i)
            {
                res *= i;
            }
            return res;
        }, TArithmeticOperator::Type::UnaryPostfix } },
};
const std::map<std::string, double> Operators::CONSTANTS = {
        { "pi", 3.14159 }
};
const std::map<std::string, std::shared_ptr<TArithmeticExpressionFunction>> Operators::STD_FUNCTIONS = {
        { "sin", std::make_shared<TComputedArithmeticExpressionFunction>([](double x) { return sin(x); })},
        { "cos", std::make_shared<TComputedArithmeticExpressionFunction>([](double x) { return cos(x); }) },
        { "tan", std::make_shared<TComputedArithmeticExpressionFunction>([](double x) { return tan(x); }) },
        { "log", std::make_shared<TComputedArithmeticExpressionFunction>([](double x) { return log(x); }) },
        { "sqrt", std::make_shared<TComputedArithmeticExpressionFunction>([](double x) { return sqrt(x); }) }
};
