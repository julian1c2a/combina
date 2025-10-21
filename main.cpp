#include <int128_helpers.hpp>
#include <sstream> // Para std::stringstream
#include <cassert> // Para assert

// --- Funciones de Test ---

void test_to_string_uint128() {
    std::cout << "Running test_to_string_uint128...\n";
    assert(to_string(static_cast<uint128_t>(0)) == "0");
    assert(to_string(static_cast<uint128_t>(123)) == "123");
    assert(to_string(static_cast<uint128_t>(9876543210ULL)) == "9876543210");
    
    uint128_t max_val = -1; // Esto asigna el valor máximo para uint128_t
    assert(to_string(max_val) == "340282366920938463463374607431768211455");
    std::cout << "test_to_string_uint128 passed.\n";
}

void test_from_string_uint128() {
    std::cout << "Running test_from_string_uint128...\n";
    assert(from_string("0") == static_cast<uint128_t>(0));
    assert(from_string("123") == static_cast<uint128_t>(123));
    assert(from_string("9876543210") == static_cast<uint128_t>(9876543210ULL));

    uint128_t max_val = -1;
    assert(from_string("340282366920938463463374607431768211455") == max_val);

    // Test de errores
    bool caught_invalid_arg = false;
    try {
        from_string("");
    } catch (const std::invalid_argument&) {
        caught_invalid_arg = true;
    }
    assert(caught_invalid_arg);

    caught_invalid_arg = false;
    try {
        from_string("123A");
    } catch (const std::invalid_argument&) {
        caught_invalid_arg = true;
    }
    assert(caught_invalid_arg);

    bool caught_out_of_range = false;
    try {
        // Un número mayor que el máximo de uint128_t
        from_string("340282366920938463463374607431768211456"); // max_val + 1
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
    ss.str(""); // Limpiar el stringstream

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

    // Test válido
    ss.str("12345");
    ss >> num;
    assert(ss);
    assert(num == static_cast<uint128_t>(12345));

    // Test con entrada inválida (caracteres no numéricos)
    ss.clear(); // Limpiar flags de error
    ss.str("123A45");
    ss >> num;
    assert(ss.fail());

    // Test con número fuera de rango
    ss.clear();
    ss.str("340282366920938463463374607431768211456"); // max_val + 1
    ss >> num;
    assert(ss.fail());

    // Test con string vacío (se comporta como un error de lectura)
    ss.clear();
    ss.str("");
    ss >> num;
    assert(ss.fail());
    std::cout << "test_istream_uint128 passed.\n";
}

void run_tests() {
    std::cout << "Starting all tests...\n";
    test_to_string_uint128();
    test_from_string_uint128();
    test_ostream_uint128();
    test_istream_uint128();
    std::cout << "All tests passed successfully!\n\n";
}

int main() {
    run_tests(); // Ejecutar las pruebas primero

    std::cout << "Introduce un numero grande (de hasta 128 bits):" << std::endl;
    uint128_t my_number;
    std::cin >> my_number;

    if (std::cin.good()) {
        std::cout << "Has introducido: " << my_number << std::endl;
        uint128_t max_val = -1;
        std::cout << "El valor maximo para uint128_t es: " << max_val << std::endl;
        std::cout << "El numero por dos es: " << (my_number * 2) << std::endl;
    } else {
        std::cout << "Entrada invalida." << std::endl;
    }

    return 0;
}