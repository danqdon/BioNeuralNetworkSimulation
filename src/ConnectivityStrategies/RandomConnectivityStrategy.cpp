#include "ConnectivityStrategies/RandomConnectivityStrategy.h"
#include "Core/INeuron.h"
#include "Network/NetworkManager.h"

namespace BioNeuralNetwork {

    RandomConnectivityStrategy::RandomConnectivityStrategy(double p,
                                                           double weight,
                                                           bool isExcitatory)
        : connectionProbability(p),
          defaultWeight(weight),
          excitatory(isExcitatory),
          gen(rd()),
          dist(0.0, 1.0)
    {
    }

    void RandomConnectivityStrategy::connectNeurons(std::vector<std::shared_ptr<INeuron>>& neurons,
                                                    NetworkManager& manager)
    {
        size_t numNeurons = neurons.size();
        for (size_t i = 0; i < numNeurons; ++i) {
            for (size_t j = 0; j < numNeurons; ++j) {
                if (i == j) continue; // Evitar auto-conexiones

                double randVal = dist(gen);
                if (randVal < connectionProbability) {
                    if (excitatory) {
                        manager.connectExcitatory(neurons[i], neurons[j], defaultWeight);
                    } else {
                        manager.connectInhibitory(neurons[i], neurons[j], defaultWeight);
                    }
                }
            }
        }
    }

} // namespace BioNeuralNetwork
