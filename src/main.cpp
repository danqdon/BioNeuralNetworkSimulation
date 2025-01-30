// src/main.cpp
#include <iostream>
#include "LIFNeuron.h"
#include "IzhikevichNeuron.h"
#include "ExcitatorySynapse.h"
#include "InhibitorySynapse.h"
#include "NetworkManager.h"

int main() {
    NetworkManager manager;

    // Crear neuronas usando el factory
    auto lifNeuron1 = manager.createNeuron(NeuronType::LIF);
    auto lifNeuron2 = manager.createNeuron(NeuronType::LIF, {-65.0, -70.0, -55.0, 10.0, 1.0});

    auto izhiNeuron1 = manager.createNeuron(NeuronType::Izhikevich);
    auto izhiNeuron2 = manager.createNeuron(NeuronType::Izhikevich, {0.02, 0.2, -65.0, 8.0, -70.0, 0.0});

    // Conectar neuronas LIF entre sí
    manager.connectExcitatory(lifNeuron1, lifNeuron2, 1.0);  // Peso inicial bajo para observar cambios

    // Conectar neuronas Izhikevich entre sí
    manager.connectExcitatory(izhiNeuron1, izhiNeuron2, 1.0); // Peso inicial bajo

    // Conectar LIF a Izhikevich
    manager.connectExcitatory(lifNeuron1, izhiNeuron1, 1.0); // Peso inicial bajo

    // Conectar Izhikevich a LIF
    manager.connectExcitatory(izhiNeuron2, lifNeuron1, 1.0); // Peso inicial bajo

    // Conectar LIF a Izhikevich como inhibidora
    manager.connectInhibitory(lifNeuron2, izhiNeuron1, 1.0); // Peso inicial bajo

    // Conectar Izhikevich a LIF como inhibidora
    manager.connectInhibitory(izhiNeuron1, lifNeuron2, 1.0); // Peso inicial bajo

    // Inyectar corriente para inducir spikes
    lifNeuron1->injectCurrent(1015.0); // Forzar spike inmediato
    izhiNeuron2->injectCurrent(1015.0); // Forzar spike inmediato

    // Ejecutar la simulación durante 50 ms con un paso de tiempo de 0.1 ms
    manager.runSimulation(50.0, 0.1);

    // Imprimir los pesos finales de las sinapsis excitatorias
    std::cout << "Pesos finales de las sinapsis excitatorias:" << std::endl;
    for (auto &synapse : manager.getSynapses()) {
        auto excitatory = std::dynamic_pointer_cast<ExcitatorySynapse>(synapse);
        if (excitatory) {
            std::cout << "Sinapsis de Neuron " << excitatory->getPreNeuronID()
                      << " a Neuron " << excitatory->getPostNeuronID()
                      << " : Peso = " << excitatory->getWeight() << std::endl;
        }
    }

    return 0;
}
