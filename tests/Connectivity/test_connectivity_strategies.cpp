// tests/Connectivity/test_connectivity_strategies.cpp

#include <gtest/gtest.h>
#include "ConnectivityStrategies/RandomConnectivityStrategy.h"
#include "ConnectivityStrategies/SmallWorldConnectivityStrategy.h"
#include "ConnectivityStrategies/ScaleFreeConnectivityStrategy.h"
#include "Neurons/LIFNeuron.h"
#include "Network/NetworkManager.h"
#include <memory>
#include <vector>

using namespace BioNeuralNetwork;

TEST(ConnectivityStrategyTest, RandomConnectivity) {
    int numNeurons = 20;
    std::vector<std::shared_ptr<INeuron>> neurons;
    for (int i = 0; i < numNeurons; ++i) {
         neurons.push_back(std::make_shared<LIFNeuron>());
    }
    NetworkManager manager;
    double p = 0.1;
    auto strategy = std::make_unique<RandomConnectivityStrategy>(p, 1.0, true);
    strategy->connectNeurons(neurons, manager);

    int totalConnections = 0;
    for (const auto &neuron: neurons) {
         totalConnections += neuron->getOutgoingSynapses().size();
    }
    // Se espera que existan conexiones, pero menos que el número máximo posible.
    EXPECT_GT(totalConnections, 0);
    EXPECT_LT(totalConnections, numNeurons * (numNeurons - 1));
}

TEST(ConnectivityStrategyTest, SmallWorldConnectivity) {
    int numNeurons = 20;
    std::vector<std::shared_ptr<INeuron>> neurons;
    for (int i = 0; i < numNeurons; ++i) {
         neurons.push_back(std::make_shared<LIFNeuron>());
    }
    NetworkManager manager;
    int k = 4; // Número de conexiones por neurona (valor par)
    double p_rewire = 0.0; // Sin reconexión para comportamiento determinista
    auto strategy = std::make_unique<SmallWorldConnectivityStrategy>(p_rewire, k);
    strategy->connectNeurons(neurons, manager);

    // En el primer bucle, cada neurona actúa como fuente en k/2 conexiones.
    for (const auto &neuron: neurons) {
         EXPECT_GE(neuron->getOutgoingSynapses().size(), static_cast<size_t>(k/2));
    }
}

TEST(ConnectivityStrategyTest, ScaleFreeConnectivity) {
    int numNeurons = 10;
    int initialNodes = 3;
    int connectionsPerNewNode = 2;
    std::vector<std::shared_ptr<INeuron>> neurons;
    for (int i = 0; i < numNeurons; ++i) {
         neurons.push_back(std::make_shared<LIFNeuron>());
    }
    NetworkManager manager;
    auto strategy = std::make_unique<ScaleFreeConnectivityStrategy>(initialNodes, connectionsPerNewNode);
    strategy->connectNeurons(neurons, manager);

    // Para los nodos iniciales, deben estar completamente conectados entre sí.
    for (int i = 0; i < initialNodes; ++i) {
         EXPECT_GE(neurons[i]->getOutgoingSynapses().size(), static_cast<size_t>(initialNodes - 1));
    }
    // Para los nodos nuevos, se espera que tengan al menos 'connectionsPerNewNode' conexiones salientes.
    for (int i = initialNodes; i < numNeurons; ++i) {
         EXPECT_GE(neurons[i]->getOutgoingSynapses().size(), static_cast<size_t>(connectionsPerNewNode));
    }
}
