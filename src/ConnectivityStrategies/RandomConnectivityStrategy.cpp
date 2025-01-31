// RandomConnectivityStrategy.cpp
#include "ConnectivityStrategies/RandomConnectivityStrategy.h"
#include "Network/NetworkManager.h"
#include <cmath>

namespace BioNeuralNetwork {

    void RandomConnectivityStrategy::connectNeurons(std::vector<std::shared_ptr<INeuron>>& neurons, NetworkManager& manager) {
        int numNeurons = static_cast<int>(neurons.size());

        for (int i = 0; i < numNeurons; ++i) {
            for (int j = 0; j < numNeurons; ++j) {
                if (i == j) continue; // Evitar auto-conexiones
                double randProb = dist(gen);
                if (randProb < p_connection) {
                    if (isExcitatory) {
                        manager.connectExcitatory(neurons[i], neurons[j], weight);
                    } else {
                        manager.connectInhibitory(neurons[i], neurons[j], weight);
                    }
                }
            }
        }
    }

}
