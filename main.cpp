#include <int128_helpers.hpp>
#include <factoriales.hpp>
#include <test_helpers.hpp>
#include <test_advanced.hpp> // Incluir la nueva cabecera de pruebas

// Hacemos los literales visibles en este archivo
using namespace integer_literals;

int main() {
    run_tests(); // Ejecutar las pruebas básicas primero
    run_advanced_tests(); // Ejecutar las nuevas pruebas avanzadas

    // std::cout << "Introduce un numero grande (de hasta 128 bits):" << std::endl;
    // uint128_t my_number;
    // std::cin >> my_number;
    //
    // if (std::cin.good()) {
    //     std::cout << "Has introducido: " << my_number << std::endl;
    //     uint128_t max_val = "340282366920938463463374607431768211455"_u128;
    //     std::cout << "El valor maximo para uint128_t es: " << max_val << std::endl;
    //     std::cout << "El numero por dos es: " << (my_number * 2) << std::endl;
    // } else {
    //     std::cout << "Entrada invalida." << std::endl;
    // }

    const int128_t n_prev{33};
    const int128_t n_next{34};

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << n_prev << "! == " << factorial(n_prev) << std::endl;
    std::cout << n_next << "! == " << factorial(n_next) << std::endl;

    return 0;
}
