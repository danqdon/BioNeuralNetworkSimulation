// src/Core/Logger.cpp
#include "Core/Logger.h"
#include <stdexcept>
#include <iostream>

namespace BioNeuralNetwork {

void Logger::logSpike(int neuronID, double time) {
    std::lock_guard<std::mutex> lock(mtx);
    spikeRecords.emplace_back(SpikeRecord{neuronID, time});
    if (realTimeLogging) {
        std::cout << "[LOG] Neurona " << neuronID << " disparó a " << std::fixed << std::setprecision(3) << time << " ms\n";
    }
}

void Logger::logWeightChange(const std::string& synapseID, double time, double oldWeight, double newWeight) {
    std::lock_guard<std::mutex> lock(mtx);
    weightChangeRecords.emplace_back(WeightChangeRecord{synapseID, time, oldWeight, newWeight});
    if (realTimeLogging) {
        std::cout << "[LOG] Sinapsis " << synapseID << ": Peso cambió de " << std::fixed << std::setprecision(3) << oldWeight
                  << " a " << newWeight << " en " << time << " ms\n";
    }
}

void Logger::exportSpikesToCSV(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo para escribir spikes CSV.");
    }

    // Escribir encabezados
    file << "NeuronID,Time(ms)\n";

    // Escribir registros
    for (const auto& spike : spikeRecords) {
        file << spike.neuronID << "," << std::fixed << std::setprecision(3) << spike.time << "\n";
    }

    file.close();
}

void Logger::exportWeightChangesToCSV(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo para escribir cambios de peso CSV.");
    }

    // Escribir encabezados
    file << "SynapseID,Time(ms),OldWeight,NewWeight\n";

    // Escribir registros
    for (const auto& change : weightChangeRecords) {
        file << change.synapseID << ","
             << std::fixed << std::setprecision(3) << change.time << ","
             << change.oldWeight << ","
             << change.newWeight << "\n";
    }

    file.close();
}

void Logger::exportLogsToText(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo para escribir logs de texto.");
    }

    // Escribir encabezados y registros de spikes
    file << "=== LOGS DE SPIKES ===\n";
    for (const auto& spike : spikeRecords) {
        file << "Neurona " << spike.neuronID << " disparó a " << std::fixed << std::setprecision(3) << spike.time << " ms\n";
    }

    // Escribir encabezados y registros de cambios de peso
    file << "\n=== LOGS DE CAMBIOS DE PESO ===\n";
    for (const auto& change : weightChangeRecords) {
        file << "Sinapsis " << change.synapseID << ": Peso cambió de " << std::fixed << std::setprecision(3) << change.oldWeight
             << " a " << change.newWeight << " en " << change.time << " ms\n";
    }

    file.close();
}

int Logger::getSpikeCount() const {
    std::lock_guard<std::mutex> lock(mtx);
    return spikeRecords.size();
}

int Logger::getWeightChangeCount() const {
    std::lock_guard<std::mutex> lock(mtx);
    return weightChangeRecords.size();
}

void Logger::setRealTimeLogging(bool enable) {
    std::lock_guard<std::mutex> lock(mtx);
    realTimeLogging = enable;
}

}
