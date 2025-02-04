// NetworkManager.cpp
#include "Network/NetworkManager.h"
#include "ConnectivityStrategies/IConnectivityStrategy.h" // Ahora incluido
#include "Neurons/LIFNeuron.h"
#include "Neurons/IzhikevichNeuron.h"
#include "Synapses/ExcitatorySynapse.h"
#include "Synapses/InhibitorySynapse.h"
#include "ConnectivityStrategies/SmallWorldConnectivityStrategy.h"
#include "ConnectivityStrategies/ScaleFreeConnectivityStrategy.h"
#include "ConnectivityStrategies/RandomConnectivityStrategy.h"
#include "Core/Logger.h"

namespace BioNeuralNetwork {

void NetworkManager::addNeuron(std::shared_ptr<INeuron> neuron) {
    neurons.push_back(neuron);
}

void NetworkManager::addSynapse(std::shared_ptr<ISynapse> synapse) {
    synapses.push_back(synapse);
}

std::shared_ptr<INeuron> NetworkManager::createNeuron(NeuronType type, const std::vector<double>& params) {
    std::shared_ptr<INeuron> neuron;

    if (type == NeuronType::LIF) {
        if (params.size() >= 6) {
            neuron = std::make_shared<LIFNeuron>(
                params[0], // vRest
                params[1], // vReset
                params[2], // vThreshold
                params[3], // R
                params[4], // C
                params[5]  // refractoryTime
            );
        } else {
            neuron = std::make_shared<LIFNeuron>();
        }
    }
    else if (type == NeuronType::Izhikevich) {
        if (params.size() >= 9) {
            neuron = std::make_shared<IzhikevichNeuron>(
                params[0], // a
                params[1], // b
                params[2], // c
                params[3], // d
                params[4], // V_threshold
                params[5], // V_reset
                params[6], // R
                params[7], // C
                params[8]  // refractoryTime
            );
        } else {
            neuron = std::make_shared<IzhikevichNeuron>();
        }
    }
    else {
        throw std::invalid_argument("Tipo de neurona desconocido.");
    }

    addNeuron(neuron);
    return neuron;
}

void NetworkManager::connectExcitatory(std::shared_ptr<INeuron> pre,
                                      std::shared_ptr<INeuron> post,
                                      double weight,
                                      double delay)
{
    if (!pre || !post) {
        throw std::invalid_argument("Neurona pre o post nula en connectExcitatory.");
    }

    auto synapse = std::make_shared<ExcitatorySynapse>(pre, post, weight, delay);
    addSynapse(synapse);

    pre->addOutgoingSynapse(synapse);
    post->addIncomingSynapse(synapse);
}

void NetworkManager::connectInhibitory(std::shared_ptr<INeuron> pre,
                                      std::shared_ptr<INeuron> post,
                                      double weight,
                                      double delay)
{
    if (!pre || !post) {
        throw std::invalid_argument("Neurona pre o post nula en connectInhibitory.");
    }

    auto synapse = std::make_shared<InhibitorySynapse>(pre, post, weight, delay);
    addSynapse(synapse);

    pre->addOutgoingSynapse(synapse);
    post->addIncomingSynapse(synapse);
}

void NetworkManager::setConnectivityStrategy(std::unique_ptr<IConnectivityStrategy> strategy) {
    connectivityStrategy = std::move(strategy);
}

void NetworkManager::applyConnectivityStrategy() {
    if (connectivityStrategy) {
        connectivityStrategy->connectNeurons(neurons, *this);
    }
    else {
        throw std::runtime_error("No se ha establecido una estrategia de conectividad.");
    }
}

void NetworkManager::runSimulation(double tMax, double dt) {
    double currentTime = 0.0;
    int steps = static_cast<int>(tMax / dt);

    for (int step = 0; step < steps; ++step) {
        currentTime = step * dt;

        for (auto &neuron : neurons) {
            neuron->injectCurrent(10.0);
        }

        for (auto &neuron : neurons) {
            neuron->stepSimulation(dt, currentTime);

            if (neuron->hasFired()) {
                neuron->setLastSpikeTime(currentTime);
                for (auto &synapse : neuron->getOutgoingSynapses()) {
                    double synDelay = synapse->getDelay();
                    double arrivalTime = currentTime + synDelay;
                    SpikeEvent event(arrivalTime, neuron, synapse);
                    eventManager.pushEvent(event);
                }
                for (auto &synapse : neuron->getIncomingSynapses()) {
                    synapse->deliverSpikeFromPost(neuron, currentTime);
                }
            }
        }

        while (!eventManager.empty() && eventManager.nextEventTime() <= currentTime) {
            SpikeEvent e = eventManager.popEvent();
            auto synapsePtr = e.synapse.lock();
            auto preNeuronPtr = e.preNeuron.lock();
            if (synapsePtr && preNeuronPtr) {
                synapsePtr->deliverSpikeFromPre(preNeuronPtr, e.time);
            }
        }
    }

    try {
        Logger::getInstance().exportSpikesToCSV("output_logs/spikes.csv");
        Logger::getInstance().exportWeightChangesToCSV("output_logs/weight_changes.csv");
        Logger::getInstance().exportLogsToText("output_logs/simulation_logs.txt");
        std::cout << "Logs exportados exitosamente.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error al exportar logs: " << e.what() << "\n";
    }
}

void NetworkManager::createNetwork(const NetworkConfig& config) {
    // 1. Crear neuronas excitatorias
    int numExcitatory = static_cast<int>(config.totalNeurons * config.excitatoryRatio);
    int numInhibitory = static_cast<int>(config.totalNeurons * config.inhibitoryRatio);

    for (int i = 0; i < numExcitatory; ++i) {
        createNeuron(NeuronType::Izhikevich,
                     {0.02, 0.2, -65.0, 8.0, 30.0, -65.0, 1.0, 1.0, 2.0});
    }

    for (int i = 0; i < numInhibitory; ++i) {
        createNeuron(NeuronType::Izhikevich,
                     {0.1, 0.2, -65.0, 2.0, 30.0, -65.0, 1.0, 1.0, 2.0});
    }

    if (config.connectivityStrategy == "Random") {
        // Conectividad aleatoria excitatoria
        auto randomStrategy = std::make_unique<RandomConnectivityStrategy>(
            config.excitatoryConnectivity.connectionProbability,
            config.excitatoryConnectivity.defaultWeight,
            config.excitatoryConnectivity.excitatory
        );
        setConnectivityStrategy(std::move(randomStrategy));
        applyConnectivityStrategy();

        auto randomStrategyInh = std::make_unique<RandomConnectivityStrategy>(
            config.inhibitoryConnectivity.connectionProbability,
            config.inhibitoryConnectivity.defaultWeight,
            config.inhibitoryConnectivity.excitatory
        );
        setConnectivityStrategy(std::move(randomStrategyInh));
        applyConnectivityStrategy();
    }
    else if (config.connectivityStrategy == "SmallWorld") {
        auto smallWorldStrategy = std::make_unique<SmallWorldConnectivityStrategy>(
            0.1,
            10
        );
        setConnectivityStrategy(std::move(smallWorldStrategy));
        applyConnectivityStrategy();

        auto smallWorldStrategyInh = std::make_unique<SmallWorldConnectivityStrategy>(
            0.1,
            10
        );
        setConnectivityStrategy(std::move(smallWorldStrategyInh));
        applyConnectivityStrategy();
    }
    else if (config.connectivityStrategy == "ScaleFree") {
        int initialNodes = 5;
        int connectionsPerNewNode = 3;
        auto scaleFreeStrategy = std::make_unique<ScaleFreeConnectivityStrategy>(
            initialNodes,
            connectionsPerNewNode
        );
        setConnectivityStrategy(std::move(scaleFreeStrategy));
        applyConnectivityStrategy();

        auto scaleFreeStrategyInh = std::make_unique<ScaleFreeConnectivityStrategy>(
            initialNodes,
            connectionsPerNewNode
        );
        setConnectivityStrategy(std::move(scaleFreeStrategyInh));
        applyConnectivityStrategy();
    }
    else {
        throw std::invalid_argument("Estrategia de conectividad desconocida: " + config.connectivityStrategy);
    }
}

}
