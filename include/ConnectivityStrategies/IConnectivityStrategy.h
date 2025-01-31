// IConnectivityStrategy.h
#ifndef ICONNECTIVITYSTRATEGY_H
#define ICONNECTIVITYSTRATEGY_H

#include <vector>
#include <memory>
#include "Core/INeuron.h"

namespace BioNeuralNetwork {

    // Forward declaration de NetworkManager
    class NetworkManager;

    /**
     * @brief Interfaz para las estrategias de conectividad.
     */
    class IConnectivityStrategy {
    public:
        virtual ~IConnectivityStrategy() = default;

        /**
         * @brief Método para conectar neuronas según la estrategia.
         * @param neurons Vector de neuronas a conectar.
         * @param manager Referencia al NetworkManager para crear sinapsis.
         */
        virtual void connectNeurons(std::vector<std::shared_ptr<INeuron>>& neurons, NetworkManager& manager) = 0;
    };

} // namespace BioNeuralNetwork

#endif // ICONNECTIVITYSTRATEGY_H
