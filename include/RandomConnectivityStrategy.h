#ifndef RANDOMCONNECTIVITYSTRATEGY_H
#define RANDOMCONNECTIVITYSTRATEGY_H

#include "ConnectivityStrategy.h"
#include <random>
#include "NetworkManager.h"  // Include the full definition


/**
 * @brief Estrategia que conecta neuronas aleatoriamente con una probabilidad p.
 */
class RandomConnectivityStrategy : public IConnectivityStrategy {
public:
    /**
     * @brief Constructor.
     *
     * @param p Probabilidad de conexión entre dos neuronas (0 <= p <= 1).
     * @param weight Peso por defecto de las sinapsis creadas.
     * @param isExcitatory Indica si las sinapsis son excitatorias (true) o inhibitorias (false).
     */
    RandomConnectivityStrategy(double p, double weight, bool isExcitatory = true)
        : connectionProbability(p),
          defaultWeight(weight),
          excitatory(isExcitatory),
          gen(rd()),
          dist(0.0, 1.0)
    {}

    /**
     * @brief Implementación de la conexión aleatoria.
     *
     * @param neurons Vector de neuronas a conectar.
     * @param manager Referencia al NetworkManager para crear sinapsis.
     */
    void connectNeurons(std::vector<std::shared_ptr<INeuron>>& neurons, NetworkManager& manager) override {
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

private:
    double connectionProbability;
    double defaultWeight;
    bool excitatory;

    // Generador de números aleatorios
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dist;
};

#endif // RANDOMCONNECTIVITYSTRATEGY_H
