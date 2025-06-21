//
// Created by mbero on 21/06/2025.
//

#ifndef VALUE_HPP
#define VALUE_HPP
#include <ostream>
#include <bits/stdc++.h>
namespace PlexiStruct::Engine {

    enum class Operations: std::uint8_t {
        ADD, SUBTRACT, MULTIPLY, DIVIDE, NO_OPERATION
    };

    inline std::ostream& operator<<(std::ostream& os, const Operations op) {
        switch (op) {
            case Operations::ADD: return os << "+";
            case Operations::SUBTRACT: return os << "-";
            case Operations::MULTIPLY: return os <<"*";
            case Operations::DIVIDE: return  os << "/";
            case Operations::NO_OPERATION: return os << "";

        }
        return os << "";
    };

    template<typename T = float>
    class ScalarValue {
    public:
        explicit ScalarValue(const T& value, const std::set<ScalarValue>& children = {}, const Operations op = Operations::NO_OPERATION)
        : value_(value), previous_(children), op_(op) {}

        friend std::ostream & operator<<(std::ostream &os, const ScalarValue &obj) {
             os     << "ScalerValue( Value : " << obj.value_
                    << ", Operation : " << obj.op_
                    << " | Children [ " ;
            // std::ranges::copy(obj.previous_, std::ostream_iterator<ScalarValue>(os, ", "));
            os <<" ] )" <<std::endl;
            return os;
        }

        auto get_children() const -> std::set<ScalarValue> {
            return previous_;
        }

        auto get_operations() const -> Operations {
            return op_;
        }

        auto operator <(const ScalarValue& other) const -> bool {
            return value_ < other.value_;
        }

        auto operator+(const ScalarValue<T>& other) -> ScalarValue<T> {
            std::set<ScalarValue> children = {*this, other};
            return ScalarValue(value_ + other.value_, children, Operations::ADD);
        }

        auto operator-(const ScalarValue<T>& other) -> ScalarValue<T> {
            std::set<ScalarValue> children = {*this, other};
            return ScalarValue(value_ - other.value_, children, Operations::SUBTRACT);
        }

        struct KeyHasher {
            auto operator()(const ScalarValue& key) const -> std::size_t {
                const std::size_t h1 = std::hash<T>()(key.value_);
                std::size_t h2 { 0 };
                if (key.get_children().size() > 0) {
                    const auto& children = key.get_children();
                    h2 = std::accumulate(std::begin(children), std::end(children), std::size_t{0},
                        [](const std::size_t& lhs, const ScalarValue& rhs) {
                            auto h_inner = std::hash<T>()(rhs.value_);
                            return lhs + h_inner;
                        });
                }

                return h1 ^ (h2 << 1);
            }
        };

        struct KeyEqual {
            auto operator()(const ScalarValue& lhs, const ScalarValue& rhs) const -> bool {
                return lhs.value_ == rhs.value_;
            }
        };


    protected:
        T value_;
        T grad_ { 0 };
        std::set<ScalarValue> previous_;
        Operations op_;
    };
}
#endif //VALUE_HPP
