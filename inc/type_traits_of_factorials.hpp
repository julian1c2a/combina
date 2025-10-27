//
// Created by julian on 25/10/2025.
//

#ifndef COMBINATORIA_FACTORIALES_TYPE_TRAITS_HPP
#define COMBINATORIA_FACTORIALES_TYPE_TRAITS_HPP

#include <boost/multiprecision/gmp.hpp>
#include <type_traits>
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
using std::is_signed_v;
using std::is_same_v;

// --- Concepts para la selección de tipos ---
template<typename T>
concept Is1024BitInteger = is_same_v<T, int1024_t> || is_same_v<T, uint1024_t>;

template<typename T>
concept Is512BitInteger = is_same_v<T, int512_t> || is_same_v<T, uint512_t>;

template<typename T>
concept Is256BitInteger = is_same_v<T, int256_t> || is_same_v<T, uint256_t>;

template<typename T>
concept Is128BitInteger = is_same_v<T, int128_t> || is_same_v<T, uint128_t>;

template<typename T>
concept Is64BitInteger = is_same_v<T, int64_t> || is_same_v<T, uint64_t>;

template<typename T>
concept Is32BitInteger = is_same_v<T, int32_t> || is_same_v<T, uint32_t>;

template<typename T>
concept Is16BitInteger = is_same_v<T, int16_t> || is_same_v<T, uint16_t>;

template<typename T>
concept Is8BitInteger = is_same_v<T, int8_t> || is_same_v<T, uint8_t>;

template<typename T>
concept IsInftyInteger = is_same_v<T, mp_int_t>;

template<typename T>
concept IsFactorialSupportedInteger =
        Is8BitInteger<T> || Is16BitInteger<T> || Is32BitInteger<T> ||
        Is64BitInteger<T> || Is128BitInteger<T> || Is256BitInteger<T> ||
        Is512BitInteger<T> || Is1024BitInteger<T> || IsInftyInteger<T>;

template<typename T>
concept IsIntegerTypeInTheFactorialTable =
        Is8BitInteger<T> || Is16BitInteger<T> || Is32BitInteger<T> ||
        Is64BitInteger<T> || Is128BitInteger<T>;

template<typename T>
concept IsIntegerTypeOutTheFactorialTable =
        Is256BitInteger<T> || Is512BitInteger<T> || Is1024BitInteger<T> ||
        IsInftyInteger<T>;

template<typename T>
concept Is8To128BitInteger =
        Is8BitInteger<T> || Is16BitInteger<T> || Is32BitInteger<T> ||
        Is64BitInteger<T> || Is128BitInteger<T>;

template<typename T>
concept Is256To1024BitInteger =
        Is256BitInteger<T> || Is512BitInteger<T> || Is1024BitInteger<T>;

namespace implementation_details {
    using std::size_t;
    using std::is_signed_v;
    using std::is_same_v;

    template<IsFactorialSupportedInteger T>
    constexpr auto get_factorial_limit() -> size_t {
        if constexpr (Is8BitInteger<T>) {
            return 5;
        } else if constexpr (Is16BitInteger<T>) {
            if constexpr (is_signed_v<T>) {
                return 7;
            } else {
                return 8;
            }
        } else if constexpr (Is32BitInteger<T>) {
            return 12;
        } else if constexpr (Is64BitInteger<T>) {
            return 20;
        } else if constexpr (Is128BitInteger<T>) {
            if constexpr (is_signed_v<T>) {
                return 33;
            } else {
                return 34;
            }
        } else if constexpr (Is256BitInteger<T>) {
            return 57;
        } else if constexpr (Is512BitInteger<T>) {
            if constexpr (is_signed_v<T>) {
                return 97;
            } else {
                return 98;
            }
        } else if constexpr (Is1024BitInteger<T>) {
            return 170;
        } else {
            return 0; // Solo para tipo entero de longitud arbitraria (mp_int_t)
        }
    }

    using std::index_sequence;
    using std::make_index_sequence;
    using std::forward;

    // Helper para generar arrays en tiempo de compilación.
    // --- Implementación interna ---
    // Recibe la secuencia de índices (0, 1, 2, ..., N-1) como un paquete de parámetros.
    template<typename T, size_t N, typename Func, size_t... Is>
    constexpr std::array<T, N> make_constexpr_array_impl(Func &&f, index_sequence<Is...>) {
        // Usa una expansión de paquete para llamar a f(0), f(1), f(2), ...
        // y usar esos valores para inicializar el array.
        return {f(Is)...};
    }

    /**
    * @brief Crea un std::array en tiempo de compilación.
    *
    * @tparam T El tipo de los elementos del array.
    * @tparam N El tamaño del array.
    * @tparam Func El tipo de la función o lambda generadora.
    * @param f La función que, dado un índice `size_t`, devuelve el valor para esa posición.
    * @return Un std::array<T, N> inicializado con los valores de f(0), f(1), ..., f(N-1).
    */
    template<typename T, size_t N, typename Func>
    constexpr std::array<T, N> make_constexpr_array(Func &&f) {
        // Crea una secuencia de índices de 0 a N-1 y la pasa a la implementación.
        return make_constexpr_array_impl<T, N>(
            forward<Func>(f),
            make_index_sequence<N>{}
        );
    }

    template<IsFactorialSupportedInteger T, T k>
    constexpr T factorial_ct() {
        using std::numeric_limits;
        constexpr size_t limit = get_factorial_limit<T>();
        if constexpr ((is_signed_v<T>) && (k < 0)) {
            // Si el tipo entero es signado
            // y k es negativo
            return static_cast<T>(-1); // Devuelve -1 para números negativos
        } else if constexpr ((limit != 0) && (k > limit)) {
            // Si el tipo es de longitud finita
            // y k es mayor que el límite
            return static_cast<T>(0); // Devuelve 0 para overflow
        } else {
            T res = static_cast<T>(1);
            for (size_t i{2}; i <= k; ++i) {
                // Comprobación de overflow antes de multiplicar
                if constexpr (limit != 0) {
                    if (res > numeric_limits<T>::max() / static_cast<T>(i)) {
                        return 0; // Overflow en la siguiente multiplicación
                        // Tipos de longitud finita
                    }
                }
                res *= static_cast<T>(i);
            }
            return res;
        }
    }

    template<IsFactorialSupportedInteger T>
    constexpr T factorial_rt(const T k) {
        using std::numeric_limits;
        static constexpr size_t limit = get_factorial_limit<T>();
        if constexpr (is_signed_v<T>) {
            // TIPOS SIGNADOS
            if (k < 0) {
                return static_cast<T>(-1); // ARGUMENTO NEGATIVO
            } else if constexpr (limit != 0) {
                // TIPOS DE LONGITUD FINITA
                if (k > limit) {
                    return static_cast<T>(0); // OVERFLOW TIPOS SIGNADOS FINITOS
                } else {
                    // TIPOS DE LONGITUD FINITA
                    T res{static_cast<T>(1)};
                    for (size_t i{2}; i <= k; ++i) {
                        // Comprobación de overflow antes de multiplicar
                        if (res > numeric_limits<T>::max() / static_cast<T>(i)) {
                            return 0; // Overflow
                        }
                        res *= static_cast<T>(i);
                    }
                    return res; // RETORNO NORMAL PARA TIPOS SIGNADOS FINITOS
                }
            } else {
                // TIPOS SIGNADOS DE LONGITUD ARBITRARIA
                T res{static_cast<T>(1)};
                for (size_t i{2}; i <= k; ++i) {
                    res *= static_cast<T>(i);
                }
                return res; // RETORNO NORMAL PARA TIPOS SIGNADOS DE LONGITUD ARBITRARIA
            }
        } else {
            // TIPOS NO SIGNADOS
            if constexpr (limit != 0) {
                // TIPOS NO SIGNADOS DE LONGITUD FINITA
                if (k > limit) {
                    return static_cast<T>(0); // OVERFLOW TIPOS NO SIGNADOS FINITOS
                } else {
                    // TIPOS NO SIGNADOS DE LONGITUD FINITA
                    T res{static_cast<T>(1)};
                    for (size_t i{2}; i <= k; ++i) {
                        // Comprobación de overflow antes de multiplicar
                        if (res > numeric_limits<T>::max() / static_cast<T>(i)) {
                            return 0; // Overflow
                        }
                        res *= static_cast<T>(i);
                    }
                    return res; // RETORNO NORMAL PARA TIPOS NO SIGNADOS FINITOS
                }
            } else {
                // TIPOS NO SIGNADOS DE LONGITUD ARBITRARIA
                T res{static_cast<T>(1)};
                for (size_t i{2}; i <= k; ++i) {
                    res *= static_cast<T>(i);
                }
                return res; // RETORNO NORMAL PARA TIPOS NO SIGNADOS DE LONGITUD ARBITRARIA
            }
        }
    }

    // Genera una tabla extendida de factoriales para tipos de cualquier tamaño en tiempo de compilación.
    template<IsFactorialSupportedInteger T, size_t Size>
    constexpr auto generate_extended_factorial_table() -> std::array<T, Size> {
        return make_constexpr_array<T, Size>([](size_t n) {
            return factorial_rt<T>(static_cast<T>(n));
        });
    }

    // Función auxiliar para obtener el factorial de una tabla estática
    template<IsFactorialSupportedInteger T>
    constexpr T factorial_from_table(T n) {
        constexpr size_t limit = get_factorial_limit<T>();

        if constexpr (is_signed_v<T>) {
            if (n < 0) {
                return static_cast<T>(-1); // Caso 1: Número negativo
            }
        } 
        
        if constexpr (limit != 0) { // Aplica solo a tipos de longitud finita
            if (static_cast<size_t>(n) > limit) {
                return static_cast<T>(0); // Caso 2: Desbordamiento
            }
        } 
        
        if constexpr (limit != 0 && limit <= 170) { // Caso 3: El número es válido y está en la tabla
            constexpr size_t sz_table = limit + 1;
            static constexpr std::array<T, sz_table> table{generate_extended_factorial_table<T, sz_table>()};
            return table[static_cast<size_t>(n)];
        } else { // Caso 4: Tipos de precisión arbitraria (o que exceden la tabla precalculada)
            return factorial_rt<T>(n);
        }
    } // end function factorial_from_table
} // namespace detail


#endif //COMBINATORIA_FACTORIALES_TYPE_TRAITS_HPP
