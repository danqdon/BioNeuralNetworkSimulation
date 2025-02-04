#include <iostream>
#include <filesystem>  // Para crear el directorio de logs, C++17/20

#include "Network/NetworkManager.h"
#include "ConnectivityStrategies/RandomConnectivityStrategy.h"
#include "ConnectivityStrategies/SmallWorldConnectivityStrategy.h"       // <-- Añadido
#include "ConnectivityStrategies/ScaleFreeConnectivityStrategy.h"        // <-- Añadido
#include "Core/Logger.h"
#include "Core/NetworkConfig.h" // <-- Añadido

int main() {
    using namespace BioNeuralNetwork;

    std::cout << "=======================================\n";
    std::cout << "      SIMULACION DE RED NEURONAL        \n";
    std::cout << "=======================================\n\n";

    std::filesystem::create_directories("output_logs");

    Logger::getInstance().setRealTimeLogging(false);

    NetworkConfig config;
    config.totalNeurons = 1000;
    config.excitatoryRatio = 0.8;
    config.inhibitoryRatio = 0.2;
    config.connectivityStrategy = "ScaleFree";

    config.excitatoryConnectivity.connectionProbability = 0.02;
    config.excitatoryConnectivity.defaultWeight = 0.5;
    config.excitatoryConnectivity.excitatory = true;

    config.inhibitoryConnectivity.connectionProbability = 0.02;
    config.inhibitoryConnectivity.defaultWeight = 0.5;
    config.inhibitoryConnectivity.excitatory = false;

    NetworkManager manager;
    try {
        manager.createNetwork(config);
        std::cout << "Red neuronal creada exitosamente.\n\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error al crear la red neuronal: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    double tMax = 1000.0;
    double dt   = 1.0;

    std::cout << "Iniciando simulacion de " << tMax << " ms con paso de " << dt << " ms...\n";
    manager.runSimulation(tMax, dt);
    std::cout << "Simulacion completada.\n\n";

    int spikeCount        = Logger::getInstance().getSpikeCount();
    int weightChangeCount = Logger::getInstance().getWeightChangeCount();

    std::cout << "Cantidad de spikes registrados: " << spikeCount << "\n";
    std::cout << "Cantidad de cambios de peso registrados: " << weightChangeCount << "\n\n";

    try {
        Logger::getInstance().exportSpikesToCSV("output_logs/spikes.csv");
        Logger::getInstance().exportWeightChangesToCSV("output_logs/weight_changes.csv");
        Logger::getInstance().exportLogsToText("output_logs/simulation_logs.txt");

        std::cout << "Logs exportados exitosamente en la carpeta 'output_logs/'\n";
        std::cout << " - spikes.csv\n";
        std::cout << " - weight_changes.csv\n";
        std::cout << " - simulation_logs.txt\n\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error al exportar logs: " << e.what() << "\n";
    }

    std::cout << "Fin de la simulacion de red neuronal.\n";

    return 0;
}
