#ifndef __LEXEME_H__
#define __LEXEME_H__

#include <string>

class TLexemeValue {
private:
    std::string str;
    double numeric = 0;
public:
    explicit TLexemeValue(std::string str = "");

    TLexemeValue& operator+=(char c);

    [[nodiscard]] std::string as_string() const;
    [[nodiscard]] char as_char() const;
    [[nodiscard]] double as_number() const;

    [[nodiscard]] bool reinterpret_as_number();
};

struct TLexeme {
    enum class Type {
        Variable,
        Number,

        Function,
        Operator,

        Bracket
    } type;
    TLexemeValue value = TLexemeValue("0");
};

#endif // __LEXEME_H__
