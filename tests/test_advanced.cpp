#include <test_advanced.hpp>
#include <factoriales.hpp>
#include <iostream>
#include <cassert>
#include <vector>
#include <chrono>

// Hacemos los literales visibles para este archivo de pruebas
using namespace integer_literals;

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

    // 4. Entradas Inválidas (ahora deben devolver -1 para tipos con signo)
    assert(factorial(static_cast<int64_t>(-1)) == -1);
    assert(factorial(static_cast<int128_t>(-10)) == -1);

    // 5. Pruebas con static_assert para verificar el cálculo en tiempo de compilación
    static_assert(factorial(10_u64) == 3628800, "Compile-time check failed");
    static_assert(factorial_ct<uint64_t, 12>() == 479001600, "Compile-time check for 12! failed");
    // La comprobación de -1 en factorial_ct no es una expresión constante pura para el compilador,
    // por lo que usamos un assert en lugar de static_assert.
    assert((factorial_ct<int64_t, -1>() == -1));

    std::cout << "test_factorial_properties passed.\n";
}

void test_factorial_known_values() {
    std::cout << "Running test_factorial_known_values...\n";
    const std::vector<uint128_t> known_factorials = {
        1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, // 0! ... 11!
        479001600, 6227020800, 87178291200, 1307674368000, // 12! ... 15!
        20922789888000, 355687428096000, 6402373705728000, 121645100408832000, // 16! ... 19!
        "2432902008176640000"_u128, "51090942171709440000"_u128, // 20! ... 21!
        "1124000727777607680000"_u128, "25852016738884976640000"_u128, // 22! ... 23!
        "620448401733239439360000"_u128, "15511210043330985984000000"_u128, // 24! ... 25!
        "403291461126605635584000000"_u128, "10888869450418352160768000000"_u128, //26! ... 27!
        "304888344611713860501504000000"_u128, "8841761993739701954543616000000"_u128, // 28! ... 29!
        "265252859812191058636308480000000"_u128, "8222838654177922817725562880000000"_u128, // 30! ... 31!
        "263130836933693530167218012160000000"_u128, // 32!
        "8683317618811886495518194401280000000"_u128, // 33!
        "295232799039604140847618609643520000000"_u128 // 34!
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
