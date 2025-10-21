#pragma once

#include <cstdint>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>

// Alias de tipos para enteros de 128 bits
using int128_t = __int128;
using uint128_t = __uint128_t;

// --- Declaraciones y Definiciones para uint128_t ---

std::string to_string(const uint128_t& n) {
    if (n == 0) return "0";
    else {
        std::string s;
        s.clear();
        uint128_t tmp{n};
        while (tmp > 0) {
            s += '0' + static_cast<char>(tmp % 10);
            tmp /= 10;
        }
        std::reverse(s.begin(), s.end());
        return s;
    }
}

uint128_t from_string(const std::string& s) {
    uint128_t res = 0;
    if (s.empty()) {
        throw std::invalid_argument("Cannot convert empty string to uint128_t");
    }
    for (char c : s) {
        if (c >= '0' && c <= '9') {
            uint128_t prev_res = res;
            res = res * 10 + (c - '0');
            if (res < prev_res) {
                 throw std::out_of_range("Input string represents a number too large for uint128_t");
            }
        } else {
            throw std::invalid_argument("Invalid character in string for uint128_t conversion");
        }
    }
    return res;
}

std::ostream& operator<<(std::ostream& os, const uint128_t& n) {
    return (os << to_string(n));
}

std::istream& operator>>(std::istream& is, uint128_t& n) {
    std::string s;
    is >> s;
    if (is) {
        try {
            n = from_string(s);
        } catch (const std::invalid_argument&) {
            is.setstate(std::ios_base::failbit);
        } catch (const std::out_of_range&) {
            is.setstate(std::ios_base::failbit);
        }
    }
    return is;
}

// --- Declaraciones y Definiciones para int128_t ---

std::string to_string(const int128_t& n) {
    if (n == 0) {
        return "0";
    }
    else {
        std::string s;
        s.clear();
        uint128_t tmp{0};
        if (n < 0) {
          tmp = -n;
          s += '-';
        }
        else {
          tmp = n;
        }

        std::string num_part;
        while (tmp > 0) {
          num_part += '0' + static_cast<char>(tmp % 10);
          tmp /= 10;
        }
        std::reverse(num_part.begin(), num_part.end());
        s += num_part;
        return s;
    }
}

std::ostream& operator<<(std::ostream& os, const int128_t& n) {
    return (os << to_string(n));
}
