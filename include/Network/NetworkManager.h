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

class IConnectivityStrategy;

enum class NeuronType {
    LIF,
    Izhikevich
};

class NetworkManager {
public:
    NetworkManager() = default;

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

    void setConnectivityStrategy(std::unique_ptr<IConnectivityStrategy> strategy);
    void applyConnectivityStrategy();

    void runSimulation(double tMax, double dt);

    const std::vector<std::shared_ptr<INeuron>>& getNeurons() const {
        return neurons;
    }

    void createNetwork(const NetworkConfig& config);

private:
    std::vector<std::shared_ptr<INeuron>> neurons;
    std::vector<std::shared_ptr<ISynapse>> synapses;

    std::unique_ptr<IConnectivityStrategy> connectivityStrategy;

    EventManager eventManager;

    void addNeuron(std::shared_ptr<INeuron> neuron);
    void addSynapse(std::shared_ptr<ISynapse> synapse);
};

}

#endif // NETWORKMANAGER_H
