#pragma once

enum class FilterOps {
    EQ,
    LT,
    LTE,
    GT,
    GTE,
};

template<typename T>
auto Compare(const FilterOps op, const T& op1, const T& op2) {
    switch(op) {
        case FilterOps::EQ: {
            return op1 == op2;
        }
        case FilterOps::LT: {
            return op1 <  op2;
        }
        case FilterOps::LTE: {
            return op1 <= op2;
        }
        case FilterOps::GT: {
            return op1 >  op2;
        }
        case FilterOps::GTE: {
            return op1 >= op2;
        }
        default: {
            return false;
        }
    }
}
template auto Compare<int>(const FilterOps, const int& op1, const int& op2);
