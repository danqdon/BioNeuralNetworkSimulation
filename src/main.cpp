#include <iostream>
#include "Network/NetworkManager.h"
#include "ConnectivityStrategies/RandomConnectivityStrategy.h"
#include "Core/Logger.h"

void testLogger() {
    using namespace BioNeuralNetwork;

    Logger& logger = Logger::getInstance();
    // Registrar eventos de prueba
    logger.logSpike(1, 10.0);
    logger.logSpike(2, 20.5);
    logger.logWeightChange("1_2", 15.0, 1.0, 1.1);
    logger.logWeightChange("2_3", 25.0, 1.0, 0.9);

    // Exportar a texto
    logger.exportLogsToText("test_logs.txt");

    // Exportar a CSV
    logger.exportSpikesToCSV("test_spikes.csv");
    logger.exportWeightChangesToCSV("test_weight_changes.csv");

    std::cout << "Test de Logger completado.\n";
}

int main() {
    using namespace BioNeuralNetwork;

    std::cout << "Iniciando simulación de red neuronal...\n";

    NetworkManager manager;

    // Crear neuronas de ejemplo
    int numNeurons = 5;
    for (int i = 0; i < numNeurons; ++i) {
        manager.createNeuron(NeuronType::Izhikevich,
            {0.02, 0.2, -65.0, 8.0, 30.0, -65.0, 1.0, 1.0, 2.0});
    }
    std::cout << numNeurons << " neuronas creadas.\n";

    // Asignar una estrategia de conectividad
    double p = 0.5;
    double weight = 0.5;
    bool excitatory = true;

    auto randomStrategy = std::make_unique<RandomConnectivityStrategy>(p, weight, excitatory);
    manager.setConnectivityStrategy(std::move(randomStrategy));

    // Aplicar la conectividad
    try {
        manager.applyConnectivityStrategy();
        std::cout << "Conectividad aplicada exitosamente.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error al aplicar conectividad: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    {
        auto& allNeurons = manager.getNeurons();
        for (auto& neuron : allNeurons) {
            // Este valor es un ejemplo, ajústalo según tus necesidades
            neuron->injectCurrent(50.0);
        }
    }

    // Ejecutar la simulación
    double tMax = 100.0;
    double dt = 1.0;

    std::cout << "Iniciando simulación...\n";
    manager.runSimulation(tMax, dt);
    std::cout << "Simulación completada.\n";

    // Cantidad de eventos registrados
    int spikeCount = Logger::getInstance().getSpikeCount();
    int weightChangeCount = Logger::getInstance().getWeightChangeCount();

    std::cout << "Cantidad de spikes registrados: " << spikeCount << "\n";
    std::cout << "Cantidad de cambios de peso registrados: " << weightChangeCount << "\n";

    // Exportar logs a archivo de texto
    try {
        Logger::getInstance().exportLogsToText("simulation_logs.txt");
        std::cout << "Logs exportados exitosamente a 'simulation_logs.txt'.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error al exportar logs: " << e.what() << "\n";
    }

    // Ejecutar test de Logger (este sí te crea logs "ficticios" de prueba)
    testLogger();

    return 0;
}
