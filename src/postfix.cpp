﻿#include "postfix.h"
#include "stack.h"
#include "lexeme.h"
#include "operators.h"
#include "validator.h"
#include <algorithm>

TDynamicList<TLexeme> tokenize(const std::string& infix)
{
    TDynamicList<TLexeme> result;

    TLexeme lex = { TLexeme::Type::Number, TLexemeValue() };
    std::string buf;

    for (const char& c : infix)
    {
        if (isspace(c))
            continue;

        if (Operators::is_service_symbol(c))
        {
            if (!buf.empty())
            {
                lex.value = TLexemeValue(buf);
                result.push_back(lex);
            }

            lex.type = Operators::is_bracket(c) ? TLexeme::Type::Bracket : TLexeme::Type::Operator;
            lex.value = TLexemeValue(std::string{c});
            if (lex.type == TLexeme::Type::Bracket && lex.value.as_char() == '('
                && result.tail().type == TLexeme::Type::Variable)
            {
                result.tail().type = TLexeme::Type::Function;
            }
            result.push_back(lex);

            lex.type = TLexeme::Type::Number;

            buf = "";
            continue;
        }

        buf += c;
        if (lex.type == TLexeme::Type::Number && (!isdigit(c) && c != '.'))
            lex.type = TLexeme::Type::Variable;
    }
    if (!buf.empty())
    {
        lex.value = TLexemeValue(buf);
        result.push_back(lex);
    }

    return result;
}

TDynamicList<TLexeme> to_postfix(const TDynamicList<TLexeme>& lexemes)
{
    TDynamicList<TLexeme> postfix(lexemes.size());
    TStack<TLexeme> stack((lexemes.size() / 2) + 1);

    size_t i = 0;
    for (const auto& lexeme : lexemes)
    {
        switch (lexeme.type)
        {
            case TLexeme::Type::Bracket: {
                if (lexeme.value.as_char() == '(')
                {
                    stack.push(lexeme);
                }
                else // lexeme.value.as_char() == ')'
                {
                    while (stack.top().value.as_char() != '(') {
                        postfix.push_back(stack.top());
                        stack.pop();
                    }
                    stack.pop();

                    if (!stack.empty() && stack.top().type == TLexeme::Type::Function)
                    {
                        postfix.push_back(stack.pop_element());
                    }
                }
                break;
            }
            case TLexeme::Type::Operator: {
                char current = lexeme.value.as_char();
                if (current == '-' && (i == 0 || lexemes[i - 1].value.as_char() == '(')) {
                    current = '~';
                }

                const auto& op_type = Operators::LIST.at(current).type;
                if (op_type == TArithmeticOperator::Type::UnaryPrefix)
                {
                    postfix.push_back(TLexeme { TLexeme::Type::Number });
                }

                while (!stack.empty())
                {
                    const TLexeme& stored = stack.top();
                    if (stored.type != TLexeme::Type::Operator || Operators::priority(current) > Operators::priority(stored.value.as_char()))
                    {
                        break;
                    }
                    postfix.push_back(stored);
                    stack.pop();
                }
                stack.push(lexeme);

                if (op_type == TArithmeticOperator::Type::UnaryPostfix)
                {
                    postfix.push_back(TLexeme { TLexeme::Type::Number });
                }
                break;
            }
            case TLexeme::Type::Function: {
                stack.push(lexeme);
                break;
            }
            case TLexeme::Type::Variable:
            case TLexeme::Type::Number: {
                postfix.push_back(lexeme);
                break;
            }
            default: {
                throw expression_parse_error("Unimplemented");
            }
        }

        i++;
    }

    while (!stack.empty())
    {
        postfix.push_back(stack.pop_element());
    }

    return postfix;
}

TArithmeticExpression::TArithmeticExpression(const std::string& infix)
    : infix(validate_infix(infix))
    , tokens(to_postfix(tokenize(infix)))
{
    //
    for (auto& token : tokens)
    {
        postfix.push_back(token.value.as_string());

        switch (token.type) {
            case TLexeme::Type::Function: {
                const std::string& name = token.value.as_string();
                if (!Operators::supports_function(name)) {
                    func_names.insert(name);
                }
                break;
            }
            case TLexeme::Type::Variable: {
                const std::string& name = token.value.as_string();
                if (!Operators::has_constant(name)) {
                    variables.insert(name);
                }
                break;
            }
            case TLexeme::Type::Number: {
                if (!token.value.reinterpret_as_number())
                {
                    throw expression_parse_error("Failed to parse numeric token: " + token.value.as_string());
                }
                break;
            }
        }
    }
    //
}

std::string TArithmeticExpression::get_infix() const
{
    return infix;
}
TDynamicList<std::string> TArithmeticExpression::get_postfix() const
{
    return postfix;
}

std::set<std::string> TArithmeticExpression::get_variables() const
{
    return variables;
}
std::set<std::string> TArithmeticExpression::get_functions() const
{
    return func_names;
}


template<typename K, typename V>
void left_join(const std::map<K, V> left, const std::map<K, V> right)
{
    for (const auto& r_pair : right)
    {
        const auto& l_pair = left.find(r_pair.first);
        if (l_pair == left.end()) continue;
        (*l_pair).second = r_pair.second;
    }
}

template<typename K, typename V>
bool require_keys(const std::map<K, V>& map, const std::set<K>& keys)
{
    return std::all_of(keys.begin(), keys.end(), [&map](K key) { return map.find(key) != map.end(); });
}

double TArithmeticExpression::calculate(const std::map<std::string, double>& _values,
                                        const std::map<std::string, std::shared_ptr<TArithmeticExpressionFunction>>& _functions
                                        ) const {

    auto values = Operators::CONSTANTS;
    for (const auto &item : _values) {
        values[item.first] = item.second;
    }

    if (!require_keys(values, variables))
        throw std::invalid_argument("Not all variables values are present");

    auto funcs = Operators::STD_FUNCTIONS;
    for (const auto &item : _functions) {
        funcs[item.first] = item.second;
    }

    if (!require_keys(funcs, func_names))
        throw std::invalid_argument("Not all function implementations are present");

    TStack<double> stack((tokens.size() / 2) + 1);
    for (const auto& lexeme : tokens)
    {
        switch (lexeme.type)
        {
            case TLexeme::Type::Operator: {
                stack.push(Operators::LIST.at(lexeme.value.as_char()).handler(stack.pop_element(), stack.pop_element()));
                break;
            }
            case TLexeme::Type::Function: {
                stack.push(funcs.at(lexeme.value.as_string())->execute(stack.pop_element()));
                break;
            }
            case TLexeme::Type::Variable:
            case TLexeme::Type::Number: {
                stack.push(lexeme.type == TLexeme::Type::Number
                        ? lexeme.value.as_number()
                        : values.at(lexeme.value.as_string()));
                break;
            }
            default: {
                throw std::runtime_error("Unimplemented");
            }
        }
    }

    return stack.top();
}