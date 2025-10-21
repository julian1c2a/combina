#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <type_traits> // Para std::is_signed_v, std::is_unsigned_v
#include <concepts>    // Para concepts

// Alias de tipos para enteros de 128 bits
using int128_t = __int128;
using uint128_t = __uint128_t;

// Concept para asegurar que el tipo es un entero de 128 bits
template<typename T>
concept Is128BitInteger = std::is_same_v<T, int128_t> || std::is_same_v<T, uint128_t>;

/**
 * @brief Calcula el factorial de un número en tiempo de compilación.
 *
 * @details Esta función utiliza recursividad de plantilla para calcular el factorial.
 *          Al ser `constexpr`, el cálculo se realiza durante la compilación.
 *          Maneja automáticamente los límites de desbordamiento para tipos con y sin signo.
 *
 * @tparam N El número entero no negativo del cual se calculará el factorial.
 * @tparam T El tipo de dato numérico (int128_t o uint128_t) sobre el que se realiza el cálculo.
 * @return   El factorial de N (N!) si N está dentro del rango representable por T.
 *           Devuelve 0 para N negativo (si T es con signo) o para N que cause desbordamiento.
 * @note     El límite de 34 es el máximo factorial que cabe en un `uint128_t`.
 *           El límite de 33 es el máximo factorial que cabe en un `int128_t`.
 */
template<Is128BitInteger T, T N>
constexpr T factorial_constexpr() {
    if constexpr (std::is_signed_v<T>) {
        // Lógica para int128_t
        if constexpr (N < 0) {
            return static_cast<T>(0); // Factorial no definido para negativos
        } else if constexpr ((N == 0) || (N == 1)) {
            return static_cast<T>(1);
        } else if constexpr (N < 34) {
            // Límite para int128_t
            return N * factorial_constexpr<T, N - 1>();
        } else {
            return static_cast<T>(0); // OVERFLOW
        }
    } else if constexpr (std::is_unsigned_v<T>) {
        // Lógica para uint128_t
        if constexpr ((N == 0) || (N == 1)) {
            return static_cast<T>(1);
        } else if constexpr (N < 35) {
            // Límite para uint128_t
            return N * factorial_constexpr<T, N - 1>();
        } else {
            return static_cast<T>(0); // OVERFLOW
        }
    }
    // Esto no debería ser alcanzable con el concept Is128BitInteger
    return static_cast<T>(0);
}

/**
 * @brief Calcula el factorial de un número (versión para uint128_t).
 * @details Si el argumento es una constante de tiempo de compilación, el cálculo se
 *          realizará durante la compilación. De lo contrario, se ejecutará en tiempo
 *          de ejecución utilizando una tabla de búsqueda rápida.
 *
 * @param n El número entero no negativo (de tipo uint128_t) del cual se calculará el factorial.
 * @return  El factorial de n (n!) si n <= 34. Si n > 34, devuelve 0 para indicar desbordamiento.
 * @note    El valor máximo de entrada es 34, ya que 35! excede la capacidad de un `uint128_t`.
 */
constexpr uint128_t factorial(uint128_t n);

/**
 * @brief Calcula el factorial de un número (versión para int128_t).
 * @details Si el argumento es una constante de tiempo de compilación, el cálculo se
 *          realizará durante la compilación. De lo contrario, se ejecutará en tiempo
 *          de ejecución utilizando una tabla de búsqueda rápida.
 *
 * @param n El número entero no negativo (de tipo int128_t) del cual se calculará el factorial.
 * @return  El factorial de n (n!) si 0 <= n <= 33. Devuelve 0 para n negativo o para n >= 34 (overflow).
 * @note    El valor máximo de entrada es 33, ya que 34! excede la capacidad de un `int128_t`.
 */
constexpr int128_t factorial(int128_t n);

