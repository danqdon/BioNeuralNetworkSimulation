#include "NetworkManager.h"
#include "SpikeEvent.h"
#include "INeuron.h"
#include "ISynapse.h"
#include <iostream>

// Implementación de runSimulation
void NetworkManager::runSimulation(double tMax, double dt) {
    double currentTime = 0.0;
    int steps = static_cast<int>(tMax / dt);

    for (int step = 0; step < steps; ++step) {
        currentTime = step * dt;

        // 1) Actualizar dinámica de cada neurona (integración)
        for (auto &neuron : neurons) {
            neuron->stepSimulation(dt, currentTime);

            if (neuron->hasFired()) {
                // Registrar el tiempo del spike
                neuron->setLastSpikeTime(currentTime);

                // Para cada sinapsis saliente, crear un SpikeEvent con el tiempo de llegada
                for (auto &synapse : neuron->getOutgoingSynapses()) {
                    double delay = synapse->getDelay();
                    double arrivalTime = currentTime + delay;
                    SpikeEvent event(arrivalTime, neuron, synapse);
                    eventManager.pushEvent(event);
                }

                // Notificar a las sinapsis entrantes sobre el spike post para ajustar peso (LTD)
                for (auto &synapse : neuron->getIncomingSynapses()) {
                    synapse->deliverSpikeFromPost(neuron, currentTime);
                }
            }
        }

        // 2) Procesar todos los eventos cuyo 'time <= currentTime'
        while (!eventManager.empty() && eventManager.nextEventTime() <= currentTime) {
            SpikeEvent e = eventManager.popEvent();

            // Obtener sinapsis y neurona pre-sináptica
            auto synapsePtr = e.synapse.lock();
            auto preNeuronPtr = e.preNeuron.lock();
            if (synapsePtr && preNeuronPtr) {
                // Llamar al método deliverSpikeFromPre para inyectar la corriente y ajustar STDP
                synapsePtr->deliverSpikeFromPre(preNeuronPtr, e.time);
            }
        }
    }
}
