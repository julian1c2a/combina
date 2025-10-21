#pragma once

#include <cstdint>
#include <string>
#include <iostream>

// Alias de tipos para enteros de 128 bits
using int128_t = __int128;
using uint128_t = __uint128_t;

// --- Declaraciones para uint128_t ---
std::string to_string(const uint128_t& n);
uint128_t from_string(const std::string& s);
std::ostream& operator<<(std::ostream& os, const uint128_t& n);
std::istream& operator>>(std::istream& is, uint128_t& n);

// --- Declaraciones para int128_t ---
std::string to_string(const int128_t& n);
std::ostream& operator<<(std::ostream& os, const int128_t& n);
