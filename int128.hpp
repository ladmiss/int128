#ifndef EXPRESSION_INT128_HPP
#define EXPRESSION_INT128_HPP

#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>

class Int128 {
    // старшая часть числа
    int64_t hi_;
    // младшая часть числа
    uint64_t lo_;

public:
    Int128();

    explicit Int128(int64_t num);

    explicit Int128(std::string_view str);

    explicit operator int64_t() const;

    explicit operator double() const;

    std::string str() const;

    // Сложение
    Int128 operator+(const Int128& rhs) const;
    Int128& operator+=(const Int128& rhs);

    // Вычитание
    Int128 operator-(const Int128& rhs) const;
    Int128& operator-=(const Int128& rhs);

    // Умножение
    Int128 operator*(const Int128& rhs) const;
    Int128& operator*=(const Int128& rhs);

    // Деление
    Int128 operator/(const Int128& rhs) const;
    Int128& operator/=(const Int128& rhs);

    // Унарный минус
    Int128 operator-() const;

    // равенство
    bool operator==(const Int128& rhs) const;
    bool operator!=(const Int128& rhs) const;

    // Вывод
    friend std::ostream& operator<<(std::ostream& out, const Int128& board);


private:
    // Конструирование от двух 64‑битных частей
    explicit Int128(int64_t hi, uint64_t lo);

    // unsigned
    bool unsignedLess(const Int128& rhs) const;
    bool unsignedLessOrEqual(const Int128& rhs) const;

public:
    // Строка из бит
    std::string bit_string(bool separator = false) const;

    // Побитовое NOT
    Int128 operator~() const;

    static const Int128 ZERO;
    static const Int128 ONE;

    Int128 operator<<(int shift) const;
    Int128& operator<<=(int shift);

    Int128 operator>>(int shift) const;
    Int128& operator>>=(int shift);

    // Остаток от деления
    Int128 operator%(const Int128& rhs) const;
    Int128& operator%=(const Int128& rhs);

    // Сравнение
    bool operator<(const Int128& rhs) const;
    bool operator<=(const Int128& rhs) const;

    bool operator>(const Int128& rhs) const;
    bool operator>=(const Int128& rhs) const;

    // Побитовое и
    Int128 operator&(const Int128& rhs) const;
    Int128& operator&=(const Int128& rhs);

    // Побитовое или
    Int128 operator|(const Int128& rhs) const;
    Int128& operator|=(const Int128& rhs);

    // abs()
    Int128 abs() const;
};

#endif