#include "postfix.h"

TComputedArithmeticExpressionFunction::TComputedArithmeticExpressionFunction(std::function<double(double)> handler)
    : handler(std::move(handler))
{}

double TComputedArithmeticExpressionFunction::execute(double x)
{
    return handler(x);
}

TExplicitArithmeticExpressionFunction::TExplicitArithmeticExpressionFunction(TArithmeticExpression expression)
    : expression(std::move(expression))
{}

double TExplicitArithmeticExpressionFunction::execute(double x)
{
    return expression.calculate({ { "x", x } }, {});
}

