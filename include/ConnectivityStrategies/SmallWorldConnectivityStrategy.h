#ifndef SMALLWORLDCONNECTIVITYSTRATEGY_H
#define SMALLWORLDCONNECTIVITYSTRATEGY_H

#include "IConnectivityStrategy.h"
#include <random>

namespace BioNeuralNetwork {

    /**
     * @brief Estrategia de conectividad Small World.
     */
    class SmallWorldConnectivityStrategy : public IConnectivityStrategy {
    public:
        /**
         * @param rewiringProbability Probabilidad de reconexión.
         * @param connectionsPerNeuron Número de conexiones iniciales por neurona.
         */
        SmallWorldConnectivityStrategy(double rewiringProbability, int connectionsPerNeuron)
            : p_rewire(rewiringProbability), k(connectionsPerNeuron), gen(rd()), dist(0.0, 1.0) {}

        void connectNeurons(std::vector<std::shared_ptr<INeuron>>& neurons, NetworkManager& manager) override;

    private:
        double p_rewire;               // Probabilidad de reconexión
        int k;                         // Número de conexiones por neurona
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<> dist;
    };

} // namespace BioNeuralNetwork

#endif // SMALLWORLDCONNECTIVITYSTRATEGY_H
