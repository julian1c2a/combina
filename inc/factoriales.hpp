//
// Created by julia on 21/10/2025.
//

#ifndef COMBINATORIA_FACTORIALES_HPP
#define COMBINATORIA_FACTORIALES_HPP

#include<int128_helpers.hpp>

/**
 * @brief Calcula el factorial de un número en tiempo de compilación para uint128_t.
 *
 * @details Esta función utiliza recursividad de plantilla para calcular el factorial.
 *          Al ser `constexpr`, el cálculo se realiza durante la compilación.
 *          Incluye una guarda para evitar el desbordamiento de `uint128_t`.
 *
 * @tparam n El número entero no negativo del cual se calculará el factorial.
 * @return   El factorial de n (n!) si n < 35, de lo contrario 0 para indicar desbordamiento.
 * @note     El límite de 34 es el máximo factorial que cabe en un `uint128_t`.
 */
template<uint128_t n>
constexpr uint128_t factorial_constexpr() {
    if constexpr ((n == 0) || (n == 1)) {
        return static_cast<uint128_t>(1);
    } else if constexpr (n < 35) {
        return n * factorial_constexpr<n - 1>();
    } else {
        return static_cast<uint128_t>(0); // OVERFLOW
    }
}

/**
 * @brief Calcula el factorial de un número en tiempo de compilación para int128_t.
 *
 * @details Similar a la versión para uint128_t, pero para enteros con signo.
 *          El factorial no está definido para números negativos.
 *
 * @tparam n El número entero no negativo del cual se calculará el factorial.
 * @return   El factorial de n (n!) si 0 <= n < 34. Devuelve 0 para n negativo o para n >= 34 (overflow).
 * @note     El límite de 33 es el máximo factorial que cabe en un `int128_t`.
 */
template<int128_t n>
constexpr int128_t factorial_constexpr() {
    if constexpr (n < 0) {
        return static_cast<int128_t>(0); // Factorial no definido para negativos
    } else if constexpr ((n == 0) || (n == 1)) {
        return static_cast<int128_t>(1);
    } else if constexpr (n < 34) {
        return n * factorial_constexpr<n - 1>();
    } else {
        return static_cast<int128_t>(0); // OVERFLOW
    }
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

#endif //COMBINATORIA_FACTORIALES_HPP
