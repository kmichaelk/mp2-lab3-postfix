#include <gtest.h>
#include "postfix.h"

TEST(TArithmeticExpression, can_parse_complex_expressions)
{
    ASSERT_NO_THROW(TArithmeticExpression expr("((a+(b*c)+((4*d)+7)/sin(8*e))+func(2*a))*2"));
}

TEST(TArithmeticExpression, can_calculate_expressions_with_numbers)
{
    TArithmeticExpression expr("1+2");
    EXPECT_EQ(1+2, expr.calculate());
}

TEST(TArithmeticExpression, can_calculate_expressions_with_spaces)
{
    TArithmeticExpression expr("1 + 2");
    EXPECT_EQ(1+2, expr.calculate());
}

TEST(TArithmeticExpression, can_calculate_expressions_with_variables_and_constants)
{
    double a = 1;
    TArithmeticExpression expr("a+2");
    EXPECT_EQ(a+2, expr.calculate({ { "a", a }}));
}

TEST(TArithmeticExpression, can_calculate_complex_expressions)
{
    double a = 5, b = 7, c = 9, d = 13;

    TArithmeticExpression expr("a+(b*c)+((4*d)+7)");
    double result = expr.calculate({
        { "a", a },
        { "b", b },
        { "c", c },
        { "d", d }
    }, {});

    EXPECT_EQ(a+(b*c)+((4*d)+7), result);
}

TEST(TArithmeticExpression, can_parse_and_calculate_deeply_nested_expressions)
{
    TArithmeticExpression expr("((((((1+2)+3)+4)+5)+6)+7)+8");
    double result = expr.calculate();

    EXPECT_EQ(((((((1+2)+3)+4)+5)+6)+7)+8, result);
}

TEST(TArithmeticExpression, can_parse_unary_prefix_operators)
{
    TArithmeticExpression expr("-5 + ((-100)*((-3)-(-3)))");
    double result = expr.calculate();

    EXPECT_EQ(-5 + ((-100)*((-3)-(-3))), result);
}

TEST(TArithmeticExpression, can_parse_unary_postfix_operators)
{
    TArithmeticExpression expr("5!");
    double result = expr.calculate();

    EXPECT_EQ(120, result);
}

TEST(TArithmeticExpression, can_calculate_complex_expressions_with_unary_operators)
{
    TArithmeticExpression expr("-5! + 10 - (-3)");
    double result = expr.calculate();

    EXPECT_EQ(-120 + 10 - (-3), result);
}

TEST(TArithmeticExpression, can_invoke_computed_function_call)
{
    std::map<std::string, std::shared_ptr<TArithmeticExpressionFunction>> funcs = {
        { "func", std::make_shared<TComputedArithmeticExpressionFunction>([](double x) { return sin(x); })},
    };

    TArithmeticExpression expr("func(123)");
    double result = expr.calculate({}, funcs);

    EXPECT_EQ(funcs["func"]->execute(123), result);
}

TEST(TArithmeticExpression, can_invoke_explicit_function_call)
{
    std::map<std::string, std::shared_ptr<TArithmeticExpressionFunction>> funcs = {
        { "func", std::make_shared<TExplicitArithmeticExpressionFunction>(TArithmeticExpression("42+x"))},
    };

    TArithmeticExpression expr("func(123)");
    double result = expr.calculate({}, funcs);

    EXPECT_EQ(funcs["func"]->execute(123), result);
}

TEST(TArithmeticExpression, can_invoke_polymorphic_functions)
{
    std::map<std::string, std::shared_ptr<TArithmeticExpressionFunction>> funcs = {
        { "computed", std::make_shared<TComputedArithmeticExpressionFunction>([](double x) { return 42 * x; })},
        { "explicit", std::make_shared<TExplicitArithmeticExpressionFunction>(TArithmeticExpression("42+x"))},
    };

    TArithmeticExpression expr("computed(123) + explicit(321)");
    double result = expr.calculate({}, funcs);

    EXPECT_EQ(funcs["computed"]->execute(123) + funcs["explicit"]->execute(321), result);
}

TEST(TArithmeticExpression, can_pass_variable_to_function)
{
    int a = 24;
    std::map<std::string, std::shared_ptr<TArithmeticExpressionFunction>> funcs = {
        { "func", std::make_shared<TExplicitArithmeticExpressionFunction>(TArithmeticExpression("42+x"))},
    };

    TArithmeticExpression expr("func(a)");
    double result = expr.calculate({ { "a", a }}, funcs);

    EXPECT_EQ(funcs["func"]->execute(a), result);
}

TEST(TArithmeticExpression, unary_minus_works_with_variables_and_functions)
{
    ASSERT_NO_THROW(TArithmeticExpression expr("-a+b*(-func(a))"));
}

TEST(TArithmeticExpression, can_calculate_expressions_with_constants)
{
    TArithmeticExpression expr("cos(pi)");
    double result = expr.calculate();

    EXPECT_EQ(true, (result - (-1)) <= 0.01);
}
