#include "Expression.hpp"
#include <stdexcept>

Const::Const(Int128 val) : value(val) {}
Const::Const(int64_t val) : value(Int128(val)) {}
Int128 Const::eval(const std::unordered_map<std::string, Int128>&) const { return value; }
Expression* Const::clone() const { return new Const(*this); }
void Const::print(std::ostream& os) const { os << value; }

Variable::Variable(std::string var_name) : name(std::move(var_name)) {}
Int128 Variable::eval(const std::unordered_map<std::string, Int128>& vars) const {
    auto it = vars.find(name);
    if (it != vars.end()) {
        return it->second;
    }
    return Int128(0);
}
Expression* Variable::clone() const { return new Variable(*this); }
void Variable::print(std::ostream& os) const { os << name; }

// --- Negate ---
Negate::Negate(const Expression& op) : operand(op.clone()) {}
Negate::Negate(const Negate& other) : operand(other.operand->clone()) {}
Negate& Negate::operator=(const Negate& other) {
    if (this != &other) {
        delete operand;
        operand = other.operand->clone();
    }
    return *this;
}
Negate::~Negate() { delete operand; }
Int128 Negate::eval(const std::unordered_map<std::string, Int128>& vars) const { return -operand->eval(vars); }
Expression* Negate::clone() const { return new Negate(*this); }
void Negate::print(std::ostream& os) const { os << "(-" << *operand << ")"; }

Add::Add(const Expression& l, const Expression& r) : left(l.clone()), right(r.clone()) {}
Add::Add(const Add& other) : left(other.left->clone()), right(other.right->clone()) {}
Add& Add::operator=(const Add& other) {
    if (this != &other) {
        delete left; delete right;
        left = other.left->clone(); right = other.right->clone();
    }
    return *this;
}
Add::~Add() { delete left; delete right; }
Int128 Add::eval(const std::unordered_map<std::string, Int128>& vars) const { return left->eval(vars) + right->eval(vars); }
Expression* Add::clone() const { return new Add(*this); }
void Add::print(std::ostream& os) const { os << "(" << *left << " + " << *right << ")"; }

Subtract::Subtract(const Expression& l, const Expression& r) : left(l.clone()), right(r.clone()) {}
Subtract::Subtract(const Subtract& other) : left(other.left->clone()), right(other.right->clone()) {}
Subtract& Subtract::operator=(const Subtract& other) {
    if (this != &other) {
        delete left; delete right;
        left = other.left->clone(); right = other.right->clone();
    }
    return *this;
}
Subtract::~Subtract() { delete left; delete right; }
Int128 Subtract::eval(const std::unordered_map<std::string, Int128>& vars) const { return left->eval(vars) - right->eval(vars); }
Expression* Subtract::clone() const { return new Subtract(*this); }
void Subtract::print(std::ostream& os) const { os << "(" << *left << " - " << *right << ")"; }

Multiply::Multiply(const Expression& l, const Expression& r) : left(l.clone()), right(r.clone()) {}
Multiply::Multiply(const Multiply& other) : left(other.left->clone()), right(other.right->clone()) {}
Multiply& Multiply::operator=(const Multiply& other) {
    if (this != &other) {
        delete left; delete right;
        left = other.left->clone(); right = other.right->clone();
    }
    return *this;
}
Multiply::~Multiply() { delete left; delete right; }
Int128 Multiply::eval(const std::unordered_map<std::string, Int128>& vars) const { return left->eval(vars) * right->eval(vars); }
Expression* Multiply::clone() const { return new Multiply(*this); }
void Multiply::print(std::ostream& os) const { os << "(" << *left << " * " << *right << ")"; }

Divide::Divide(const Expression& l, const Expression& r) : left(l.clone()), right(r.clone()) {}
Divide::Divide(const Divide& other) : left(other.left->clone()), right(other.right->clone()) {}
Divide& Divide::operator=(const Divide& other) {
    if (this != &other) {
        delete left; delete right;
        left = other.left->clone(); right = other.right->clone();
    }
    return *this;
}
Divide::~Divide() { delete left; delete right; }
Int128 Divide::eval(const std::unordered_map<std::string, Int128>& vars) const { return left->eval(vars) / right->eval(vars); }
Expression* Divide::clone() const { return new Divide(*this); }
void Divide::print(std::ostream& os) const { os << "(" << *left << " / " << *right << ")"; }