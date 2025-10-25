#pragma once


#include <type_traits_of_factorials.hpp> // Para los tipos de 128 bits


/**
 * @brief Obtiene un factorial precalculado en tiempo de compilación.
 * @return El factorial de N. Devuelve -1 para N negativo (tipos con signo) o 0 para overflow.
 */
template<IsFactorialSupportedInteger T, T N>
constexpr T factorial_ct_val() {
    using implementation_details::factorial_from_table;
    using std::is_signed_v;

    // Validación del argumento de plantilla N
    if constexpr (is_signed_v<T> && N < 0) {
        return -1;
    }

    // Reutilizamos la lógica centralizada y robusta de factorial_from_table.
    // Esto elimina la duplicación de código y la tabla redundante.
    constexpr T result = factorial_from_table(static_cast<T>(N));
    return result;

    /* --- CÓDIGO ORIGINAL COMENTADO ---
     * El código de abajo es la implementación original, que es compleja y redundante.
     * Se ha reemplazado por la llamada a factorial_from_table de arriba.

    using implementation_details::get_factorial_limit;
    using implementation_details::generate_extended_factorial_table;
    static constexpr size_t factorial_limit{get_factorial_limit<T>()};
    static constexpr std::array<T, 35> factorial_table{generate_extended_factorial_table<T, 35>()};

    if constexpr (std::is_signed_v<T> && (N < 0)) return -1;
    else if constexpr (N > factorial_limit) return 0;
    else if constexpr (Is8To128BitInteger<T>)
        return static_cast<T>(factorial_table[static_cast<size_t>(N)]);
    else if constexpr (Is256To1024BitInteger<T>) {
        if constexpr (std::is_signed_v<T> && (N < 0)) {
            return -1;
        } else if constexpr (N > get_factorial_limit<T>()) {
            return 0;
        } else {
            if constexpr (N <= 34) {
                return static_cast<T>(factorial_table[static_cast<size_t>(N)]);
            } else {
                T result{factorial_table[34]};
                for (T i{34}; i <= static_cast<T>(N); ++i)
                    result *= i;
                return result;
            }
        }
    } else if constexpr (IsInftyInteger<T>) {
        if constexpr (is_signed_v<T> && (N < 0))
            return -1;
        else {
            T result{factorial_table[2]};
            for (T i = 3; i <= N; ++i) {
                result *= i;
            }
            return result;
        }
    } else return 0;
    */
}

/**
* @brief Calcula el factorial de un número en tiempo de ejecución o compilación.
* @details Utiliza una tabla de búsqueda generada automáticamente para devolver valores precalculados.
* @return El factorial de n. Devuelve -1 para n negativo (tipos con signo) o 0 para overflow.
*/
template<IsFactorialSupportedInteger T>
constexpr T factorial(T n) {
    using implementation_details::factorial_from_table;
    using std::is_signed_v;

    if constexpr (is_signed_v<T>) {
        if (n < 0) {
            return -1; // Error para n negativo
        }
    }

    // Delegamos toda la lógica a la función auxiliar que ya usa la tabla
    // y maneja los límites y los tipos de precisión arbitraria.
    return factorial_from_table(n);
} // end function factorial (template T)
