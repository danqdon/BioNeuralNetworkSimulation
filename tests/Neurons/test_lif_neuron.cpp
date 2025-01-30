// tests/test_neuron.cpp
#include <gtest/gtest.h>
#include "Neurons/LIFNeuron.h"
#include "Neurons/IzhikevichNeuron.h"

// Verificar que una LIFNeuron dispara un spike con corriente suficiente
TEST(LIFNeuronTest, FiresWithEnoughCurrent) {
    auto lifNeuron = std::make_shared<BioNeuralNetwork::LIFNeuron>();
    lifNeuron->injectCurrent(1015.0);
    lifNeuron->stepSimulation(1.0, 0.0);
    EXPECT_TRUE(lifNeuron->hasFired());
}
