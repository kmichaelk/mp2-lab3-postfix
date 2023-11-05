#include "lexeme.h"
#include <stdexcept>
#include <charconv>

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
        auto [p, ec] = std::from_chars(str.data(), str.data() + str.size(), numeric);
        if (ec != std::errc())
            return false;

        str.resize(0);
    }

    return true;
}
