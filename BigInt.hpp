#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

class BigInt {
    std::vector<int32_t> numVec;
    bool isNegativeVar;

public:
    const static int32_t base = 1000000000;

    /// helper functions
    inline void removeLeadingZeroes() {
        while (numVec.size() > 1 && numVec.back() == 0) {
            numVec.pop_back();
        }
    }
    
    inline int32_t stoint32(const std::string& num) {
        int32_t result = 0;
        for (size_t i = 0; i < num.size(); i++) {
            result = result * 10 + num[i] - 48;
        }
        return result;
    }
    
    inline void checkZeroSign() {
        if (numVec.size() == 0 || (numVec.size() == 1 && numVec.back() == 0)) {
            isNegativeVar = false;
        }
    }
    
    std::string toString();
    static BigInt unsignedAddition(const BigInt& a, const BigInt& b);
    static int32_t unsignedComparison(const BigInt& a, const BigInt& b);
    static BigInt unsignedSubtraction(const BigInt& a, const BigInt& b);
    static BigInt multiplication(const BigInt& a, const BigInt& b);
    static std::pair<BigInt, BigInt> division(const BigInt& a, const BigInt& b); // first - division second - remainder

    // constructors
    BigInt();
    BigInt(int64_t num);
    explicit BigInt(std::string num);
    BigInt(const BigInt& a, bool customIsNegative);

    //destructor
    ~BigInt() {
        // do nothing
    }

    inline bool isNegative() const {
        return isNegativeVar;
    }

    inline std::vector<int32_t> getNumVec() const {
        return numVec;
    }

    /// arithmetic assign operators
    BigInt& operator+=(const BigInt& b);
    BigInt& operator-=(const BigInt& b);

    inline BigInt& operator*=(const BigInt& b) {
        *this = BigInt::multiplication(*this, b);
        return *this;
    }

    BigInt& operator*=(const int32_t& b);

    inline BigInt& operator/=(const BigInt& b) {
        *this = BigInt::division(*this, b).first;
        return *this;
    }

    BigInt& operator/=(const int32_t& b);

    inline BigInt& operator%=(const BigInt& b) {
        *this = BigInt::division(*this, b).second;
        return *this;   
    }
    
    inline BigInt& operator++() {
        *this += 1;
        return *this;
    }

    inline BigInt operator++(int) {
        BigInt b = *this;
        *this += 1;
        return b;
    }
    
    inline BigInt& operator--() {
        *this -= 1;
        return *this;
    }

    inline BigInt operator--(int) {
        BigInt b = *this;
        *this -= 1;
        return b;
    }

    /// arithmetic unary operators
    inline BigInt operator+() const {
        BigInt r = *this;
        return r;
    }

    inline BigInt operator-() const {
        BigInt r = BigInt(*this, !this->isNegative());
        return r;
    }

    int32_t operator%(const int32_t& b);
};

/// input/output operators
inline std::istream& operator>>(std::istream& in, BigInt& num) {
    std::string s;
    in >> s;
    num = BigInt(s);
    return in;
}

inline std::ostream& operator<<(std::ostream& out, BigInt num) {
    out << num.toString();
    return out;
}

/// arithmetic operators
inline BigInt operator+(const BigInt& a, const BigInt& b) {
    BigInt r = a;
    r += b;
    return r;
}

inline BigInt operator-(const BigInt& a, const BigInt& b) {
    BigInt r = a;
    r -= b;
    return r;
}

inline BigInt operator*(const BigInt& a, const BigInt& b) {
    BigInt r = a;
    r *= b;
    return r;
}

inline BigInt operator*(const BigInt& a, const int64_t& b) {
    BigInt r = a;
    r *= b;
    return r;
}

inline BigInt operator/(const BigInt& a, const BigInt& b) {
    BigInt r = a;
    r /= b;
    return r;
}

inline BigInt operator%(const BigInt& a, const BigInt& b) {
    BigInt r = a;
    r %= b;
    return r;
}

/// compare operators
inline bool operator==(const BigInt& a, const BigInt& b) {
    return a.isNegative() == b.isNegative() && a.getNumVec() == b.getNumVec();
}

inline bool operator!=(const BigInt& a, const BigInt& b) {
    return !(a == b);
}

inline bool operator<(const BigInt& a, const BigInt& b) {
    if (a.isNegative() != b.isNegative()) {
        return a.isNegative();
    }
    if (a.isNegative()) {
        return BigInt::unsignedComparison(a, b) == 1;
    } else {
        return BigInt::unsignedComparison(a, b) == -1;
    }
}

inline bool operator>(const BigInt& a, const BigInt& b) {
    return b < a;
}

inline bool operator<=(const BigInt& a, const BigInt& b) {
    return !(a > b);
}

inline bool operator>=(const BigInt& a, const BigInt& b) {
    return b >= a;
}

/// math functions
inline BigInt abs(const BigInt& a) {
    return BigInt(a, false);
}