// tests/test_network_manager.cpp
#include <gtest/gtest.h>
#include "NetworkManager.h"

// Verificar que se pueden crear neuronas en el NetworkManager
TEST(NetworkManagerTest, CreateNeurons) {
    NetworkManager manager;
    auto neuron = manager.createNeuron(NeuronType::LIF);
    EXPECT_NE(neuron, nullptr);
}
