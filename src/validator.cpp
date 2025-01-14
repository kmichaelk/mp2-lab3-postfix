#include "validator.h"
#include "operators.h"
#include "stack.h"
#include <stdexcept>
#include <cassert>

enum class ExpressionSymbol {
    Begin,

    Space,

    Digit,
    Dot,
    Letter,

    OpeningBracket,
    ClosingBracket,

    Operator,

    Unknown
};

ExpressionSymbol get_type(const char c)
{
    if (isspace(c))
    {
        return ExpressionSymbol::Space;
    }
    else if (c == '(')
    {
        return ExpressionSymbol::OpeningBracket;
    }
    else if (c == ')')
    {
        return ExpressionSymbol::ClosingBracket;
    }
    else if (Operators::is_operator(c))
    {
        return ExpressionSymbol::Operator;
    }
    else if (c == '.')
    {
        return ExpressionSymbol::Dot;
    }
    else if (isdigit(c))
    {
        return ExpressionSymbol::Digit;
    }
    else if (isalpha(c))
    {
        return ExpressionSymbol::Letter;
    }
    return ExpressionSymbol::Unknown;
}

std::string& validate_infix(const std::string& infix)
{
    ExpressionSymbol previous = ExpressionSymbol::Begin;

    TStack<size_t> brackets;

    size_t i = 0;
    for (const char c : infix)
    {
        ++i;

        const ExpressionSymbol current = get_type(c);
        assert(current != ExpressionSymbol::Unknown);

        if (current == ExpressionSymbol::OpeningBracket)
        {
            brackets.push(i);
        }
        else if (current == ExpressionSymbol::ClosingBracket)
        {
            if (brackets.empty())
            {
                throw expression_validation_error("Missing opening bracket", i,
                                                  expression_validation_error::cause::MissingBracket);
            }
            brackets.pop();
        }

        switch (previous) {
            case ExpressionSymbol::Begin: {
                if (current == ExpressionSymbol::Operator && c != '-')
                {
                    throw expression_validation_error("Unexpected operator", i,
                                                      expression_validation_error::cause::BadOperator);
                }
                break;
            }
            case ExpressionSymbol::Digit: {
                if (current != ExpressionSymbol::Digit && current != ExpressionSymbol::Dot
                    && current != ExpressionSymbol::Operator && current != ExpressionSymbol::Space && c != ')')
                {
                    throw expression_validation_error("Malformed number", i,
                                                      expression_validation_error::cause::BadNumber);
                }
                break;
            }
            case ExpressionSymbol::Dot: {
                if (current != ExpressionSymbol::Digit)
                {
                    throw expression_validation_error("Malformed number, fractional part expected", i,
                                                      expression_validation_error::cause::BadNumber);
                }
                break;
            }
            case ExpressionSymbol::Operator: {
                if ((i > 1 && infix[i - 2] != '!') && (current == ExpressionSymbol::ClosingBracket || current == ExpressionSymbol::Operator))
                {
                    throw expression_validation_error("Malformed operator", i,
                                                      expression_validation_error::cause::BadOperator);
                }
                break;
            }
        }

        previous = current;
    }

    if (previous == ExpressionSymbol::Operator && infix[i - 1] != '!')
    {
        throw expression_validation_error("Useless operator at the end of the expression", i,
                                          expression_validation_error::cause::BadOperator);
    }

    if (!brackets.empty())
    {
        throw expression_validation_error("Missing closing bracket", brackets.top(),
                                          expression_validation_error::cause::MissingBracket);
    }

    return const_cast<std::string &>(infix);
}