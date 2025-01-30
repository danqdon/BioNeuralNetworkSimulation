// tests/test_neuron.cpp
#include <gtest/gtest.h>
#include "LIFNeuron.h"
#include "IzhikevichNeuron.h"

// Verificar que una LIFNeuron dispara un spike con corriente suficiente
TEST(LIFNeuronTest, FiresWithEnoughCurrent) {
    auto lifNeuron = std::make_shared<LIFNeuron>();
    lifNeuron->injectCurrent(1015.0);
    lifNeuron->stepSimulation(1.0, 0.0);
    EXPECT_TRUE(lifNeuron->hasFired());
}

// Verificar que una IzhikevichNeuron responde correctamente
TEST(IzhikevichNeuronTest, FiresWithEnoughCurrent) {
    auto izhiNeuron = std::make_shared<IzhikevichNeuron>();
    izhiNeuron->injectCurrent(1015.0);
    izhiNeuron->stepSimulation(1.0, 0.0);
    EXPECT_TRUE(izhiNeuron->hasFired());
}
