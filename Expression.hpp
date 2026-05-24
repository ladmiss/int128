#pragma once

#include "int128.hpp" 
#include <string>
#include <unordered_map>
#include <iostream>
#include <utility>

class Expression {
public:
    virtual ~Expression() = default;
    virtual Int128 eval(const std::unordered_map<std::string, Int128>& vars) const = 0;
    virtual Expression* clone() const = 0;
    virtual void print(std::ostream& os) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Expression& expr) {
        expr.print(os);
        return os;
    }
};

class Const : public Expression {
    Int128 value;
public:
    Const(Int128 val);
    Const(int64_t val);
    Int128 eval(const std::unordered_map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Variable : public Expression {
    std::string name;
public:
    Variable(std::string var_name);
    Int128 eval(const std::unordered_map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Negate : public Expression {
    Expression* operand;
public:
    Negate(const Expression& op);
    Negate(const Negate& other);
    Negate& operator=(const Negate& other);
    ~Negate() override;
    
    Int128 eval(const std::unordered_map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Add : public Expression {
    Expression* left;
    Expression* right;
public:
    Add(const Expression& l, const Expression& r);
    Add(const Add& other);
    Add& operator=(const Add& other);
    ~Add() override;

    Int128 eval(const std::unordered_map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Subtract : public Expression {
    Expression* left;
    Expression* right;
public:
    Subtract(const Expression& l, const Expression& r);
    Subtract(const Subtract& other);
    Subtract& operator=(const Subtract& other);
    ~Subtract() override;

    Int128 eval(const std::unordered_map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Multiply : public Expression {
    Expression* left;
    Expression* right;
public:
    Multiply(const Expression& l, const Expression& r);
    Multiply(const Multiply& other);
    Multiply& operator=(const Multiply& other);
    ~Multiply() override;

    Int128 eval(const std::unordered_map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Divide : public Expression {
    Expression* left;
    Expression* right;
public:
    Divide(const Expression& l, const Expression& r);
    Divide(const Divide& other);
    Divide& operator=(const Divide& other);
    ~Divide() override;

    Int128 eval(const std::unordered_map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

inline Add operator+(const Expression& lhs, const Expression& rhs) { return Add(lhs, rhs); }
inline Subtract operator-(const Expression& lhs, const Expression& rhs) { return Subtract(lhs, rhs); }
inline Multiply operator*(const Expression& lhs, const Expression& rhs) { return Multiply(lhs, rhs); }
inline Divide operator/(const Expression& lhs, const Expression& rhs) { return Divide(lhs, rhs); }
inline Negate operator-(const Expression& expr) { return Negate(expr); }