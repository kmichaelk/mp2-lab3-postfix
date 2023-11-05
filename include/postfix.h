#ifndef __POSTFIX_H__
#define __POSTFIX_H__

#include <stdexcept>
#include <string>
#include <functional>
#include <memory>
#include <map>
#include "lexeme.h"
#include "list.h"

class parse_error : public std::runtime_error
{
public:
    explicit
    parse_error(const std::string &message)
        : runtime_error(message)
    {}
};

class TArithmeticExpressionFunction {
public:
    [[nodiscard]]
    virtual double execute(double x) = 0;
};

class TArithmeticExpression {
private:
    const std::string infix;
    TDynamicList<std::string> postfix;

    TDynamicList<TLexeme> tokens;

    TDynamicList<std::string> variables;
    TDynamicList<std::string> func_names;
public:
    explicit TArithmeticExpression(const std::string& infix);

    [[nodiscard]] std::string get_infix() const;
    [[nodiscard]] TDynamicList<std::string> get_postfix() const;

    [[nodiscard]] TDynamicList<std::string> get_variables() const;
    [[nodiscard]] TDynamicList<std::string> get_functions() const;

    [[nodiscard]]
    double calculate(
            const std::map<std::string, double>& values = {},
            const std::map<std::string, std::shared_ptr<TArithmeticExpressionFunction>>& functions = {}) const;

    static const char POSTFIX_LEXEME_SEPARATOR = ' ';
};

class TComputedArithmeticExpressionFunction : public TArithmeticExpressionFunction {
private:
    const std::function<double (double)> handler;
public:
    explicit TComputedArithmeticExpressionFunction(std::function<double (double)>  handler);

    [[nodiscard]]
    double execute(double x) override;
};

class TExplicitArithmeticExpressionFunction : public TArithmeticExpressionFunction {
private:
    const TArithmeticExpression expression;
public:
    explicit TExplicitArithmeticExpressionFunction(TArithmeticExpression expression);

    [[nodiscard]]
    double execute(double x) override;
};

#endif // __POSTFIX_H__
