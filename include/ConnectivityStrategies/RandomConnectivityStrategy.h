// RandomConnectivityStrategy.h
#ifndef RANDOMCONNECTIVITYSTRATEGY_H
#define RANDOMCONNECTIVITYSTRATEGY_H

#include "IConnectivityStrategy.h"
#include <random>

namespace BioNeuralNetwork {

    /**
     * @brief Estrategia de conectividad aleatoria.
     */
    class RandomConnectivityStrategy : public IConnectivityStrategy {
    public:
        /**
         * @param connectionProbability Probabilidad de conexión entre neuronas.
         * @param defaultWeight Peso por defecto de las sinapsis.
         * @param excitatory Si es verdadero, las conexiones son excitatorias; de lo contrario, inhibitorias.
         */
        RandomConnectivityStrategy(double connectionProbability, double defaultWeight, bool excitatory)
            : p_connection(connectionProbability), weight(defaultWeight), isExcitatory(excitatory),
              gen(rd()), dist(0.0, 1.0) {}

        void connectNeurons(std::vector<std::shared_ptr<INeuron>>& neurons, NetworkManager& manager) override;

    private:
        double p_connection;
        double weight;
        bool isExcitatory;
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<> dist;
    };

}

#endif // RANDOMCONNECTIVITYSTRATEGY_H
