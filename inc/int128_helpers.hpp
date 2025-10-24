#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <stdexcept>

// Alias de tipos para enteros de 128 bits
using int128_t = __int128;
using uint128_t = __uint128_t;

// --- Declaraciones para uint128_t ---
std::string to_string(const uint128_t& n);
uint128_t parse_uint128(const std::string& s);
std::ostream& operator<<(std::ostream& os, const uint128_t& n);
std::istream& operator>>(std::istream& is, uint128_t& n);

// --- Declaraciones para int128_t ---
std::string to_string(const int128_t& n);
int128_t parse_int128(const std::string& s);
std::ostream& operator<<(std::ostream& os, const int128_t& n);

// --- Literales de usuario para enteros de 64 y 128 bits ---
namespace integer_literals {
    // --- Literales de 128 bits ---
    inline uint128_t operator""_u128(const char* str, std::size_t len) {
        return parse_uint128(std::string(str, len));
    }
    constexpr inline uint128_t operator""_u128(unsigned long long val) {
        return static_cast<uint128_t>(val);
    }
    inline int128_t operator""_i128(const char* str, std::size_t len) {
        return parse_int128(std::string(str, len));
    }
    constexpr inline int128_t operator""_i128(unsigned long long val) {
        return static_cast<int128_t>(val);
    }

    // --- Literales de 64 bits ---
    constexpr inline std::uint64_t operator""_u64(unsigned long long val) {
        return static_cast<std::uint64_t>(val);
    }
    constexpr inline std::int64_t operator""_i64(unsigned long long val) {
        return static_cast<std::int64_t>(val);
    }
}
