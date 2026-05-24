// используем локальный заголовок для определения класса
#include "int128.hpp"

#include <algorithm>
#include <bitset>
#include <cmath>
#include <iostream>

// Конструирование без аргументов
Int128::Int128() : hi_(0), lo_(0) {}

// Конструирование от int64_t
Int128::Int128(int64_t num) : hi_(0) {
    lo_  = static_cast<uint64_t>(num);
    hi_ = static_cast<int64_t>(num < 0 ? ~0 : 0);
}

// старшая и младшая
Int128::Int128(int64_t hi, uint64_t lo) : hi_(hi), lo_(lo) {}

Int128::Int128(std::string_view str) : hi_(0), lo_(0) {
    bool is_negative = false;
    size_t start     = 0;
    if (!str.empty() && str[0] == '-') {
        is_negative = true;
        start       = 1;
    }
    for (size_t i = start; i < str.size(); ++i) {
        if (str[i] < '0' || str[i] > '9') {
            continue;
        }
        *this *= Int128(10);
        *this += Int128(str[i] - '0');
    }
    if (is_negative) {
        *this = -*this;
    }
}

// Явное приведение к int64_t
Int128::operator int64_t() const {
    return static_cast<int64_t>(lo_);
}

// Явное приведение к double
Int128::operator double() const {
    return std::ldexp(double(hi_), 64) + double(lo_);
}

std::string Int128::str() const {
    if (*this == ZERO) {
        return "0";
    }

    bool is_negative = *this < ZERO;
    Int128 tmp = is_negative ? -*this : *this;
    Int128 ten(10);
    std::string result;

    while (tmp != ZERO) {
        Int128 quotient;
        Int128 remainder;

        for (int i = 127; i >= 0; --i) {
            remainder <<= 1;
            bool bit = false;
            if (i >= 64) {
                bit = ((static_cast<uint64_t>(tmp.hi_) >> (i - 64)) & 1) != 0;
            } else {
                bit = ((tmp.lo_ >> i) & 1) != 0;
            }
            if (bit) {
                remainder.lo_ |= 1;
            }
            if (ten.unsignedLessOrEqual(remainder)) {
                remainder -= ten;
                quotient |= ONE << i;
            }
        }

        result += static_cast<char>('0' + remainder.lo_);
        tmp = quotient;
    }

    if (is_negative) {
        result += '-';
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// Сложение
Int128 Int128::operator+(const Int128 &_rhs) const {
    uint64_t result_low = lo_ + _rhs.lo_;
    uint64_t carry = result_low < lo_ ? 1 : 0;
    uint64_t result_high = static_cast<uint64_t>(hi_) + static_cast<uint64_t>(_rhs.hi_) + carry;
    return Int128(static_cast<int64_t>(result_high), result_low);
}
Int128 &Int128::operator+=(const Int128 &rhs) {
    *this = *this + rhs;
    return *this;
}

// Вычитание
Int128 Int128::operator-(const Int128 &rhs) const {
    return *this + -rhs;
}
Int128 &Int128::operator-=(const Int128 &rhs) {
    *this = *this - rhs;
    return *this;
}

// Умножение
Int128 Int128::operator*(const Int128 &_rhs) const {
    bool is_negative = (*this < ZERO) != (_rhs < ZERO);
    Int128 lhs = *this < ZERO ? -*this : *this;
    Int128 rhs = _rhs < ZERO ? -_rhs : _rhs;
    Int128 result;

    for (int i = 0; i < 128 && rhs != ZERO; ++i) {
        if ((rhs.lo_ & 1) != 0) {
            result += lhs;
        }
        lhs <<= 1;

        rhs.lo_ = (rhs.lo_ >> 1) | (static_cast<uint64_t>(rhs.hi_) << 63);
        rhs.hi_ = static_cast<int64_t>(static_cast<uint64_t>(rhs.hi_) >> 1);
    }

    if (is_negative) {
        result = -result;
    }
    return result;
}
Int128 &Int128::operator*=(const Int128 &rhs) {
    *this = *this * rhs;
    return *this;
}

// Деление
Int128 Int128::operator/(const Int128 &_rhs) const {
    if (_rhs == ZERO) {
        return ZERO;
    }

    bool is_negative = (*this < ZERO) != (_rhs < ZERO);
    Int128 lhs = *this < ZERO ? -*this : *this;
    Int128 rhs = _rhs < ZERO ? -_rhs : _rhs;
    Int128 result;
    Int128 current;

    for (int i = 127; i >= 0; --i) {
        current <<= 1;
        bool bit = false;
        if (i >= 64) {
            bit = ((static_cast<uint64_t>(lhs.hi_) >> (i - 64)) & 1) != 0;
        } else {
            bit = ((lhs.lo_ >> i) & 1) != 0;
        }
        if (bit) {
            current.lo_ |= 1;
        }
        if (rhs.unsignedLessOrEqual(current)) {
            current -= rhs;
            result |= ONE << i;
        }
    }

    if (is_negative) {
        result = -result;
    }
    return result;
}
Int128 &Int128::operator/=(const Int128 &rhs) {
    *this = *this / rhs;
    return *this;
}

// Унарный минус
Int128 Int128::operator-() const {
    uint64_t result_low = ~lo_ + 1;
    uint64_t carry = result_low == 0 ? 1 : 0;
    uint64_t result_high = ~static_cast<uint64_t>(hi_) + carry;
    return Int128(static_cast<int64_t>(result_high), result_low);
}

// Сравнение на равенство
bool Int128::operator==(const Int128 &rhs) const {
    return hi_ == rhs.hi_ && lo_ == rhs.lo_;
}
bool Int128::operator!=(const Int128 &rhs) const {
    return !(*this == rhs);
}

// Вывод в поток
std::ostream &operator<<(std::ostream &out, const Int128 &num) {
    return out << num.str();
}

// Строка из бит (с разделителем)
std::string Int128::bit_string(bool separator) const {
    std::bitset<64> high_bits(static_cast<uint64_t>(hi_));
    std::bitset<64> low_bits(lo_);
    std::string result = high_bits.to_string();
    if (separator) {
        result += '|';
    }
    result += low_bits.to_string();
    return result;
}

// Побитовое NOT (~)
Int128 Int128::operator~() const {
    return Int128(~hi_, ~lo_);
}

// Константы 0 и 1
const Int128 Int128::ZERO = Int128(0);
const Int128 Int128::ONE  = Int128(1);

// Битовый сдвиг влево
Int128 Int128::operator<<(int shift) const {
    if (shift <= 0) {
        return *this;
    }
    if (shift >= 128) {
        return ZERO;
    }

    Int128 result;
    if (shift >= 64) {
        result.hi_ = static_cast<int64_t>(lo_ << (shift - 64));
        result.lo_ = 0;
    } else {
        uint64_t high = (static_cast<uint64_t>(hi_) << shift) | (lo_ >> (64 - shift));
        result.hi_ = static_cast<int64_t>(high);
        result.lo_ = lo_ << shift;
    }
    return result;
}
Int128 &Int128::operator<<=(int shift) {
    *this = *this << shift;
    return *this;
}

// Битовый сдвиг вправо
Int128 Int128::operator>>(int shift) const {
    if (shift <= 0) {
        return *this;
    }
    if (shift >= 128) {
        return hi_ < 0 ? Int128(-1) : ZERO;
    }

    Int128 result;
    if (shift >= 64) {
        int inner_shift = shift - 64;
        result.lo_ = static_cast<uint64_t>(hi_ >> inner_shift);
        result.hi_ = hi_ < 0 ? -1 : 0;
    } else {
        result.lo_ = (lo_ >> shift) | (static_cast<uint64_t>(hi_) << (64 - shift));
        result.hi_ = hi_ >> shift;
    }
    return result;
}
Int128 &Int128::operator>>=(int shift) {
    *this = *this >> shift;
    return *this;
}

// Остаток от деления: %, %=
Int128 Int128::operator%(const Int128 &rhs) const {
    if (rhs == ZERO) {
        return ZERO;
    }
    return *this - (*this / rhs) * rhs;
}
Int128 &Int128::operator%=(const Int128 &rhs) {
    *this = *this % rhs;
    return *this;
}

// Сравнение: <, <=
bool Int128::operator<(const Int128 &rhs) const {
    if (hi_ == rhs.hi_) {
        return lo_ < rhs.lo_;
    }
    return hi_ < rhs.hi_;
}
bool Int128::operator<=(const Int128 &rhs) const {
    return *this < rhs || *this == rhs;
}

// Сравнение: >, >=
bool Int128::operator>(const Int128 &rhs) const {
    return rhs < *this;
}
bool Int128::operator>=(const Int128 &rhs) const {
    return rhs <= *this;
}

// Сравнение побитовое (беззнаковое): <, <=
// unsigned сравнение: строгое меньше
bool Int128::unsignedLess(const Int128 &rhs) const {
    if (hi_ == rhs.hi_) {
        return lo_ < rhs.lo_;
    }
    // сравниваем как беззнаковые значения
    return static_cast<uint64_t>(hi_) < static_cast<uint64_t>(rhs.hi_);
}
// unsigned сравнение: меньше или равно
bool Int128::unsignedLessOrEqual(const Int128 &rhs) const {
    return unsignedLess(rhs) || *this == rhs;
}

// Побитовое И: &, &=
Int128 Int128::operator&(const Int128 &rhs) const {
    return Int128(hi_ & rhs.hi_, lo_ & rhs.lo_);
}
Int128 &Int128::operator&=(const Int128 &rhs) {
    *this = *this & rhs;
    return *this;
}

// Побитовое ИЛИ: |, |=
Int128 Int128::operator|(const Int128 &rhs) const {
    return Int128(hi_ | rhs.hi_, lo_ | rhs.lo_);
}
Int128 &Int128::operator|=(const Int128 &rhs) {
    *this = *this | rhs;
    return *this;
}

// Модуль числа: abs()
Int128 Int128::abs() const {
    if (hi_ < 0) {
        return -*this;
    }
    return *this;
}