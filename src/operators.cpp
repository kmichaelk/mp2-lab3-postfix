#include "operators.h"

const std::map<char, TArithmeticOperator> Operators::LIST = {
        { '+', { 2, std::plus<>{} } },
        { '-', { 2, std::minus<>{} } },
        { '*', { 3, std::multiplies<>{} } },
        { '/', { 3, std::divides<>{} } },
};