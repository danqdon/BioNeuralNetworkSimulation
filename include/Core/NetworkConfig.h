#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H

#include <string>

namespace BioNeuralNetwork {

    struct ConnectivityParameters {
        double connectionProbability;
        double defaultWeight;
        bool excitatory; // true para excitatorio, false para inhibitorio
    };

    struct NetworkConfig {
        int totalNeurons;
        double excitatoryRatio;
        double inhibitoryRatio;
        ConnectivityParameters excitatoryConnectivity;
        ConnectivityParameters inhibitoryConnectivity;
        std::string connectivityStrategy; // Opciones: "Random", "SmallWorld", "ScaleFree"
        // Puedes añadir más parámetros según tus necesidades
    };

} // namespace BioNeuralNetwork

#endif // NETWORKCONFIG_H
