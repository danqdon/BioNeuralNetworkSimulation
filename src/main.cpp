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

    // 1. Mostrar mensaje inicial
    std::cout << "=======================================\n";
    std::cout << "      SIMULACIÓN DE RED NEURONAL        \n";
    std::cout << "=======================================\n\n";

    // 2. Crear el directorio de salida para logs
    std::filesystem::create_directories("output_logs");

    // 3. Configurar el Logger para no mostrar logs en tiempo real
    Logger::getInstance().setRealTimeLogging(false);

    // 4. Definir la configuración de la red neuronal
    NetworkConfig config;
    config.totalNeurons = 1000;
    config.excitatoryRatio = 0.8; // 80% excitatorias
    config.inhibitoryRatio = 0.2; // 20% inhibitorias
    config.connectivityStrategy = "Random"; // Opciones: "Random", "SmallWorld", "ScaleFree"

    // Configuración para conexiones excitatorias
    config.excitatoryConnectivity.connectionProbability = 0.02;
    config.excitatoryConnectivity.defaultWeight = 0.5;
    config.excitatoryConnectivity.excitatory = true;

    // Configuración para conexiones inhibitorias
    config.inhibitoryConnectivity.connectionProbability = 0.02;
    config.inhibitoryConnectivity.defaultWeight = 0.5;
    config.inhibitoryConnectivity.excitatory = false;

    // 5. Crear y configurar el NetworkManager
    NetworkManager manager;
    try {
        manager.createNetwork(config);
        std::cout << "Red neuronal creada exitosamente.\n\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error al crear la red neuronal: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    // 6. Definir parámetros de la simulación
    double tMax = 1000.0; // Tiempo máximo en ms
    double dt   = 1.0;    // Paso de tiempo en ms

    // 7. Ejecutar la simulación
    std::cout << "Iniciando simulación de " << tMax << " ms con paso de " << dt << " ms...\n";
    manager.runSimulation(tMax, dt);
    std::cout << "Simulación completada.\n\n";

    // 8. Obtener y mostrar la cantidad de eventos registrados
    int spikeCount        = Logger::getInstance().getSpikeCount();
    int weightChangeCount = Logger::getInstance().getWeightChangeCount();

    std::cout << "Cantidad de spikes registrados: " << spikeCount << "\n";
    std::cout << "Cantidad de cambios de peso registrados: " << weightChangeCount << "\n\n";

    // 9. Exportar los logs a la carpeta 'output_logs'
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

    // Mensaje final
    std::cout << "Fin de la simulación de red neuronal.\n";

    return 0;
}
