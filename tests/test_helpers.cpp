#include <test_helpers.hpp>
#include <int128_helpers.hpp>
#include <iostream>
#include <sstream>
#include <cassert>

// Hacemos los literales visibles para este archivo de pruebas
using namespace int128_literals;

// --- Definiciones de las Funciones de Test ---

void test_to_string_uint128() {
    std::cout << "Running test_to_string_uint128...\n";
    assert(to_string(static_cast<uint128_t>(0)) == "0");
    assert(to_string(static_cast<uint128_t>(123)) == "123");
    assert(to_string(static_cast<uint128_t>(9876543210ULL)) == "9876543210");
    
    uint128_t max_val = -1;
    assert(to_string(max_val) == "340282366920938463463374607431768211455");
    std::cout << "test_to_string_uint128 passed.\n";
}

void test_from_string_uint128() {
    std::cout << "Running test_from_string_uint128...\n";
    assert(parse_uint128("0") == static_cast<uint128_t>(0));
    assert(parse_uint128("123") == static_cast<uint128_t>(123));
    assert(parse_uint128("9876543210") == static_cast<uint128_t>(9876543210ULL));

    uint128_t max_val = -1;
    assert(parse_uint128("340282366920938463463374607431768211455") == max_val);

    // Test de errores
    bool caught_invalid_arg = false;
    try {
        parse_uint128("");
    } catch (const std::invalid_argument&) {
        caught_invalid_arg = true;
    }
    assert(caught_invalid_arg);

    caught_invalid_arg = false;
    try {
        parse_uint128("123A");
    } catch (const std::invalid_argument&) {
        caught_invalid_arg = true;
    }
    assert(caught_invalid_arg);

    bool caught_out_of_range = false;
    try {
        parse_uint128("340282366920938463463374607431768211456");
    } catch (const std::out_of_range&) {
        caught_out_of_range = true;
    }
    assert(caught_out_of_range);
    std::cout << "test_from_string_uint128 passed.\n";
}

void test_ostream_uint128() {
    std::cout << "Running test_ostream_uint128...\n";
    std::stringstream ss;
    uint128_t val0 = 0;
    ss << val0;
    assert(ss.str() == "0");
    ss.str("");

    uint128_t val1 = 12345;
    ss << val1;
    assert(ss.str() == "12345");
    ss.str("");

    uint128_t max_val = -1;
    ss << max_val;
    assert(ss.str() == "340282366920938463463374607431768211455");
    std::cout << "test_ostream_uint128 passed.\n";
}

void test_istream_uint128() {
    std::cout << "Running test_istream_uint128...\n";
    std::stringstream ss;
    uint128_t num;

    ss.str("12345");
    ss >> num;
    assert(ss);
    assert(num == static_cast<uint128_t>(12345));

    ss.clear();
    ss.str("123A45");
    ss >> num;
    assert(ss.fail());

    ss.clear();
    ss.str("340282366920938463463374607431768211456");
    ss >> num;
    assert(ss.fail());

    ss.clear();
    ss.str("");
    ss >> num;
    assert(ss.fail());
    std::cout << "test_istream_uint128 passed.\n";
}

void test_user_literals() {
    std::cout << "Running test_user_literals...\n";

    // Test para _u128 con literales numéricos
    assert(0_u128 == static_cast<uint128_t>(0));
    assert(12345_u128 == static_cast<uint128_t>(12345));

    // Test para _u128 con literales de cadena
    uint128_t max_u_val = -1;
    assert("340282366920938463463374607431768211455"_u128 == max_u_val);

    // Test para _i128 con literales numéricos
    assert(0_i128 == static_cast<int128_t>(0));
    assert(12345_i128 == static_cast<int128_t>(12345));
    assert(-(12345_i128) == static_cast<int128_t>(-12345)); // El '-' es un operador unario
    
    // Test para _i128 con literales de cadena
    int128_t max_i_val = (static_cast<uint128_t>(1) << 127) - 1;
    assert("170141183460469231731687303715884105727"_i128 == max_i_val);

    // El valor mínimo de int128_t se prueba con una cadena para evitar warnings de overflow
    int128_t min_i_val = "-170141183460469231731687303715884105728"_i128;
    assert(to_string(min_i_val) == "-170141183460469231731687303715884105728");

    std::cout << "test_user_literals passed.\n";
}

void run_tests() {
    std::cout << "Starting all tests...\n";
    test_to_string_uint128();
    test_from_string_uint128();
    test_ostream_uint128();
    test_istream_uint128();
    test_user_literals(); // Nueva prueba añadida
    std::cout << "All tests passed successfully!\n\n";
}
