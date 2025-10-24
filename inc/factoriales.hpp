#pragma once

#include <cstdint>
#include <type_traits>
#include <concepts>
#include <int128_helpers.hpp> // Para los tipos de 128 bits

// --- Concepts para la selección de tipos ---
template<typename T>
concept Is128BitInteger = std::is_same_v<T, int128_t> || std::is_same_v<T, uint128_t>;

template<typename T>
concept Is64BitInteger = std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>;

template<typename T>
concept IsFactorialSupportedInteger = Is64BitInteger<T> || Is128BitInteger<T>;

// --- Implementaciones de bajo nivel (privadas a este fichero) ---
namespace detail {
    // Implementación recursiva con desenrollado manual y comprobaciones de límites integradas.
    template<IsFactorialSupportedInteger T, T N>
    constexpr T factorial_impl() {
        // 1. Comprobar límites y valores inválidos PRIMERO para evitar warnings de overflow.
        if constexpr (std::is_signed_v<T>) {
            if constexpr (N < 0) return -1; // DEVOLVEMOS -1 para negativos
            if constexpr (Is64BitInteger<T> && N >= 21) return 0;
            if constexpr (Is128BitInteger<T> && N >= 34) return 0;
        } else {
            if constexpr (Is64BitInteger<T> && N >= 21) return 0;
            if constexpr (Is128BitInteger<T> && N >= 35) return 0;
        }

        // 2. Casos base para la recursión.
        if constexpr (N == 0) {
            return 1;
        } else if constexpr (N < 4) {
            return N * factorial_impl<T, N - 1>();
        } else {
            // 3. Desenrollando de la recursión.
            return N * (N - 1) * (N - 2) * (N - 3) * factorial_impl<T, N - 4>();
        }
    }
} // namespace detail

/**
 * @brief Calcula el factorial de un número en tiempo de compilación (versión de plantilla).
 * @return El factorial de N. Devuelve -1 para N negativo (tipos con signo) o 0 para overflow.
 */
template<IsFactorialSupportedInteger T, T N>
constexpr T factorial_ct() {
    return detail::factorial_impl<T, N>();
}

/**
 * @brief Calcula el factorial de un número en tiempo de ejecución o compilación.
 * @return El factorial de n. Devuelve -1 para n negativo (tipos con signo) o 0 para overflow.
 */
template<IsFactorialSupportedInteger T>
constexpr T factorial(T n) {
    if constexpr (std::is_signed_v<T>) {
        if (n < 0) return -1; // DEVOLVEMOS -1 para negativos
    }

    if constexpr (Is64BitInteger<T>) {
        switch (n) {
            case 0:
            case 1: return factorial_ct<T, 1>();
            case 2: return factorial_ct<T, 2>();
            case 3: return factorial_ct<T, 3>();
            case 4: return factorial_ct<T, 4>();
            case 5: return factorial_ct<T, 5>();
            case 6: return factorial_ct<T, 6>();
            case 7: return factorial_ct<T, 7>();
            case 8: return factorial_ct<T, 8>();
            case 9: return factorial_ct<T, 9>();
            case 10: return factorial_ct<T, 10>();
            case 11: return factorial_ct<T, 11>();
            case 12: return factorial_ct<T, 12>();
            case 13: return factorial_ct<T, 13>();
            case 14: return factorial_ct<T, 14>();
            case 15: return factorial_ct<T, 15>();
            case 16: return factorial_ct<T, 16>();
            case 17: return factorial_ct<T, 17>();
            case 18: return factorial_ct<T, 18>();
            case 19: return factorial_ct<T, 19>();
            case 20: return factorial_ct<T, 20>();
            default: return 0; // Overflow
        }
    } else if constexpr (Is128BitInteger<T>) {
        if constexpr (std::is_signed_v<T>) {
            switch (n) {
                case 0:
                case 1: return factorial_ct<T, 1>();
                case 2: return factorial_ct<T, 2>();
                case 3: return factorial_ct<T, 3>();
                case 4: return factorial_ct<T, 4>();
                case 5: return factorial_ct<T, 5>();
                case 6: return factorial_ct<T, 6>();
                case 7: return factorial_ct<T, 7>();
                case 8: return factorial_ct<T, 8>();
                case 9: return factorial_ct<T, 9>();
                case 10: return factorial_ct<T, 10>();
                case 11: return factorial_ct<T, 11>();
                case 12: return factorial_ct<T, 12>();
                case 13: return factorial_ct<T, 13>();
                case 14: return factorial_ct<T, 14>();
                case 15: return factorial_ct<T, 15>();
                case 16: return factorial_ct<T, 16>();
                case 17: return factorial_ct<T, 17>();
                case 18: return factorial_ct<T, 18>();
                case 19: return factorial_ct<T, 19>();
                case 20: return factorial_ct<T, 20>();
                case 21: return factorial_ct<T, 21>();
                case 22: return factorial_ct<T, 22>();
                case 23: return factorial_ct<T, 23>();
                case 24: return factorial_ct<T, 24>();
                case 25: return factorial_ct<T, 25>();
                case 26: return factorial_ct<T, 26>();
                case 27: return factorial_ct<T, 27>();
                case 28: return factorial_ct<T, 28>();
                case 29: return factorial_ct<T, 29>();
                case 30: return factorial_ct<T, 30>();
                case 31: return factorial_ct<T, 31>();
                case 32: return factorial_ct<T, 32>();
                case 33: return factorial_ct<T, 33>();
                default: return 0; // Overflow
            }
        } else {
            // unsigned 128-bit
            switch (n) {
                case 0:
                case 1: return factorial_ct<T, 1>();
                case 2: return factorial_ct<T, 2>();
                case 3: return factorial_ct<T, 3>();
                case 4: return factorial_ct<T, 4>();
                case 5: return factorial_ct<T, 5>();
                case 6: return factorial_ct<T, 6>();
                case 7: return factorial_ct<T, 7>();
                case 8: return factorial_ct<T, 8>();
                case 9: return factorial_ct<T, 9>();
                case 10: return factorial_ct<T, 10>();
                case 11: return factorial_ct<T, 11>();
                case 12: return factorial_ct<T, 12>();
                case 13: return factorial_ct<T, 13>();
                case 14: return factorial_ct<T, 14>();
                case 15: return factorial_ct<T, 15>();
                case 16: return factorial_ct<T, 16>();
                case 17: return factorial_ct<T, 17>();
                case 18: return factorial_ct<T, 18>();
                case 19: return factorial_ct<T, 19>();
                case 20: return factorial_ct<T, 20>();
                case 21: return factorial_ct<T, 21>();
                case 22: return factorial_ct<T, 22>();
                case 23: return factorial_ct<T, 23>();
                case 24: return factorial_ct<T, 24>();
                case 25: return factorial_ct<T, 25>();
                case 26: return factorial_ct<T, 26>();
                case 27: return factorial_ct<T, 27>();
                case 28: return factorial_ct<T, 28>();
                case 29: return factorial_ct<T, 29>();
                case 30: return factorial_ct<T, 30>();
                case 31: return factorial_ct<T, 31>();
                case 32: return factorial_ct<T, 32>();
                case 33: return factorial_ct<T, 33>();
                case 34: return factorial_ct<T, 34>();
                default: return 0; // Overflow
            }
        }
    }
}
