#pragma once


#include<boost/multiprecision/cpp_int.hpp>
#include<boost/multiprecision/gmp.hpp>
#include <cstdint>
#include <type_traits>
#include <concepts>
#include <array>
#include <int128_helpers.hpp> // Para los tipos de 128 bits

namespace mp = boost::multiprecision;
using int256_t = mp::int256_t;
using uint256_t = mp::uint256_t;
using int512_t = mp::int512_t;
using uint512_t = mp::uint512_t;
using int1024_t = mp::int1024_t;
using uint1024_t = mp::uint1024_t;
using mp_int_t = mp::gmp_int;
using std::size_t;

// --- Concepts para la selección de tipos ---
template<typename T>
concept Is1024BitInteger = std::is_same_v<T, int1024_t> || std::is_same_v<T, uint1024_t>;

template<typename T>
concept Is512BitInteger = std::is_same_v<T, int512_t> || std::is_same_v<T, uint512_t>;

template<typename T>
concept Is256BitInteger = std::is_same_v<T, int256_t> || std::is_same_v<T, uint256_t>;

template<typename T>
concept Is128BitInteger = std::is_same_v<T, int128_t> || std::is_same_v<T, uint128_t>;

template<typename T>
concept Is64BitInteger = std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>;

template<typename T>
concept Is32BitInteger = std::is_same_v<T, int32_t> || std::is_same_v<T, uint32_t>;

template<typename T>
concept Is16BitInteger = std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>;

template<typename T>
concept Is8BitInteger = std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t>;

template<typename T>
concept IsFactorialSupportedInteger =
        Is8BitInteger<T> || Is16BitInteger<T> || Is32BitInteger<T> ||
        Is64BitInteger<T> || Is128BitInteger<T> || Is256BitInteger<T> ||
        Is512BitInteger<T> || Is1024BitInteger<T> || std::is_same_v<T, mp_int_t>;

template<typename T>
concept IsIntegerTypeInTheFactorialTable =
        Is8BitInteger<T> || Is16BitInteger<T> || Is32BitInteger<T> ||
        Is64BitInteger<T> || Is128BitInteger<T>;

template<typename T>
concept IsIntegerTypeOutTheFactorialTable =
        Is256BitInteger<T> ||
        Is512BitInteger<T> ||
        Is1024BitInteger<T> ||
        std::is_same_v<T, mp_int_t>;
template<typename T>
concept Is8To128BitInteger =
        Is8BitInteger<T> ||
        Is16BitInteger<T> ||
        Is32BitInteger<T> ||
        Is64BitInteger<T> ||
        Is128BitInteger<T>;
template<typename T>
concept Is256To1024BitInteger =
        Is256BitInteger<T> ||
        Is512BitInteger<T> ||
        Is1024BitInteger<T>;

// --- Implementaciones de bajo nivel (privadas a este fichero) ---
namespace detail {
    template<typename T>
    constexpr auto get_factorial_limit() -> std::size_t {
        if constexpr (Is8BitInteger<T>) {
            return 5;
        } else if constexpr (Is16BitInteger<T>) {
            if constexpr (std::is_signed_v<T>) {
                return 7;
            } else {
                return 8;
            }
        } else if constexpr (Is32BitInteger<T>) {
            return 12;
        } else if constexpr (Is64BitInteger<T>) {
            return 20;
        } else if constexpr (Is128BitInteger<T>) {
            if constexpr (std::is_signed_v<T>) {
                return 33;
            } else {
                return 34;
            }
        } else if constexpr (Is256BitInteger<T>) {
            return 57;
        } else if constexpr (Is512BitInteger<T>) {
            if constexpr (std::is_signed_v<T>) {
                return 97;
            } else {
                return 98;
            }
        } else if constexpr (Is1024BitInteger<T>) {
            return 170;
        } else {
            return 0; // Solo para tipo entero de longitud arbitraria (gmp_int)
        }
    }

    // Genera una tabla de factoriales en tiempo de compilación.
    template<typename T>
    constexpr auto generate_factorial_table() -> std::array<T, 35> {
        std::array<T, 35> table{};
        table[0] = 1;
        for (size_t i = 1; i < table.size(); ++i) {
            table[i] = table[i - 1] * i;
        }
        return table;
    }

    // Genera una tabla extendida de factoriales para tipos más grandes
    template<typename T, std::size_t Size>
    constexpr auto generate_extended_factorial_table() -> std::array<T, Size> {
        std::array<T, Size> table{};
        table[0] = 1;
        for (size_t i = 1; i < Size; ++i) {
            table[i] = table[i - 1] * static_cast<T>(i);
        }
        return table;
    }

    // Función auxiliar para obtener el factorial de una tabla estática
    template<typename T>
    constexpr T factorial_from_table(T n) {
        constexpr std::size_t limit = get_factorial_limit<T>();

        if constexpr (limit <= 34) {
            // Para tipos pequeños, usamos la tabla de 35 elementos
            static constexpr std::array<T, 35> table = generate_factorial_table<T>();
            return table[static_cast<std::size_t>(n)];
        } else if constexpr (limit <= 170) {
            // Para tipos más grandes, generamos una tabla extendida
            static constexpr std::array<T, 171> table = generate_extended_factorial_table<T, 171>();
            return table[static_cast<std::size_t>(n)];
        } else {
            // Para tipos de precisión arbitraria (gmp_int), calculamos en runtime
            T result = 1;
            for (T i = 2; i <= n; ++i) {
                result *= i;
            }
            return result;
        }
    }
} // namespace detail

/**
 * @brief Obtiene un factorial precalculado en tiempo de compilación.
 * @return El factorial de N. Devuelve -1 para N negativo (tipos con signo) o 0 para overflow.
 */
template<IsFactorialSupportedInteger T, T N>
constexpr T factorial_ct() {
    using detail::get_factorial_limit;
    using detail::generate_factorial_table;
    using std::is_signed_v;
    static constexpr size_t factorial_limit{get_factorial_limit<T>()};

    static constexpr std::array<T, 35> factorial_table{generate_factorial_table<T>()};

    if constexpr (std::is_signed_v<T> && (N < 0)) {
        return -1; // Error argumento negativo
    } else if constexpr (N > factorial_limit) {
        return 0; // Overflow
    } else if constexpr (Is8To128BitInteger<T>) {
        return static_cast<T>(factorial_table[static_cast<size_t>(N)]);
    } else if constexpr (Is256To1024BitInteger<T>) {
        if constexpr (std::is_signed_v<T> && (N < 0)) {
            return -1; // N Negativo es un error en el argumento
        } else if constexpr (N > get_factorial_limit<T>()) {
            return 0; // Overflow
        } else {
            if constexpr (N <= 34) {
                return static_cast<T>(factorial_table[static_cast<size_t>(N)]);
            } else {
                T result{factorial_table[34]}; // Empezamos desde 34!
                for (T i = 34; i < (N + 1); ++i) {
                    result *= i;
                }
                return result;
            }
        }
    } else if constexpr (std::is_same_v<T, mp_int_t>) {
        if constexpr (std::is_signed_v<T> && (N < 0)) {
            return -1; // N Negativo es un error en el argumento
        } else {
            T result{factorial_table[2]};
            for (T i = 3; i <= N; ++i) {
                result *= i;
            }
            return result;
        }
    } else {
        return 0; // Tipo no soportado
    }
}

namespace detail {
    using std::index_sequence;
    using std::make_index_sequence;

    // Helper para generar casos del switch usando fold expressions
    template<typename T, size_t... Ns>
    constexpr T factorial_switch_impl(T n, index_sequence<Ns...>) {
        T result = 0;
        // Usa fold expression para crear todos los casos del switch
        (
            (
                n == static_cast<T>(Ns) ? (result = factorial_ct<T, static_cast<T>(Ns)>(), true) : false
            ) || ...
        );
        return result;
    }

    // Wrapper del switch que genera automáticamente todos los casos
    template<typename T, size_t MaxN>
    constexpr T factorial_switch(T n) {
        return factorial_switch_impl<T>(n, make_index_sequence<MaxN + 1>{});
    }
} // namespace detail

/**
* @brief Calcula el factorial de un número en tiempo de ejecución o compilación.
* @details Utiliza una tabla de búsqueda generada automáticamente para devolver valores precalculados.
* @return El factorial de n. Devuelve -1 para n negativo (tipos con signo) o 0 para overflow.
*/
template<IsFactorialSupportedInteger T>
constexpr T factorial(T n) {
    using detail::get_factorial_limit;
    using detail::factorial_switch;
    using std::is_signed_v;

    // Validación para tipos con signo
    if constexpr (is_signed_v<T>) {
        if (n < 0) {
            return -1; // Error para n negativo
        }
    }
    // Validación de overflow
    constexpr size_t limit = get_factorial_limit<T>();
    if (n > static_cast<T>(limit)) {
        return 0; // Overflow
    }
    // Usa el wrapper del switch para generar todos los casos automáticamente
    return factorial_switch<T, limit>(n);
} // end function factorial (template T)
