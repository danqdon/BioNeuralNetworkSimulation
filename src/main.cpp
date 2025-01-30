#include "NetworkManager.h"
#include "RandomConnectivityStrategy.h"
// Incluye otras estrategias según sea necesario

#include <iostream>

int main() {
    NetworkManager manager;

    // Crear 100 neuronas Izhikevich con parámetros por defecto
    for (int i = 0; i < 100; ++i) {
        manager.createNeuron(NeuronType::Izhikevich, {0.02, 0.2, -65.0, 8.0, 30.0, -65.0});
    }

    // Crear y asignar una estrategia de conectividad aleatoria
    double p = 0.1; // Probabilidad de conexión
    double weight = 0.5; // Peso de las sinapsis
    bool excitatory = true; // Tipo de sinapsis

    auto randomStrategy = std::make_unique<RandomConnectivityStrategy>(p, weight, excitatory);
    manager.setConnectivityStrategy(std::move(randomStrategy));

    // Aplicar la estrategia de conectividad para conectar las neuronas
    try {
        manager.applyConnectivityStrategy();
        std::cout << "Conectividad aplicada exitosamente.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error al aplicar conectividad: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    // Ejecutar la simulación (ejemplo: 1000 ms con pasos de 1 ms)
    double tMax = 1000.0; // Tiempo máximo en ms
    double dt = 1.0;       // Paso de tiempo en ms

    manager.runSimulation(tMax, dt);
    std::cout << "Simulacion completada.\n";

    return 0;
}
