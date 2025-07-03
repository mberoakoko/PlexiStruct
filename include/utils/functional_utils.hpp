//
// Created by mbero on 03/07/2025.
//

#ifndef FUNCTIONAL_UTILS_HPP
#define FUNCTIONAL_UTILS_HPP
#include <utility>

namespace PlexiStruct::functional {
    template<typename F, typename G>
    auto compose(F&& f, G&& g)  {
        return [=](auto x){return f(g(x));};
    }

    template<typename F, typename ... F_Rest>
    auto compose(F&& f, F_Rest... r) {
        return [=](auto x) { return f(compose(r...)(x)); };
    }

    class Number;
    class UnaryExpression;
    class BinaryExpression;
    class IExprVisitor;

    class IExpr {
    public:
        virtual ~IExpr() = default;
        virtual auto accept(IExprVisitor& expr_visitor)-> double = 0;
    };

    class IExprVisitor {
    public:
        virtual ~IExprVisitor() = default;
        virtual auto accept(const Number& number) -> double = 0;
        virtual auto accept(const UnaryExpression& unary_expr) -> double = 0;
        virtual auto accept(const BinaryExpression& binary_expr) -> double = 0;
    };

    using ExpPtr = std::shared_ptr<IExpr>;

    class Number final: public IExpr {
        double num_ { 0 };
    public:
        explicit Number(double num) : num_(num) {}

        static auto make(double num) ->  ExpPtr {
            return std::make_shared<Number>(num);
        }

        ~Number() override = default;

        auto get_num() const -> double { return num_; }

        auto accept(IExprVisitor &expr_visitor) -> double override {
            return expr_visitor.accept(*this);
        };
    };



    enum class Operator: std::uint8_t {PLUS, MULTIPLY, DIVIDE, SUBTRACT};
    class BinaryExpression final: public IExpr {
        std::shared_ptr<IExpr> left_;
        std::shared_ptr<IExpr> right_;
        Operator op_ { Operator::PLUS };
    public:
        explicit BinaryExpression(const std::shared_ptr<IExpr>& left,const std::shared_ptr<IExpr>& right, const Operator& op = Operator::PLUS)
        : left_(std::move(left)), right_(std::move(right)), op_(op) {}

        ~BinaryExpression() override = default;

        static auto make(const ExpPtr& left, const  ExpPtr& right, const Operator& op = Operator::PLUS) ->ExpPtr {
            return std::make_shared<BinaryExpression>(left, right, op);
        }

        auto get_op() const -> Operator { return op_; }

        auto get_right() const -> std::shared_ptr<IExpr> { return right_; }

        auto get_left() const -> std::shared_ptr<IExpr> { return left_; }

        auto accept(IExprVisitor &expr_visitor) -> double override {
            return expr_visitor.accept(*this);
        };
    };


    class UnaryExpression final: public IExpr {
        std::shared_ptr<IExpr> operand_;
        Operator op_ { Operator::PLUS };
    public:
        explicit UnaryExpression(const std::shared_ptr<IExpr>& operand_, const Operator& op = Operator::PLUS )
            : operand_(std::move(operand_)), op_(op) {}

        ~UnaryExpression() override = default;

        static auto make(const ExpPtr& operand , const Operator& op = Operator::PLUS) ->ExpPtr {
            return std::make_shared<UnaryExpression>(operand, op);
        }

        auto get_op() const -> Operator { return op_; }

        auto get_operand() const -> std::shared_ptr<IExpr> { return operand_; }

        auto accept(IExprVisitor &expr_visitor) -> double override {
            return expr_visitor.accept(*this);
        }

    };


    class TreeEvalVisitor final: public IExprVisitor {
    public:
        ~TreeEvalVisitor() override = default;

        auto accept(const Number &number) -> double override {
            return number.get_num();
        };

        auto accept(const UnaryExpression &unary_expr) -> double override {
            auto op = unary_expr.get_op();
            auto rhs_val = unary_expr.get_operand()->accept(*this);
            switch (op) {
                case Operator::PLUS: {
                    return +rhs_val;
                }
                case Operator::SUBTRACT: {
                    return -rhs_val;
                }
                default: {
                    return +rhs_val; // This is absolutely wrong
                }
            }
        };

        auto accept(const BinaryExpression &binary_expr) -> double override {
            auto op = binary_expr.get_op();
            auto rhs_val = binary_expr.get_right()->accept(*this);
            auto lhs_val = binary_expr.get_left()->accept(*this);
            switch (op) {
                case Operator::PLUS: {
                    return rhs_val + lhs_val;
                };
                case Operator::SUBTRACT: {
                    return rhs_val - lhs_val;
                }
                case Operator::MULTIPLY: {
                    return rhs_val * lhs_val;
                }
                case Operator::DIVIDE: {
                    return rhs_val / lhs_val;
                }
                default: {
                    return rhs_val;
                }
            }
        };
    };

    inline auto evaluate(std::shared_ptr<IExpr> expr) -> double {
        const auto evaluator = std::make_shared<TreeEvalVisitor>();
        return expr->accept(*evaluator);
    }

    inline auto test_evaluation() -> void  {
        ExpPtr exp = Number::make(1);
        ExpPtr exp2 = Number::make(2);
        ExpPtr exp3 = Number::make(3);
        ExpPtr binary_expr = BinaryExpression::make(exp, exp2, Operator::MULTIPLY);
        ExpPtr binary_expr2 = BinaryExpression::make(binary_expr, exp3, Operator::MULTIPLY);
        std::cout << "Evaluating binary expression "<< std::endl;
        std::cout << evaluate(binary_expr) << std::endl;
        std::cout << "Evaluating binary expression 2 "<< std::endl;
        std::cout << evaluate(binary_expr2) << std::endl;

    }


}




#endif //FUNCTIONAL_UTILS_HPP
