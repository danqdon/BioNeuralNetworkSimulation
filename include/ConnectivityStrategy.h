#ifndef CONNECTIVITYSTRATEGY_H
#define CONNECTIVITYSTRATEGY_H

#include <memory>
#include <vector>
#include "INeuron.h"

// Forward declaration para evitar dependencias circulares
class NetworkManager;

/**
 * @brief Interfaz para distintas estrategias de conectividad de la red.
 *
 * Cada implementación de IConnectivityStrategy encapsulará la lógica
 * para conectar un conjunto de neuronas según algún patrón (aleatorio,
 * small-world, scale-free, etc.).
 */
class IConnectivityStrategy {
public:
    virtual ~IConnectivityStrategy() = default;

    /**
     * @brief Conecta las neuronas en el vector `neurons` usando la
     *        lógica de cada estrategia concreta.
     *
     * @param neurons Referencia al contenedor de neuronas.
     * @param manager Referencia al NetworkManager para crear sinapsis.
     */
    virtual void connectNeurons(std::vector<std::shared_ptr<INeuron>>& neurons, NetworkManager& manager) = 0;
};

#endif // CONNECTIVITYSTRATEGY_H
