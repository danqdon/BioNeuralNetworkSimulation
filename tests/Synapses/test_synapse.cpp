// tests/test_synapse.cpp
#include <gtest/gtest.h>
#include "Neurons/LIFNeuron.h"
#include "Synapses/ExcitatorySynapse.h"

namespace BioNeuralNetwork {
    TEST(ExcitatorySynapseTest, TransmitSpike) {
        auto preNeuron = std::make_shared<LIFNeuron>();
        auto postNeuron = std::make_shared<LIFNeuron>();
        auto synapse = std::make_shared<ExcitatorySynapse>(preNeuron, postNeuron, 1.0, 1.0);

        // Registrar el potencial inicial de la neurona post
        double initialPotential = postNeuron->getPotential();

        // Simular un spike en la neurona pre
        preNeuron->injectCurrent(1015.0); // Forzar spike
        preNeuron->stepSimulation(1.0, 0.0); // Simular en t=0 ms

        // Entregar el spike a través de la sinapsis
        synapse->deliverSpikeFromPre(preNeuron, 1.0); // Spike llega a t=1 ms

        // Simular el paso de tiempo para que la neurona post procese la inyección
        postNeuron->stepSimulation(1.0, 2.0); // Simular en t=2 ms

        // Registrar el potencial después de la inyección
        double newPotential = postNeuron->getPotential();

        // Verificar que el potencial ha aumentado
        EXPECT_GT(newPotential, initialPotential);
    }

}
