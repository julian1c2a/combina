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
        Is512BitInteger<T> || Is1024BitInteger<T> || std::is_same_v<T, mp_int_t>;


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
        } else if constexpr (Is1024BitInteger<T>) {
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
} // namespace detail

/**
 * @brief Obtiene un factorial precalculado en tiempo de compilación.
 * @return El factorial de N. Devuelve -1 para N negativo (tipos con signo) o 0 para overflow.
 */
template<IsFactorialSupportedInteger T, T N>
constexpr T factorial_ct() {
    static constexpr std::size_t factorial_limit{detail::get_factorial_limit<T>()};

    static constexpr std::array<T, 35> factorial_table{detail::generate_factorial_table<T>()};

    if constexpr (std::is_signed_v<T> && (N < 0)) {
        return -1;
    } else if constexpr (N > factorial_limit) {
        return 0; // Overflow
    } else if constexpr (Is8BitInteger<T> ||
                         Is16BitInteger<T> ||
                         Is32BitInteger<T> ||
                         Is64BitInteger<T> ||
                         Is128BitInteger<T>) {
        return static_cast<T>(factorial_table[static_cast<size_t>(N)]);
    } else if constexpr (Is256BitInteger<T> ||
                         Is512BitInteger<T> ||
                         Is1024BitInteger<T>) {
        if constexpr (std::is_signed_v<T> && (N < 0)) {
            return -1; // N Negativo es un error en el argumento
        } else if constexpr (N > detail::get_factorial_limit<T>()) {
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

/**
 * @brief Calcula el factorial de un número en tiempo de ejecución o compilación.
 * @details Utiliza una tabla de búsqueda (switch) para devolver valores precalculados.
 * @return El factorial de n. Devuelve -1 para n negativo (tipos con signo) o 0 para overflow.
 */
template<IsFactorialSupportedInteger T>
constexpr T factorial(T n) {
    if constexpr (std::is_signed_v<T>) {
        if (n < 0)
            return -1; // Error para n negativo
    } else if (n > detail::get_factorial_limit<T>()) {
        return 0; // Overflow
    } else {
        if constexpr (std::is_signed_v<T>) {
            if (n < 0) return -1;
            else if (n > detail::get_factorial_limit<T>()) return 0;
            else {
                switch (n) {
                    case 0: return factorial_ct<T, 0>();
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
                    case 35: return factorial_ct<T, 35>();
                    case 36: return factorial_ct<T, 36>();
                    case 37: return factorial_ct<T, 37>();
                    case 38: return factorial_ct<T, 38>();
                    case 39: return factorial_ct<T, 39>();
                    case 40: return factorial_ct<T, 40>();
                    case 41: return factorial_ct<T, 41>();
                    case 42: return factorial_ct<T, 42>();
                    case 43: return factorial_ct<T, 43>();
                    case 44: return factorial_ct<T, 44>();
                    case 45: return factorial_ct<T, 45>();
                    case 46: return factorial_ct<T, 46>();
                    case 47: return factorial_ct<T, 47>();
                    case 48: return factorial_ct<T, 48>();
                    case 49: return factorial_ct<T, 49>();
                    case 50: return factorial_ct<T, 50>();
                    case 51: return factorial_ct<T, 51>();
                    case 52: return factorial_ct<T, 52>();
                    case 53: return factorial_ct<T, 53>();
                    case 54: return factorial_ct<T, 54>();
                    case 55: return factorial_ct<T, 55>();
                    case 56: return factorial_ct<T, 56>();
                    case 57: return factorial_ct<T, 57>();
                    case 58: return factorial_ct<T, 58>();
                    case 59: return factorial_ct<T, 59>();
                    case 60: return factorial_ct<T, 60>();
                    case 61: return factorial_ct<T, 61>();
                    case 62: return factorial_ct<T, 62>();
                    case 63: return factorial_ct<T, 63>();
                    case 64: return factorial_ct<T, 64>();
                    case 65: return factorial_ct<T, 65>();
                    case 66: return factorial_ct<T, 66>();
                    case 67: return factorial_ct<T, 67>();
                    case 68: return factorial_ct<T, 68>();
                    case 69: return factorial_ct<T, 69>();
                    case 70: return factorial_ct<T, 70>();
                    case 71: return factorial_ct<T, 71>();
                    case 72: return factorial_ct<T, 72>();
                    case 73: return factorial_ct<T, 73>();
                    case 74: return factorial_ct<T, 74>();
                    case 75: return factorial_ct<T, 75>();
                    case 76: return factorial_ct<T, 76>();
                    case 77: return factorial_ct<T, 77>();
                    case 78: return factorial_ct<T, 78>();
                    case 79: return factorial_ct<T, 79>();
                    case 80: return factorial_ct<T, 80>();
                    case 81: return factorial_ct<T, 81>();
                    case 82: return factorial_ct<T, 82>();
                    case 83: return factorial_ct<T, 83>();
                    case 84: return factorial_ct<T, 84>();
                    case 85: return factorial_ct<T, 85>();
                    case 86: return factorial_ct<T, 86>();
                    case 87: return factorial_ct<T, 87>();
                    case 88: return factorial_ct<T, 88>();
                    case 89: return factorial_ct<T, 89>();
                    case 90: return factorial_ct<T, 90>();
                    case 91: return factorial_ct<T, 91>();
                    case 92: return factorial_ct<T, 92>();
                    case 93: return factorial_ct<T, 93>();
                    case 94: return factorial_ct<T, 94>();
                    case 95: return factorial_ct<T, 95>();
                    case 96: return factorial_ct<T, 96>();
                    case 97: return factorial_ct<T, 97>();
                    case 98: return factorial_ct<T, 98>();
                    case 99: return factorial_ct<T, 99>();
                    case 100: return factorial_ct<T, 100>();
                    case 101: return factorial_ct<T, 101>();
                    case 102: return factorial_ct<T, 102>();
                    case 103: return factorial_ct<T, 103>();
                    case 104: return factorial_ct<T, 104>();
                    case 105: return factorial_ct<T, 105>();
                    case 106: return factorial_ct<T, 106>();
                    case 107: return factorial_ct<T, 107>();
                    case 108: return factorial_ct<T, 108>();
                    case 109: return factorial_ct<T, 109>();
                    case 110: return factorial_ct<T, 110>();
                    case 111: return factorial_ct<T, 111>();
                    case 112: return factorial_ct<T, 112>();
                    case 113: return factorial_ct<T, 113>();
                    case 114: return factorial_ct<T, 114>();
                    case 115: return factorial_ct<T, 115>();
                    case 116: return factorial_ct<T, 116>();
                    case 117: return factorial_ct<T, 117>();
                    case 118: return factorial_ct<T, 118>();
                    case 119: return factorial_ct<T, 119>();
                    case 120: return factorial_ct<T, 120>();
                    case 121: return factorial_ct<T, 121>();
                    case 122: return factorial_ct<T, 122>();
                    case 123: return factorial_ct<T, 123>();
                    case 124: return factorial_ct<T, 124>();
                    case 125: return factorial_ct<T, 125>();
                    case 126: return factorial_ct<T, 126>();
                    case 127: return factorial_ct<T, 127>();
                    case 128: return factorial_ct<T, 128>();
                    case 129: return factorial_ct<T, 129>();
                    case 130: return factorial_ct<T, 130>();
                    case 131: return factorial_ct<T, 131>();
                    case 132: return factorial_ct<T, 132>();
                    case 133: return factorial_ct<T, 133>();
                    case 134: return factorial_ct<T, 134>();
                    case 135: return factorial_ct<T, 135>();
                    case 136: return factorial_ct<T, 136>();
                    case 137: return factorial_ct<T, 137>();
                    case 138: return factorial_ct<T, 138>();
                    case 139: return factorial_ct<T, 139>();
                    case 140: return factorial_ct<T, 140>();
                    case 141: return factorial_ct<T, 141>();
                    case 142: return factorial_ct<T, 142>();
                    case 143: return factorial_ct<T, 143>();
                    case 144: return factorial_ct<T, 144>();
                    case 145: return factorial_ct<T, 145>();
                    case 146: return factorial_ct<T, 146>();
                    case 147: return factorial_ct<T, 147>();
                    case 148: return factorial_ct<T, 148>();
                    case 149: return factorial_ct<T, 149>();
                    case 150: return factorial_ct<T, 150>();
                    case 151: return factorial_ct<T, 151>();
                    case 152: return factorial_ct<T, 152>();
                    case 153: return factorial_ct<T, 153>();
                    case 154: return factorial_ct<T, 154>();
                    case 155: return factorial_ct<T, 155>();
                    case 156: return factorial_ct<T, 156>();
                    case 157: return factorial_ct<T, 157>();
                    case 158: return factorial_ct<T, 158>();
                    case 159: return factorial_ct<T, 159>();
                    case 160: return factorial_ct<T, 160>();
                    case 161: return factorial_ct<T, 161>();
                    case 162: return factorial_ct<T, 162>();
                    case 163: return factorial_ct<T, 163>();
                    case 164: return factorial_ct<T, 164>();
                    case 165: return factorial_ct<T, 165>();
                    case 166: return factorial_ct<T, 166>();
                    case 167: return factorial_ct<T, 167>();
                    case 168: return factorial_ct<T, 168>();
                    case 169: return factorial_ct<T, 169>();
                    case 170: return factorial_ct<T, 170>();
                    default: return 0; // Overflow
                }
            }
        } else {
            // unsigned
            if (n > detail::get_factorial_limit<T>()) {
                return 0; // Overflow
            } else {
                switch (n) {
                    case 0: return factorial_ct<T, 0>();
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
                    case 35: return factorial_ct<T, 35>();
                    case 36: return factorial_ct<T, 36>();
                    case 37: return factorial_ct<T, 37>();
                    case 38: return factorial_ct<T, 38>();
                    case 39: return factorial_ct<T, 39>();
                    case 40: return factorial_ct<T, 40>();
                    case 41: return factorial_ct<T, 41>();
                    case 42: return factorial_ct<T, 42>();
                    case 43: return factorial_ct<T, 43>();
                    case 44: return factorial_ct<T, 44>();
                    case 45: return factorial_ct<T, 45>();
                    case 46: return factorial_ct<T, 46>();
                    case 47: return factorial_ct<T, 47>();
                    case 48: return factorial_ct<T, 48>();
                    case 49: return factorial_ct<T, 49>();
                    case 50: return factorial_ct<T, 50>();
                    case 51: return factorial_ct<T, 51>();
                    case 52: return factorial_ct<T, 52>();
                    case 53: return factorial_ct<T, 53>();
                    case 54: return factorial_ct<T, 54>();
                    case 55: return factorial_ct<T, 55>();
                    case 56: return factorial_ct<T, 56>();
                    case 57: return factorial_ct<T, 57>();
                    case 58: return factorial_ct<T, 58>();
                    case 59: return factorial_ct<T, 59>();
                    case 60: return factorial_ct<T, 60>();
                    case 61: return factorial_ct<T, 61>();
                    case 62: return factorial_ct<T, 62>();
                    case 63: return factorial_ct<T, 63>();
                    case 64: return factorial_ct<T, 64>();
                    case 65: return factorial_ct<T, 65>();
                    case 66: return factorial_ct<T, 66>();
                    case 67: return factorial_ct<T, 67>();
                    case 68: return factorial_ct<T, 68>();
                    case 69: return factorial_ct<T, 69>();
                    case 70: return factorial_ct<T, 70>();
                    case 71: return factorial_ct<T, 71>();
                    case 72: return factorial_ct<T, 72>();
                    case 73: return factorial_ct<T, 73>();
                    case 74: return factorial_ct<T, 74>();
                    case 75: return factorial_ct<T, 75>();
                    case 76: return factorial_ct<T, 76>();
                    case 77: return factorial_ct<T, 77>();
                    case 78: return factorial_ct<T, 78>();
                    case 79: return factorial_ct<T, 79>();
                    case 80: return factorial_ct<T, 80>();
                    case 81: return factorial_ct<T, 81>();
                    case 82: return factorial_ct<T, 82>();
                    case 83: return factorial_ct<T, 83>();
                    case 84: return factorial_ct<T, 84>();
                    case 85: return factorial_ct<T, 85>();
                    case 86: return factorial_ct<T, 86>();
                    case 87: return factorial_ct<T, 87>();
                    case 88: return factorial_ct<T, 88>();
                    case 89: return factorial_ct<T, 89>();
                    case 90: return factorial_ct<T, 90>();
                    case 91: return factorial_ct<T, 91>();
                    case 92: return factorial_ct<T, 92>();
                    case 93: return factorial_ct<T, 93>();
                    case 94: return factorial_ct<T, 94>();
                    case 95: return factorial_ct<T, 95>();
                    case 96: return factorial_ct<T, 96>();
                    case 97: return factorial_ct<T, 97>();
                    case 98: return factorial_ct<T, 98>();
                    case 99: return factorial_ct<T, 99>();
                    case 100: return factorial_ct<T, 100>();
                    case 101: return factorial_ct<T, 101>();
                    case 102: return factorial_ct<T, 102>();
                    case 103: return factorial_ct<T, 103>();
                    case 104: return factorial_ct<T, 104>();
                    case 105: return factorial_ct<T, 105>();
                    case 106: return factorial_ct<T, 106>();
                    case 107: return factorial_ct<T, 107>();
                    case 108: return factorial_ct<T, 108>();
                    case 109: return factorial_ct<T, 109>();
                    case 110: return factorial_ct<T, 110>();
                    case 111: return factorial_ct<T, 111>();
                    case 112: return factorial_ct<T, 112>();
                    case 113: return factorial_ct<T, 113>();
                    case 114: return factorial_ct<T, 114>();
                    case 115: return factorial_ct<T, 115>();
                    case 116: return factorial_ct<T, 116>();
                    case 117: return factorial_ct<T, 117>();
                    case 118: return factorial_ct<T, 118>();
                    case 119: return factorial_ct<T, 119>();
                    case 120: return factorial_ct<T, 120>();
                    case 121: return factorial_ct<T, 121>();
                    case 122: return factorial_ct<T, 122>();
                    case 123: return factorial_ct<T, 123>();
                    case 124: return factorial_ct<T, 124>();
                    case 125: return factorial_ct<T, 125>();
                    case 126: return factorial_ct<T, 126>();
                    case 127: return factorial_ct<T, 127>();
                    case 128: return factorial_ct<T, 128>();
                    case 129: return factorial_ct<T, 129>();
                    case 130: return factorial_ct<T, 130>();
                    case 131: return factorial_ct<T, 131>();
                    case 132: return factorial_ct<T, 132>();
                    case 133: return factorial_ct<T, 133>();
                    case 134: return factorial_ct<T, 134>();
                    case 135: return factorial_ct<T, 135>();
                    case 136: return factorial_ct<T, 136>();
                    case 137: return factorial_ct<T, 137>();
                    case 138: return factorial_ct<T, 138>();
                    case 139: return factorial_ct<T, 139>();
                    case 140: return factorial_ct<T, 140>();
                    case 141: return factorial_ct<T, 141>();
                    case 142: return factorial_ct<T, 142>();
                    case 143: return factorial_ct<T, 143>();
                    case 144: return factorial_ct<T, 144>();
                    case 145: return factorial_ct<T, 145>();
                    case 146: return factorial_ct<T, 146>();
                    case 147: return factorial_ct<T, 147>();
                    case 148: return factorial_ct<T, 148>();
                    case 149: return factorial_ct<T, 149>();
                    case 150: return factorial_ct<T, 150>();
                    case 151: return factorial_ct<T, 151>();
                    case 152: return factorial_ct<T, 152>();
                    case 153: return factorial_ct<T, 153>();
                    case 154: return factorial_ct<T, 154>();
                    case 155: return factorial_ct<T, 155>();
                    case 156: return factorial_ct<T, 156>();
                    case 157: return factorial_ct<T, 157>();
                    case 158: return factorial_ct<T, 158>();
                    case 159: return factorial_ct<T, 159>();
                    case 160: return factorial_ct<T, 160>();
                    case 161: return factorial_ct<T, 161>();
                    case 162: return factorial_ct<T, 162>();
                    case 163: return factorial_ct<T, 163>();
                    case 164: return factorial_ct<T, 164>();
                    case 165: return factorial_ct<T, 165>();
                    case 166: return factorial_ct<T, 166>();
                    case 167: return factorial_ct<T, 167>();
                    case 168: return factorial_ct<T, 168>();
                    case 169: return factorial_ct<T, 169>();
                    case 170: return factorial_ct<T, 170>();
                    default: return 0; // Overflow
                }
            }
        }
    }
} // end function
