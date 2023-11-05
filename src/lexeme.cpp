#include "lexeme.h"
#include <stdexcept>

TLexemeValue::TLexemeValue(std::string str)
        : str(std::move(str))
{}

TLexemeValue &TLexemeValue::operator+=(char c)
{
    str += c;
    return *this;
}

std::string TLexemeValue::as_string() const
{
    //if (str.empty())
    //    throw std::runtime_error("Lexeme is interpreted as a number");

    return str;
}

char TLexemeValue::as_char() const
{
    return as_string()[0];
}

double TLexemeValue::as_number() const
{
    return numeric;
}

bool TLexemeValue::reinterpret_as_number()
{
    if (!str.empty())
    {
        try {
            numeric = std::stod(str);
        } catch (...) {
            return false;
        }

        str.resize(0);
    }

    return true;
}
