#ifndef ICONNECTIVITYSTRATEGY_H
#define ICONNECTIVITYSTRATEGY_H

#include <memory>
#include <vector>

namespace BioNeuralNetwork {

    // Forward declaration
    class INeuron;
    class NetworkManager;

    /**
     * @brief Interfaz para distintas estrategias de conectividad de la red.
     */
    class IConnectivityStrategy {
    public:
        virtual ~IConnectivityStrategy() = default;

        /**
         * @brief Conecta las neuronas usando la lógica de la estrategia concreta.
         *
         * @param neurons Vector de neuronas a conectar.
         * @param manager Referencia al NetworkManager para crear sinapsis.
         */
        virtual void connectNeurons(std::vector<std::shared_ptr<INeuron>>& neurons,
                                    NetworkManager& manager) = 0;
    };

} // namespace BioNeuralNetwork

#endif // ICONNECTIVITYSTRATEGY_H
