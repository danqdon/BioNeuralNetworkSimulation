// ScaleFreeConnectivityStrategy.h
#ifndef SCALEFREECONNECTIVITYSTRATEGY_H
#define SCALEFREECONNECTIVITYSTRATEGY_H

#include "IConnectivityStrategy.h"
#include <random>
#include <vector>

namespace BioNeuralNetwork {

/**
 * @brief Estrategia de conectividad Scale-Free utilizando el algoritmo de Barabási-Albert.
 */
class ScaleFreeConnectivityStrategy : public IConnectivityStrategy {
public:
    /**
     * @param initialNodes Número de nodos iniciales completamente conectados.
     * @param connectionsPerNewNode Número de conexiones nuevas por cada nodo que se añade.
     */
    ScaleFreeConnectivityStrategy(int initialNodes, int connectionsPerNewNode)
        : m_initialNodes(initialNodes), m_connectionsPerNewNode(connectionsPerNewNode),
          gen(rd()), dist(0.0, 1.0) {}

    void connectNeurons(std::vector<std::shared_ptr<INeuron>>& neurons, NetworkManager& manager) override;

private:
    int m_initialNodes;
    int m_connectionsPerNewNode;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dist;

    // Función auxiliar para seleccionar un nodo basado en preferencia
    int selectPreferentially(const std::vector<int>& degree, int totalDegree);
};

}

#endif // SCALEFREECONNECTIVITYSTRATEGY_H
