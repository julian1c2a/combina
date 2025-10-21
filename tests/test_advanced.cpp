#include <test_advanced.hpp>
#include <factoriales.hpp>
#include <iostream>
#include <cassert>
#include <vector>
#include <chrono>

// Hacemos los literales visibles para este archivo de pruebas
using namespace int128_literals;

void test_factorial_properties() {
    std::cout << "Running test_factorial_properties...\n";

    // 1. Casos Base
    assert(factorial(0_u64) == 1_u64);
    assert(factorial(1_u64) == 1_u64);
    assert(factorial(0_i128) == 1_i128);
    assert(factorial(1_i128) == 1_i128);

    // 2. Propiedad Recursiva
    assert(factorial(7_u64) == 7 * factorial(6_u64));
    assert(factorial(15_i128) == 15 * factorial(14_i128));

    // 3. Límites de Desbordamiento
    assert(factorial(20_u64) != 0 && factorial(21_u64) == 0);
    assert(factorial(20_i64) != 0 && factorial(21_i64) == 0);
    assert(factorial(34_u128) != 0 && factorial(35_u128) == 0);
    assert(factorial(33_i128) != 0 && factorial(34_i128) == 0);

    // 4. Entradas Inválidas
    assert(factorial(-1_i64) == 0);
    assert(factorial(-10_i128) == 0);

    // 5. Pruebas con static_assert para verificar el cálculo en tiempo de compilación
    static_assert(factorial(10_u64) == 3628800, "Compile-time check failed");
    static_assert(factorial_ct<uint64_t, 12>() == 479001600, "Compile-time check failed");

    std::cout << "test_factorial_properties passed.\n";
}

void test_factorial_known_values() {
    std::cout << "Running test_factorial_known_values...\n";
    const std::vector<uint128_t> known_factorials = {
        1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800,
        479001600, 6227020800, 87178291200, 1307674368000, 20922789888000,
        355687428096000, 6402373705728000, 121645100408832000, 2432902008176640000_u128
    };

    for (size_t i = 0; i < known_factorials.size(); ++i) {
        assert(factorial(static_cast<uint128_t>(i)) == known_factorials[i]);
    }
    std::cout << "test_factorial_known_values passed.\n";
}

void demonstrate_performance_test() {
    std::cout << "\n--- Performance Test Demonstration ---\n";
    std::cout << "(Note: This is a demonstration. Run in Release mode for accurate results.)\n";

    const int num_iterations = 1000000;
    const int value_to_test = 20;
    std::vector<int> test_values(num_iterations, value_to_test);
    volatile uint64_t result = 0; // volatile para evitar que el optimizador elimine el bucle

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int val : test_values) {
        result = factorial(static_cast<uint64_t>(val));
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "Time to execute factorial(" << value_to_test << ") " << num_iterations << " times: "
              << duration.count() << " microseconds.\n";
    std::cout << "Average time per call: " << static_cast<double>(duration.count()) / num_iterations << " microseconds.\n";
    std::cout << "(Last computed result: " << result << ")\n"; // Para asegurar que 'result' se usa
    std::cout << "--- End of Performance Test ---\n";
}

void run_advanced_tests() {
    std::cout << "\nStarting advanced tests...\n";
    test_factorial_properties();
    test_factorial_known_values();
    demonstrate_performance_test();
    std::cout << "All advanced tests passed successfully!\n\n";
}
