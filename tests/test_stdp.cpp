// tests/test_stdp.cpp
#include <gtest/gtest.h>
#include "ExcitatorySynapse.h"
#include "LIFNeuron.h"

TEST(STDPTest, LongTermPotentiation) {
    // Crear neuronas pre y post
    auto preNeuron = std::make_shared<LIFNeuron>();
    auto postNeuron = std::make_shared<LIFNeuron>();

    // Crear una sinapsis excitatoria con peso inicial 1.0
    auto synapse = std::make_shared<ExcitatorySynapse>(preNeuron, postNeuron, 1.0, 1.0);

    // Simular un spike en la neurona pre
    preNeuron->injectCurrent(1015.0); // Forzar spike
    preNeuron->stepSimulation(1.0, 10.0); // Simular en t=10 ms

    // Entregar el spike a través de la sinapsis
    synapse->deliverSpikeFromPre(preNeuron, 11.0); // Spike llega a t=11 ms (delay=1 ms)

    // Simular un spike en la neurona post después del spike pre
    postNeuron->injectCurrent(1015.0); // Forzar spike
    postNeuron->stepSimulation(1.0, 15.0); // Simular en t=15 ms

    // Notificar a las sinapsis entrantes sobre el spike post para ajustar el peso
    synapse->deliverSpikeFromPost(postNeuron, 15.0); // Spike post en t=15 ms

    // Verificar que el peso ha aumentado (Potenciación)
    EXPECT_GT(synapse->getWeight(), 1.0);
}

TEST(STDPTest, LongTermDepression) {
    // Crear neuronas pre y post
    auto preNeuron = std::make_shared<LIFNeuron>();
    auto postNeuron = std::make_shared<LIFNeuron>();

    // Crear una sinapsis excitatoria con peso inicial 1.0
    auto synapse = std::make_shared<ExcitatorySynapse>(preNeuron, postNeuron, 1.0, 1.0);

    // Simular un spike en la neurona post primero
    postNeuron->injectCurrent(1015.0); // Forzar spike
    postNeuron->stepSimulation(1.0, 5.0); // Simular en t=5 ms

    // Simular un spike en la neurona pre después del spike post
    preNeuron->injectCurrent(1015.0); // Forzar spike
    preNeuron->stepSimulation(1.0, 10.0); // Simular en t=10 ms

    // Entregar el spike a través de la sinapsis
    synapse->deliverSpikeFromPre(preNeuron, 11.0); // Spike llega a t=11 ms (delay=1 ms)

    // Notificar a las sinapsis entrantes sobre el spike post para ajustar el peso
    synapse->deliverSpikeFromPost(postNeuron, 5.0); // Spike post en t=5 ms

    // Verificar que el peso ha disminuido (Depresión)
    EXPECT_LT(synapse->getWeight(), 1.0);
}
