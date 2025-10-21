#include <int128_helpers.hpp>
#include <test_helpers.hpp>

// Hacemos los literales _u128 y _i128 visibles en este archivo
using namespace int128_literals;

int main() {
    run_tests(); // Ejecutar las pruebas primero

    std::cout << "Introduce un numero grande (de hasta 128 bits):" << std::endl;
    uint128_t my_number;
    std::cin >> my_number;

    if (std::cin.good()) {
        std::cout << "Has introducido: " << my_number << std::endl;
        uint128_t max_val = "340282366920938463463374607431768211455"_u128;
        std::cout << "El valor maximo para uint128_t es: " << max_val << std::endl;
        std::cout << "El numero por dos es: " << (my_number * 2) << std::endl;
    } else {
        std::cout << "Entrada invalida." << std::endl;
    }

    return 0;
}
