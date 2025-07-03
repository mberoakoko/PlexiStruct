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

    enum class expr_kind: std::uint8_t {
        ILLEGAL = 0, OPEARATOR, VALUE
    };

    struct expr_node_item {
        expr_kind kind { expr_kind::ILLEGAL };
        double value { 0 };
        Operator op { Operator::PLUS };

        static auto create(const double& value) -> expr_node_item {
           return {.kind = expr_kind::VALUE, .value = value};
        }

        static auto create(const Operator& op) -> expr_node_item {
            return {.kind = expr_kind::OPEARATOR, .op = op};
        }
    };

    class FlattenedExpresstionVisitor final: public IExprVisitor {
        std::list<expr_node_item> nodes_ {};

        static auto make_item(const double& number) -> expr_node_item {
            return expr_node_item::create(number);
        }

        static auto make_item(const Operator& op) -> expr_node_item {
            return  expr_node_item::create(op);
        }
    public:
        explicit FlattenedExpresstionVisitor() = default;
        ~FlattenedExpresstionVisitor() override;

        auto accept(const Number &number) -> double override {
            nodes_.push_back(make_item(number.get_num()));
            return number.get_num();
        };

        auto accept(const UnaryExpression &unary_expr) -> double override {
            unary_expr.get_operand()->accept(*this);
            nodes_.push_back(make_item(unary_expr.get_op()));
            return 43;
        };

        auto accept(const BinaryExpression &binary_expr) -> double override {
            binary_expr.get_right()->accept(*this);
            binary_expr.get_left()->accept(*this);
            nodes_.push_back(make_item(binary_expr.get_op()));
            return 42;
        };

        auto get_expression_list() -> std::list<expr_node_item> {
            return nodes_;
        }
    };

    inline auto gen_expression_list(const ExpPtr& expression) -> std::list<expr_node_item> {
        std::shared_ptr<FlattenedExpresstionVisitor> evaluator = std::make_shared<FlattenedExpresstionVisitor>();
        expression->accept(*evaluator);
        return evaluator->get_expression_list();
    }

    class stack: public std::stack<double> {
    public:
        stack() = default;
        auto push_data(const double& value) -> void {
            this->push(value);
        }

        auto pop_data() -> double {
            auto value = this->top();
            this->pop();
            return value;
        }
    };

    inline auto evaluate(const std::list<expr_node_item>& expr_list) -> double {

    }

    inline auto evaluate(std::shared_ptr<IExpr> expr) -> double {
        const auto evaluator = std::make_shared<TreeEvalVisitor>();
        return expr->accept(*evaluator);
    }

    inline auto

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
