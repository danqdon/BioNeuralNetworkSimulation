// ScaleFreeConnectivityStrategy.cpp
#include "ConnectivityStrategies/ScaleFreeConnectivityStrategy.h"
#include "Network/NetworkManager.h"
#include <algorithm>
#include <stdexcept>
#include <random>

namespace BioNeuralNetwork {

int ScaleFreeConnectivityStrategy::selectPreferentially(const std::vector<int>& degree, int totalDegree) {
    double r = dist(gen) * totalDegree;
    double cumulative = 0.0;
    for (size_t i = 0; i < degree.size(); ++i) {
        cumulative += degree[i];
        if (cumulative >= r) {
            return static_cast<int>(i);
        }
    }
    return static_cast<int>(degree.size() - 1); // Fallback
}

void ScaleFreeConnectivityStrategy::connectNeurons(std::vector<std::shared_ptr<INeuron>>& neurons, NetworkManager& manager) {
    int numNeurons = static_cast<int>(neurons.size());

    if (m_initialNodes >= numNeurons) {
        throw std::invalid_argument("El número de nodos iniciales debe ser menor que el total de neuronas.");
    }

    // Inicializar la red completa con los nodos iniciales
    for (int i = 0; i < m_initialNodes; ++i) {
        for (int j = i + 1; j < m_initialNodes; ++j) {
            manager.connectExcitatory(neurons[i], neurons[j]);
            manager.connectExcitatory(neurons[j], neurons[i]); // Bidireccional
        }
    }

    // Mantener un vector que representa el "grado" de cada nodo para el algoritmo de preferencia
    std::vector<int> degree(numNeurons, 0);
    for (int i = 0; i < m_initialNodes; ++i) {
        degree[i] = m_initialNodes - 1;
    }

    for (int i = m_initialNodes; i < numNeurons; ++i) {
        std::vector<int> possibleTargets;
        for (int j = 0; j < i; ++j) {
            if (neurons[j] != neurons[i]) { // Evitar auto-conexiones
                possibleTargets.push_back(j);
            }
        }

        // Seleccionar 'm_connectionsPerNewNode' conexiones preferenciales
        int connectionsMade = 0;
        while (connectionsMade < m_connectionsPerNewNode && !possibleTargets.empty()) {
            int totalDegree = 0;
            for (int j : possibleTargets) {
                totalDegree += degree[j];
            }

            if (totalDegree == 0) break; // Evitar división por cero

            int selected = selectPreferentially(degree, totalDegree);
            if (selected >= possibleTargets.size()) selected = static_cast<int>(possibleTargets.size() - 1);

            int target = possibleTargets[selected];
            manager.connectExcitatory(neurons[i], neurons[target]);
            manager.connectExcitatory(neurons[target], neurons[i]); // Bidireccional

            // Actualizar grados
            degree[i]++;
            degree[target]++;

            // Remover el target seleccionado para evitar duplicados
            possibleTargets.erase(possibleTargets.begin() + selected);
            connectionsMade++;
        }
    }
}

} // namespace BioNeuralNetwork
