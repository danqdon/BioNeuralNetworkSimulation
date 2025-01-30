// src/Network/NetworkManager.cpp
#include "Network/NetworkManager.h"

#include <iostream>
#include <Core/Logger.h>

#include "Neurons/LIFNeuron.h"
#include "Neurons/IzhikevichNeuron.h"
#include "Synapses/ExcitatorySynapse.h"
#include "Synapses/InhibitorySynapse.h"

namespace BioNeuralNetwork {

std::shared_ptr<INeuron> NetworkManager::createNeuron(NeuronType type, const std::vector<double>& params)
{
    std::shared_ptr<INeuron> neuron;

    switch (type) {
        case NeuronType::LIF:
            if (params.size() >= 5) {
                neuron = std::make_shared<LIFNeuron>(
                    params[0], // vRest
                    params[1], // vReset
                    params[2], // vThreshold
                    params[3], // rMembrane
                    params[4], // cMembrane
                    (params.size() >= 6) ? params[5] : 2.0 // refractoryPeriod
                );
            } else {
                neuron = std::make_shared<LIFNeuron>();
            }
            break;

        case NeuronType::Izhikevich:
            if (params.size() >= 6) {
                neuron = std::make_shared<IzhikevichNeuron>(
                    params[0], // a
                    params[1], // b
                    params[2], // c
                    params[3], // d
                    params[4], // V_threshold
                    params[5], // V_reset
                    params.size() >= 7 ? params[6] : 1.0, // R_param
                    params.size() >= 8 ? params[7] : 1.0, // C_param
                    params.size() >= 9 ? params[8] : 2.0  // refractoryTime_param
                );
            } else {
                neuron = std::make_shared<IzhikevichNeuron>();
            }
            break;

        default:
            throw std::invalid_argument("Tipo de neurona desconocido");
    }

    addNeuron(neuron);
    return neuron;
}

void NetworkManager::addNeuron(std::shared_ptr<INeuron> neuron) {
    neurons.push_back(neuron);
}

void NetworkManager::addSynapse(std::shared_ptr<ISynapse> synapse) {
    synapses.push_back(synapse);
}

void NetworkManager::connectExcitatory(std::shared_ptr<INeuron> pre,
                                       std::shared_ptr<INeuron> post,
                                       double weight,
                                       double delay) {
    auto synapse = std::make_shared<ExcitatorySynapse>(pre, post, weight, delay);
    synapses.push_back(synapse);
    pre->addOutgoingSynapse(synapse);
    post->addIncomingSynapse(synapse);
}

void NetworkManager::connectInhibitory(std::shared_ptr<INeuron> pre,
                                       std::shared_ptr<INeuron> post,
                                       double weight,
                                       double delay) {
    auto synapse = std::make_shared<InhibitorySynapse>(pre, post, weight, delay);
    synapses.push_back(synapse);
    pre->addOutgoingSynapse(synapse);
    post->addIncomingSynapse(synapse);
}

void NetworkManager::setConnectivityStrategy(std::unique_ptr<IConnectivityStrategy> strategy) {
    connectivityStrategy = std::move(strategy);
}

void NetworkManager::applyConnectivityStrategy() {
    if (!connectivityStrategy) {
        throw std::runtime_error("No se ha establecido una estrategia de conectividad.");
    }
    connectivityStrategy->connectNeurons(neurons, *this);
}

void NetworkManager::runSimulation(double tMax, double dt) {
    double currentTime = 0.0;
    int steps = static_cast<int>(tMax / dt);

    for (int step = 0; step < steps; ++step) {
        currentTime = step * dt;

        // 1) Actualizar la dinámica de cada neurona
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

        // 2) Procesar todos los eventos cuyo 'time <= currentTime'
        while (!eventManager.empty() && eventManager.nextEventTime() <= currentTime) {
            SpikeEvent e = eventManager.popEvent();

            auto synapsePtr = e.synapse.lock();
            auto preNeuronPtr = e.preNeuron.lock();
            if (synapsePtr && preNeuronPtr) {
                synapsePtr->deliverSpikeFromPre(preNeuronPtr, e.time);
            }
        }
    }

    // Exportar logs al finalizar la simulación
    try {
        Logger::getInstance().exportSpikesToCSV("spikes.csv");
        Logger::getInstance().exportWeightChangesToCSV("weight_changes.csv");
        std::cout << "Logs exportados exitosamente.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error al exportar logs: " << e.what() << "\n";
    }
}

} // namespace BioNeuralNetwork
