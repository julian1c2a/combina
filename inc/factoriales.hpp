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
    /**
     * @brief Implementación del factorial en tiempo de compilación para enteros de 64 bits.
     */
    template<Is64BitInteger T, T N>
    constexpr T factorial_constexpr_64bits() {
        if constexpr (std::is_signed_v<T>) {
            if constexpr (N < 0) return -1;
            if constexpr (N < 21) return N > 1 ? N * factorial_constexpr_64bits<T, N - 1>() : 1;
        } else {
            if constexpr (N < 21) return N > 1 ? N * factorial_constexpr_64bits<T, N - 1>() : 1;
        }
        return 0; // OVERFLOW
    }

    /**
     * @brief Implementación del factorial en tiempo de compilación para enteros de 128 bits.
     */
    template<Is128BitInteger T, T N>
    constexpr T factorial_constexpr_128bits() {
        if constexpr (std::is_signed_v<T>) {
            if constexpr (N < 0) return -1;
            if constexpr (N < 34) return N > 1 ? N * factorial_constexpr_128bits<T, N - 1>() : 1;
        } else {
            if constexpr (N < 35) return N > 1 ? N * factorial_constexpr_128bits<T, N - 1>() : 1;
        }
        return 0; // OVERFLOW
    }
} // namespace detail

/**
 * @brief Calcula el factorial de un número en tiempo de compilación.
 *
 * @details Delega a la implementación correcta (64 o 128 bits) según el tipo T.
 * @tparam T El tipo de dato (int64_t, uint64_t, int128_t, uint128_t).
 * @tparam N El número del cual se calculará el factorial.
 * @return El factorial de N, o 0 si hay desbordamiento o -1 si N es negativo.
 */
template<IsFactorialSupportedInteger T, T N>
constexpr T factorial_ct() {
    if constexpr (Is64BitInteger<T>) {
        return detail::factorial_constexpr_64bits<T, N>();
    } else if constexpr (Is128BitInteger<T>) {
        return detail::factorial_constexpr_128bits<T, N>();
    }
}

/**
 * @brief Calcula el factorial de un número en tiempo de ejecución o compilación.
 *
 * @details Utiliza una tabla de búsqueda (switch) para devolver valores precalculados.
 *          Si el argumento `n` es una constante, el resultado se calcula en tiempo de compilación.
 * @tparam T El tipo de dato (int64_t, uint64_t, int128_t, uint128_t).
 * @param n El número del cual se calculará el factorial.
 * @return El factorial de n, o 0 si hay desbordamiento o -1 si n es negativo.
 */
template<IsFactorialSupportedInteger T>
constexpr T factorial(T n) {
    if (n < 0) return -1;

    if constexpr (Is64BitInteger<T>) {
        if (n > 20) return 0; // Límite para 64 bits
    } else if constexpr (Is128BitInteger<T>) {
        if constexpr (std::is_signed_v<T>) {
            if (n > 33) return 0; // Límite para int128_t
        } else {
            if (n > 34) return 0; // Límite para uint128_t
        }
    }

    // La tabla de búsqueda se expandirá según sea necesario por el compilador
    switch (n) {
        case 0:
        case 1: return 1;
        case 2: return 2;
        case 3: return 6;
        case 4: return 24;
        case 5: return 120;
        case 6: return 720;
        case 7: return 5040;
        case 8: return 40320;
        case 9: return 362880;
        case 10: return 3628800;
        // ... (se pueden añadir más casos si se desea, pero la recursividad los cubre)
        default:
            if (n > 1) return n * factorial(n - 1);
            else return 0;
    }
}
