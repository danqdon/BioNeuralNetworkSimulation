// include/NetworkManager.h
#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <vector>
#include <memory>
#include <stdexcept>

#include "INeuron.h"
#include "ISynapse.h"
#include "ExcitatorySynapse.h"
#include "InhibitorySynapse.h"
#include "IzhikevichNeuron.h"
#include "LIFNeuron.h"
#include "EventManager.h"

// Definición del tipo de neurona
enum class NeuronType {
    LIF,
    Izhikevich
    // Puedes añadir más tipos en el futuro
};

// Clase que gestiona la red de neuronas y sinapsis
class NetworkManager {
public:
    // Método para crear y añadir una neurona específica
    std::shared_ptr<INeuron> createNeuron(NeuronType type, const std::vector<double>& params = {}) {
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
                        params[4], // vInit
                        params[5]  // uInit
                    );
                } else {
                    neuron = std::make_shared<IzhikevichNeuron>();
                }
                break;

            // Añadir más tipos de neuronas aquí

            default:
                throw std::invalid_argument("Tipo de neurona desconocido");
        }

        addNeuron(neuron);
        return neuron;
    }

    // Añade una neurona a la red
    void addNeuron(std::shared_ptr<INeuron> neuron) {
        neurons.push_back(neuron);
    }

    // Añade una sinapsis a la red
    void addSynapse(std::shared_ptr<ISynapse> synapse) {
        synapses.push_back(synapse);
    }

    // Conecta dos neuronas con una sinapsis excitatoria
    void connectExcitatory(std::shared_ptr<INeuron> pre, std::shared_ptr<INeuron> post,
                           double weight, double delay = 1.0) {
        auto synapse = std::make_shared<ExcitatorySynapse>(pre, post, weight, delay);
        synapses.push_back(synapse);
        pre->addOutgoingSynapse(synapse);
        post->addIncomingSynapse(synapse);
    }

    // Conecta dos neuronas con una sinapsis inhibidora
    void connectInhibitory(std::shared_ptr<INeuron> pre, std::shared_ptr<INeuron> post,
                           double weight, double delay = 1.0) {
        auto synapse = std::make_shared<InhibitorySynapse>(pre, post, weight, delay);
        synapses.push_back(synapse);
        pre->addOutgoingSynapse(synapse);
        post->addIncomingSynapse(synapse);
    }

    // Ejecuta la simulación durante tMax milisegundos con un paso de tiempo dt
    void runSimulation(double tMax, double dt);

    // Obtener las sinapsis para imprimir pesos finales
    const std::vector<std::shared_ptr<ISynapse>>& getSynapses() const { return synapses; }

private:
    std::vector<std::shared_ptr<INeuron>> neurons;
    std::vector<std::shared_ptr<ISynapse>> synapses;

    EventManager eventManager; // Cola global de eventos
};

#endif // NETWORKMANAGER_H
