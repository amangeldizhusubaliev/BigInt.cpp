#include "BigInt.hpp"

/// helper functions
std::string BigInt::toString() {
    std::stringstream ss;
    if (isNegativeVar) {
        ss << "-";
    }
    ss << numVec.back();
    if (numVec.size() > 1) {
        for (size_t i = numVec.size() - 2; i >= 0; i--) {
            ss << std::setfill('0') << std::setw(9) << numVec[i];
            if (i == 0) {
                break;
            }
        }
    }
    return ss.str();
}

BigInt BigInt::unsignedAddition(const BigInt& a, const BigInt& b) {
    BigInt res;
    res.numVec = std::vector<int32_t>(0);
    int32_t carry = 0;
    for (size_t i = 0; carry > 0 || i < a.numVec.size() || i < b.numVec.size(); i++) {
        carry += (i < a.numVec.size() ? a.numVec[i] : 0) + (i < b.numVec.size() ? b.numVec[i] : 0);
        res.numVec.emplace_back(carry % base);
        carry /= base;
    }
    return res;
}

int32_t BigInt::unsignedComparison(const BigInt& a, const BigInt& b) {
    if (a.numVec.size() != b.numVec.size()) {
        return a.numVec.size() > b.numVec.size() ? 1 : -1;
    }
    for (size_t i = a.numVec.size() - 1; i >= 0; i--) {
        if (a.numVec[i] != b.numVec[i]) {
            return a.numVec[i] > b.numVec[i] ? 1 : -1;
        }
        if (i == 0) {
            break;
        }
    }
    return 0;
}

BigInt BigInt::unsignedSubtraction(const BigInt& a, const BigInt& b) {
    if (unsignedComparison(a, b) == -1) {
        return unsignedSubtraction(b, a);
    } 
    BigInt res;
    res.numVec = std::vector<int32_t>(0);
    int32_t carry = 0;
    for (size_t i = 0; carry != 0 || i < a.numVec.size(); i++) {
        carry += (i < a.numVec.size() ? a.numVec[i] : 0) - (i < b.numVec.size() ? b.numVec[i] : 0);
        if (carry < 0) {
            res.numVec.emplace_back(carry + base);
            carry = -1;
        } else {
            res.numVec.emplace_back(carry);
            carry = 0;
        }
    }
    res.removeLeadingZeroes();
    return res;
}

BigInt BigInt::multiplication(const BigInt& a, const BigInt& b) {
    BigInt r;
    r.numVec.resize(a.numVec.size() + b.numVec.size());
    for (size_t i = 0; i < a.numVec.size(); i++) {
        int64_t carry = 0;
        for (size_t j = 0; j < b.numVec.size() || carry > 0; j++) {
            carry += r.numVec[i + j] + a.numVec[i] * 1LL * (j < b.numVec.size() ? b.numVec[j] : 0);
            r.numVec[i + j] = int32_t(carry % base);
            carry /= base;
        }
    }
    r.isNegativeVar = a.isNegativeVar != b.isNegativeVar;
    r.removeLeadingZeroes();
    r.checkZeroSign();
    return r;
}

std::pair<BigInt, BigInt> BigInt::division(const BigInt& a, const BigInt& b) {
    BigInt p, q = BigInt(b, false), r;
    r.numVec.resize(0);
    size_t ptr = a.numVec.size() - 1;
    while (ptr >= 0 && ptr < a.numVec.size()) {
        p *= base;
        p += a.numVec[ptr--];
        while (p < q && ptr >= 0 && ptr < a.numVec.size()) {
            r.numVec.emplace_back(0);
            p *= base;
            p += a.numVec[ptr--];
        }
        if (p < q) {
            r.numVec.emplace_back(0);
            break;
        }
        int32_t L = 1, R = base; 
        while (L < R) {
            int32_t M = (L + R + 1) >> 1;
            if (q * M <= p) {
                L = M;
            } else {
                R = M - 1;
            }
        }
        r.numVec.emplace_back(L);
        p -= q * L;
    }
    std::reverse(r.numVec.begin(), r.numVec.end());
    r.isNegativeVar = a.isNegativeVar != b.isNegativeVar;
    r.removeLeadingZeroes();
    r.checkZeroSign();
    p.isNegativeVar = a.isNegativeVar;
    p.checkZeroSign();
    return std::make_pair(r, p);
}

/// constructors
BigInt::BigInt() : isNegativeVar(false) {
    numVec = std::vector<int32_t>(1, 0);
}

BigInt::BigInt(int64_t num) {
    isNegativeVar = false;
    numVec = std::vector<int32_t>(0);
    if (num < 0) {
        isNegativeVar = true;
        num *= -1;
    }
    numVec.emplace_back(int32_t(num % base));
    num /= base;
    if (num > 0) {
        numVec.emplace_back(int32_t(num % base));
        num /= base;
        if (num > 0) {
            numVec.emplace_back(int32_t(num));
        }
    }
}

BigInt::BigInt(std::string num) {
    if (num.empty()) {
        throw std::runtime_error("string cannot be empty");
    }
    if (!(num[0] == '-' || num[0] == '+' || isdigit(num[0]))) {
        throw std::runtime_error("incorrect string " + num);
    }
    for (size_t i = 1; i < num.size(); i++) {
        if (!isdigit(num[i])) {
            throw std::runtime_error("incorrect string " + num);
        }
    }
    isNegativeVar = false;
    size_t start = 0;
    if (num[0] == '-' || num[0] == '+') {
        isNegativeVar = num[0] == '-';
        ++start;
    }
    numVec = std::vector<int32_t>(0);
    for (size_t i = num.size() - 1; i >= start; i -= 9) {
        size_t j = start + 8 <= i ? i - 8 : start;
        numVec.emplace_back(stoint32(num.substr(j, i - j + 1)));
        if (i < 9) {
            break;
        }
    }
    removeLeadingZeroes();
    checkZeroSign();
}

BigInt::BigInt(const BigInt &a, bool customIsNegative) {
    numVec = a.numVec;
    isNegativeVar = customIsNegative;
}

/// arithmetic operators
BigInt& BigInt::operator+=(const BigInt& b) {
    bool customIsNegative;
    if (this->isNegative() != b.isNegative()) {
        int cmp = BigInt::unsignedComparison(*this, b);
        customIsNegative = (this->isNegative() && cmp == 1) || (b.isNegative() && cmp == -1);
        *this = BigInt::unsignedSubtraction(*this, b);
    } else {
        customIsNegative = this->isNegative();
        *this = BigInt::unsignedAddition(*this, b);
    }
    this->isNegativeVar = customIsNegative;
    this->checkZeroSign();
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& b) {
    bool customIsNegative;
    if (this->isNegative() != b.isNegative()) {
        customIsNegative = this->isNegative();
        *this = BigInt::unsignedAddition(*this, b);
    } else {
        int32_t cmp = BigInt::unsignedComparison(*this, b);
        customIsNegative = (this->isNegative() && cmp == 1) || (!this->isNegative() && cmp == -1);
        *this = BigInt::unsignedSubtraction(*this, b);
    }
    this->isNegativeVar = customIsNegative;
    this->checkZeroSign();
    return *this;
}

BigInt& BigInt::operator*=(const int32_t& b) {
    int64_t carry = 0;
    for (size_t i = 0; i < this->numVec.size() || carry; i++) {
        if (i == this->numVec.size()) {
            this->numVec.emplace_back(0);
        }
        carry += this->numVec[i] * 1LL * b;
        this->numVec[i] = carry % base;
        carry /= base;
    }
    removeLeadingZeroes();
    checkZeroSign();
    return *this;
}

BigInt& BigInt::operator/=(const int32_t& b) {
    int64_t carry = 0;
    for (size_t i = this->numVec.size() - 1; i >= 0; i--) {
        carry = this->numVec[i] + carry * base;
        this->numVec[i] = carry / b;
        carry %= b;
        if (i == 0) {
            break;
        }
    }
    removeLeadingZeroes();
    return *this;
}

int32_t BigInt::operator%(const int32_t& b) {
    int32_t r = 0;
    for (size_t i = this->numVec.size() - 1; i >= 0; i--) {
        r = (r * 1LL * base + this->numVec[i]) % b;
        if (i == 0) {
            break;
        }
    }
    return r;
}