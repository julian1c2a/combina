#include <int128_helpers.hpp>
#include <algorithm>
#include <stdexcept>

// --- Implementaciones para uint128_t ---

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

uint128_t parse_uint128(const std::string& s) {
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

std::istream& operator>>(std::istream& is, uint128_t& n) {
    std::string s;
    is >> s;
    if (is) {
        try {
            n = parse_uint128(s);
        } catch (const std::invalid_argument&) {
            is.setstate(std::ios_base::failbit);
        } catch (const std::out_of_range&) {
            is.setstate(std::ios_base::failbit);
        }
    }
    return is;
}

// --- Implementaciones para int128_t ---

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

int128_t parse_int128(const std::string& s) {
    if (s.empty()) {
        throw std::invalid_argument("Cannot convert empty string to int128_t");
    }

    if (s[0] == '-') {
        // Manejar número negativo
        if (s.length() == 1) {
            throw std::invalid_argument("Invalid string for int128_t conversion: '-'");
        }
        uint128_t u_val = parse_uint128(s.substr(1));
        uint128_t max_int_plus_1 = static_cast<uint128_t>(1) << 127;
        if (u_val > max_int_plus_1) {
            throw std::out_of_range("Input string represents a number too small for int128_t");
        }
        if (u_val == max_int_plus_1) {
            return -static_cast<int128_t>(max_int_plus_1);
        }
        return -static_cast<int128_t>(u_val);
    } else {
        // Manejar número positivo
        uint128_t u_val = parse_uint128(s);
        uint128_t max_int = (static_cast<uint128_t>(1) << 127) - 1;
        if (u_val > max_int) {
            throw std::out_of_range("Input string represents a number too large for int128_t");
        }
        return static_cast<int128_t>(u_val);
    }
}

std::ostream& operator<<(std::ostream& os, const int128_t& n) {
    return (os << to_string(n));
}

std::ostream& operator<<(std::ostream& os, const uint128_t& n) {
    return (os << to_string(n));
}
