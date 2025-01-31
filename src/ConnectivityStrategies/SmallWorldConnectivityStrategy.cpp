// SmallWorldConnectivityStrategy.cpp
#include "ConnectivityStrategies/SmallWorldConnectivityStrategy.h"
#include "Network/NetworkManager.h"
#include <cmath>
#include <random>

namespace BioNeuralNetwork {

    void SmallWorldConnectivityStrategy::connectNeurons(std::vector<std::shared_ptr<INeuron>>& neurons, NetworkManager& manager) {
        int numNeurons = static_cast<int>(neurons.size());

        // Crear una red regular primero: cada neurona se conecta a 'k' neuronas a su alrededor
        for (int i = 0; i < numNeurons; ++i) {
            for (int j = 1; j <= k / 2; ++j) {
                int target = (i + j) % numNeurons;
                manager.connectExcitatory(neurons[i], neurons[target]);
                manager.connectExcitatory(neurons[target], neurons[i]); // Asumiendo conexiones bidireccionales
            }
        }

        // Rewiring (reconectar) con probabilidad p_rewire
        for (int i = 0; i < numNeurons; ++i) {
            for (int j = 1; j <= k / 2; ++j) {
                if (dist(gen) < p_rewire) {
                    // Seleccionar una nueva neurona aleatoria para reconectar
                    std::uniform_int_distribution<> newTargetDist(0, numNeurons - 1);
                    int newTarget = newTargetDist(gen);
                    // Evitar auto-conexiones y duplicados
                    if (newTarget == i) continue;

                    // Re-conectar
                    manager.connectExcitatory(neurons[i], neurons[newTarget]);
                }
            }
        }
    }

} // namespace BioNeuralNetwork
