#ifndef SPIKEEVENT_H
#define SPIKEEVENT_H

#include <memory>

namespace BioNeuralNetwork {

    class INeuron;
    class ISynapse;

    /**
     * @brief Estructura que representa un evento de spike
     */
    struct SpikeEvent {
        double time;  // Momento en que el spike llega a la neurona post-sináptica
        std::weak_ptr<INeuron> preNeuron;  // Neurona pre-sináptica
        std::weak_ptr<ISynapse> synapse;   // Sinapsis a través de la cual se transmite el spike

        SpikeEvent(double t,
                   const std::shared_ptr<INeuron>& pre,
                   const std::shared_ptr<ISynapse>& s)
            : time(t), preNeuron(pre), synapse(s) {}
    };

}

#endif // SPIKEEVENT_H
