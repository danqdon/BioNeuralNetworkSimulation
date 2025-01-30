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

#include "ConnectivityStrategy.h" // Inclusión para el Patrón Strategy

// Definición del tipo de neurona
enum class NeuronType {
    LIF,
    Izhikevich
    // Puedes añadir más tipos en el futuro
};

// Forward declaration para evitar incluir el header completo
class IConnectivityStrategy;

// Clase que gestiona la red de neuronas y sinapsis
class NetworkManager {
public:
    /**
     * @brief Crea y añade una neurona específica a la red.
     *
     * @param type Tipo de neurona (LIF, Izhikevich, etc.).
     * @param params Parámetros específicos para la creación de la neurona.
     * @return std::shared_ptr<INeuron> Puntero compartido a la neurona creada.
     */
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
                        params[4], // V_threshold
                        params[5]  // V_reset
                        // Opcionales: R, C, refractoryTime (usados valores por defecto si no se proporcionan)
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

    /**
     * @brief Añade una neurona a la red.
     *
     * @param neuron Puntero compartido a la neurona a añadir.
     */
    void addNeuron(std::shared_ptr<INeuron> neuron) {
        neurons.push_back(neuron);
    }

    /**
     * @brief Añade una sinapsis a la red.
     *
     * @param synapse Puntero compartido a la sinapsis a añadir.
     */
    void addSynapse(std::shared_ptr<ISynapse> synapse) {
        synapses.push_back(synapse);
    }

    /**
     * @brief Conecta dos neuronas con una sinapsis excitatoria.
     *
     * @param pre Neurona presináptica.
     * @param post Neurona postsináptica.
     * @param weight Peso de la sinapsis.
     * @param delay Retardo de la sinapsis (opcional).
     */
    void connectExcitatory(std::shared_ptr<INeuron> pre, std::shared_ptr<INeuron> post,
                           double weight, double delay = 1.0) {
        auto synapse = std::make_shared<ExcitatorySynapse>(pre, post, weight, delay);
        synapses.push_back(synapse);
        pre->addOutgoingSynapse(synapse);
        post->addIncomingSynapse(synapse);
    }

    /**
     * @brief Conecta dos neuronas con una sinapsis inhibidora.
     *
     * @param pre Neurona presináptica.
     * @param post Neurona postsináptica.
     * @param weight Peso de la sinapsis.
     * @param delay Retardo de la sinapsis (opcional).
     */
    void connectInhibitory(std::shared_ptr<INeuron> pre, std::shared_ptr<INeuron> post,
                           double weight, double delay = 1.0) {
        auto synapse = std::make_shared<InhibitorySynapse>(pre, post, weight, delay);
        synapses.push_back(synapse);
        pre->addOutgoingSynapse(synapse);
        post->addIncomingSynapse(synapse);
    }

    /**
     * @brief Asigna una estrategia de conectividad a la red.
     *
     * @param strategy Puntero único a la estrategia de conectividad.
     */
    void setConnectivityStrategy(std::unique_ptr<IConnectivityStrategy> strategy) {
        connectivityStrategy = std::move(strategy);
    }

    /**
     * @brief Aplica la estrategia de conectividad asignada para conectar las neuronas.
     *
     * Lanza una excepción si no se ha asignado ninguna estrategia.
     */
    void applyConnectivityStrategy() {
        if (connectivityStrategy) {
            connectivityStrategy->connectNeurons(neurons, *this);
        } else {
            throw std::runtime_error("No se ha establecido una estrategia de conectividad.");
        }
    }

    /**
     * @brief Ejecuta la simulación de la red durante un tiempo máximo con un paso de tiempo dado.
     *
     * @param tMax Tiempo máximo de simulación en milisegundos.
     * @param dt Paso de tiempo en milisegundos.
     */
    void runSimulation(double tMax, double dt);

    /**
     * @brief Obtiene todas las sinapsis de la red.
     *
     * @return const std::vector<std::shared_ptr<ISynapse>>& Referencia constante al vector de sinapsis.
     */
    const std::vector<std::shared_ptr<ISynapse>>& getSynapses() const { return synapses; }

private:
    std::vector<std::shared_ptr<INeuron>> neurons;             // Vector de neuronas en la red
    std::vector<std::shared_ptr<ISynapse>> synapses;           // Vector de sinapsis en la red

    EventManager eventManager; // Cola global de eventos

    std::unique_ptr<IConnectivityStrategy> connectivityStrategy; // Estrategia de conectividad asignada
};

#endif // NETWORKMANAGER_H
