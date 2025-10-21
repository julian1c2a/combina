//
// Created by julia on 21/10/2025.

#include<factoriales.hpp>

constexpr uint128_t factorial(uint128_t n) {
    switch (n) {
        case  0:
        case  1: return factorial_constexpr< 1>();
        case  2: return factorial_constexpr< 2>();
        case  3: return factorial_constexpr< 3>();
        case  4: return factorial_constexpr< 4>();
        case  5: return factorial_constexpr< 5>();
        case  6: return factorial_constexpr< 6>();
        case  7: return factorial_constexpr< 7>();
        case  8: return factorial_constexpr< 8>();
        case  9: return factorial_constexpr< 9>();
        case 10: return factorial_constexpr<10>();
        case 11: return factorial_constexpr<11>();
        case 12: return factorial_constexpr<12>();
        case 13: return factorial_constexpr<13>();
        case 14: return factorial_constexpr<14>();
        case 15: return factorial_constexpr<15>();
        case 16: return factorial_constexpr<16>();
        case 17: return factorial_constexpr<17>();
        case 18: return factorial_constexpr<18>();
        case 19: return factorial_constexpr<19>();
        case 20: return factorial_constexpr<20>();
        case 21: return factorial_constexpr<21>();
        case 22: return factorial_constexpr<22>();
        case 23: return factorial_constexpr<23>();
        case 24: return factorial_constexpr<24>();
        case 25: return factorial_constexpr<25>();
        case 26: return factorial_constexpr<26>();
        case 27: return factorial_constexpr<27>();
        case 28: return factorial_constexpr<28>();
        case 29: return factorial_constexpr<29>();
        case 30: return factorial_constexpr<30>();
        case 31: return factorial_constexpr<31>();
        case 32: return factorial_constexpr<32>();
        case 33: return factorial_constexpr<33>();
        case 34: return factorial_constexpr<34>();
        default: return static_cast<uint128_t>(0); // OVERFLOW
    }
}

constexpr int128_t factorial(int128_t n) {
    if (n < 0) return 0; // El factorial no está definido para números negativos
    switch (n) {
        case  0:
        case  1: return factorial_constexpr<1>();
        case  2: return factorial_constexpr<2>();
        case  3: return factorial_constexpr<3>();
        case  4: return factorial_constexpr<4>();
        case  5: return factorial_constexpr<5>();
        case  6: return factorial_constexpr<6>();
        case  7: return factorial_constexpr<7>();
        case  8: return factorial_constexpr<8>();
        case  9: return factorial_constexpr<9>();
        case 10: return factorial_constexpr<10>();
        case 11: return factorial_constexpr<11>();
        case 12: return factorial_constexpr<12>();
        case 13: return factorial_constexpr<13>();
        case 14: return factorial_constexpr<14>();
        case 15: return factorial_constexpr<15>();
        case 16: return factorial_constexpr<16>();
        case 17: return factorial_constexpr<17>();
        case 18: return factorial_constexpr<18>();
        case 19: return factorial_constexpr<19>();
        case 20: return factorial_constexpr<20>();
        case 21: return factorial_constexpr<21>();
        case 22: return factorial_constexpr<22>();
        case 23: return factorial_constexpr<23>();
        case 24: return factorial_constexpr<24>();
        case 25: return factorial_constexpr<25>();
        case 26: return factorial_constexpr<26>();
        case 27: return factorial_constexpr<27>();
        case 28: return factorial_constexpr<28>();
        case 29: return factorial_constexpr<29>();
        case 30: return factorial_constexpr<30>();
        case 31: return factorial_constexpr<31>();
        case 32: return factorial_constexpr<32>();
        case 33: return factorial_constexpr<33>();
        default: return static_cast<int128_t>(0); // OVERFLOW o n < 0
    }
}
