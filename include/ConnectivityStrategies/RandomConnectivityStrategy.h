#ifndef RANDOMCONNECTIVITYSTRATEGY_H
#define RANDOMCONNECTIVITYSTRATEGY_H

#include <random>
#include <vector>
#include <memory>

#include "IConnectivityStrategy.h"
#include "Network/NetworkManager.h"  // Para usar NetworkManager en la firma

namespace BioNeuralNetwork {

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
        RandomConnectivityStrategy(double p, double weight, bool isExcitatory = true);

        /**
         * @brief Implementación de la conexión aleatoria.
         *
         * @param neurons Vector de neuronas a conectar.
         * @param manager Referencia al NetworkManager para crear sinapsis.
         */
        void connectNeurons(std::vector<std::shared_ptr<INeuron>>& neurons,
                            NetworkManager& manager) override;

    private:
        double connectionProbability;
        double defaultWeight;
        bool excitatory;

        // Generador de números aleatorios
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<> dist;
    };

} // namespace BioNeuralNetwork

#endif // RANDOMCONNECTIVITYSTRATEGY_H
