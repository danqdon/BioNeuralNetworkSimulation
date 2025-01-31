// NetworkManager.h
#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <vector>
#include <memory>
#include <string>
#include "Core/INeuron.h"
#include "Core/ISynapse.h"
#include "Core/SpikeEvent.h"
#include "Core/EventManager.h"
#include "Core/NetworkConfig.h"

namespace BioNeuralNetwork {

// Forward declaration de IConnectivityStrategy
class IConnectivityStrategy;

enum class NeuronType {
    LIF,
    Izhikevich
};

class NetworkManager {
public:
    NetworkManager() = default;

    // Crear neuronas individualmente
    std::shared_ptr<INeuron> createNeuron(NeuronType type, const std::vector<double>& params = {});

    // Conectar neuronas individualmente
    void connectExcitatory(std::shared_ptr<INeuron> pre,
                           std::shared_ptr<INeuron> post,
                           double weight = 1.0,
                           double delay = 1.0);

    void connectInhibitory(std::shared_ptr<INeuron> pre,
                           std::shared_ptr<INeuron> post,
                           double weight = 1.0,
                           double delay = 1.0);

    // Establecer y aplicar estrategias de conectividad
    void setConnectivityStrategy(std::unique_ptr<IConnectivityStrategy> strategy);
    void applyConnectivityStrategy();

    // Ejecutar simulación
    void runSimulation(double tMax, double dt);

    // Getter para acceder a las neuronas
    const std::vector<std::shared_ptr<INeuron>>& getNeurons() const {
        return neurons;
    }

    // Método de Alto Nivel para Crear la Red
    void createNetwork(const NetworkConfig& config);

private:
    std::vector<std::shared_ptr<INeuron>> neurons;
    std::vector<std::shared_ptr<ISynapse>> synapses;

    // Puntero a IConnectivityStrategy, usando forward declaration
    std::unique_ptr<IConnectivityStrategy> connectivityStrategy;

    EventManager eventManager;

    // Métodos privados para agregar neuronas y sinapsis
    void addNeuron(std::shared_ptr<INeuron> neuron);
    void addSynapse(std::shared_ptr<ISynapse> synapse);
};

} // namespace BioNeuralNetwork

#endif // NETWORKMANAGER_H
