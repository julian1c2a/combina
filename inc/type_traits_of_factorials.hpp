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
    constexpr std::array<T, N> make_constexpr_array_impl(
        Func &&f, index_sequence<Is...>) {
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

    enum class fact_sign_info { negative, other };

    enum class fact_over_info { overflow, other };

    enum class fact_type_info { signed_int, unsigned_int };

    enum class fact_capacity_info { finite, infty };

    enum class fact_sz_in_table { in, out };

    static constexpr size_t max_sz_fact_table{get_factorial_limit<uint1024_t>() + 1};

    template<IsFactorialSupportedInteger T, T k>
    constexpr fact_sign_info check_negative() {
        if constexpr ((is_signed_v<T>) && (k < 0))
            return fact_sign_info::negative;
        else return fact_sign_info::other;
    }

    template<IsFactorialSupportedInteger T>
    constexpr fact_sign_info check_negative_rt(T k) {
        if ((is_signed_v<T>) && (k < 0))
            return fact_sign_info::negative;
        else return fact_sign_info::other;
    }

    template<IsFactorialSupportedInteger T, T k>
    constexpr fact_over_info check_overflow() {
        static constexpr size_t limit{get_factorial_limit<T>()};
        if constexpr (limit != 0) {
            if constexpr (k > limit) {
                return fact_over_info::overflow;
            } else {
                return fact_over_info::other;
            }
        } else {
            return fact_over_info::other;
        }
    }

    template<IsFactorialSupportedInteger T>
    constexpr fact_over_info check_overflow_rt(T k) {
        static constexpr size_t limit{get_factorial_limit<T>()};
        if constexpr (limit != 0) {
            if (k > limit) {
                return fact_over_info::overflow;
            } else {
                return fact_over_info::other;
            }
        } else {
            return fact_over_info::other;
        }
    }

    template<IsFactorialSupportedInteger T, T k>
    constexpr fact_sz_in_table check_sz_in_table() {
        if constexpr (k <= max_sz_fact_table) {
            return fact_sz_in_table::in;
        } else {
            return fact_sz_in_table::out;
        }
    }

    template<IsFactorialSupportedInteger T>
    constexpr fact_sz_in_table check_sz_in_table_rt(T k) {
        if (k <= max_sz_fact_table) {
            return fact_sz_in_table::in;
        } else {
            return fact_sz_in_table::out;
        }
    }

    template<IsFactorialSupportedInteger T>
    constexpr fact_type_info sign_type_info() {
        if constexpr (is_signed_v<T>) {
            return fact_type_info::signed_int;
        } else {
            return fact_type_info::unsigned_int;
        }
    }

    template<IsFactorialSupportedInteger T>
    constexpr fact_capacity_info capacity_type_info() {
        if constexpr (IsInftyInteger<T>) {
            return fact_capacity_info::infty;
        } else {
            return fact_capacity_info::finite;
        }
    }

    template<IsFactorialSupportedInteger T, T k>
    constexpr T factorial_ct() {
        using std::numeric_limits;

        static constexpr size_t limit{get_factorial_limit<T>()};

        static constexpr bool is_negative{
            check_negative<T, k>() == fact_sign_info::negative
        };

        static constexpr bool is_overflow{
            check_overflow<T, k>() == fact_over_info::overflow
        };

        if constexpr (is_negative) {
            // Si el tipo entero es signado
            // y k es negativo
            return static_cast<T>(-1); // Devuelve -1 para números negativos
        } else if constexpr (is_overflow) {
            // Si el tipo es de longitud finita
            // y k es mayor que el límite
            return static_cast<T>(0); // Devuelve 0 para overflow
        } else {
            T res = static_cast<T>(1);
            for (size_t i{2}; i <= k; ++i) {
                // Comprobación de overflow antes de multiplicar
                if constexpr (limit != 0) {
                    const T division_check{
                        numeric_limits<T>::max() / static_cast<T>(i)
                    };
                    if (res > division_check) {
                        return 0; // Overflow en la siguiente multiplicación
                        // Tipos de longitud finita
                    } else {
                        res *= static_cast<T>(i);
                    }
                }
            }
            return res;
        }
    }

    template<IsFactorialSupportedInteger T>
    constexpr T factorial_rt(const T k) {
        using std::numeric_limits;

        static constexpr bool is_signed_integer{
            sign_type_info<T>() == fact_type_info::signed_int
        };

        const bool is_negative{
            check_negative_rt<T>(k) == fact_sign_info::negative
        };

        const bool is_overflow{
            check_overflow_rt<T>(k) == fact_over_info::overflow
        };

        static constexpr bool is_finite_integer{
            capacity_type_info<T>() == fact_capacity_info::finite
        };


        if constexpr (is_signed_integer) {
            // TIPOS SIGNADOS
            if (k < 0) {
                return static_cast<T>(-1); // ARGUMENTO NEGATIVO
            } else if constexpr (is_finite_integer) {
                // TIPOS DE LONGITUD FINITA
                if (is_overflow) {
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
            if constexpr (is_finite_integer) {
                // TIPOS NO SIGNADOS DE LONGITUD FINITA
                if (is_overflow) {
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
        return make_constexpr_array<T, Size>([](const size_t index) -> T {
            if (index == 0) {
                // Posición 0: -1 para tipos con signo, 0 para sin signo (ya que no pueden ser negativos).
                if constexpr (is_signed_v<T>) {
                    return static_cast<T>(-1);
                } else {
                    return static_cast<T>(0);
                }
            } else if (index == 1) {
                // Posición 1: 0 para overflow.
                return static_cast<T>(0);
            } else {
                // Posición n: (n-2)!
                return factorial_rt<T>(static_cast<T>(index - 2));
            }
        });
    }

    // Función auxiliar para obtener el factorial de una tabla estática
    template<IsFactorialSupportedInteger T>
    constexpr T factorial_from_table(T n) {
        // 1. Manejar el caso de error: número negativo (solo para tipos con signo).
        if constexpr (is_signed_v<T>) {
            if (n < 0) {
                return static_cast<T>(-1);
            }
        }

        // 2. Comprobar si el valor está dentro de los límites de la tabla precalculada (0 a 170).
        constexpr size_t table_limit = get_factorial_limit<uint1024_t>(); // Es 170
        if (n <= table_limit) {
            // El valor está en el rango de la tabla, la usamos.
            constexpr size_t sz_table = table_limit + 1 + 2; // +2 para casos especiales
            static constexpr std::array<T, sz_table> table{
                generate_extended_factorial_table<T, sz_table>()
            };

            // Comprobar si 'n' excede el límite del *tipo de dato* actual.
            constexpr size_t type_limit = get_factorial_limit<T>();
            if constexpr (type_limit != 0) {
                // Si el tipo tiene un límite
                if (n > type_limit) {
                    return table[1]; // Devolver el valor de overflow precalculado.
                }
            }
            
            // El valor es válido y está en la tabla.
            return table[static_cast<size_t>(n) + 2]; // n! está en la posición n+2
        } else {
            // 3. Si n > 170, la tabla no es suficiente. Recurrir al cálculo en tiempo de ejecución.
            //    Esto es principalmente para tipos de precisión arbitraria como mp_int_t.
            return factorial_rt<T>(n);
        }
    } // end function factorial_from_table
} // namespace detail


#endif //COMBINATORIA_FACTORIALES_TYPE_TRAITS_HPP
