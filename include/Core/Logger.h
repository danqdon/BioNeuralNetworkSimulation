// include/Core/Logger.h
#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <string>
#include <mutex>
#include <fstream>
#include <iomanip>
#include <iostream>

namespace BioNeuralNetwork {

struct SpikeRecord {
    int neuronID;
    double time; // en ms
};

struct WeightChangeRecord {
    std::string synapseID; // Formato "preID_postID"
    double time;           // en ms
    double oldWeight;
    double newWeight;
};

/**
 * @brief Clase Logger para registrar eventos de spikes y cambios de peso.
 */
class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void logSpike(int neuronID, double time);
    void logWeightChange(const std::string& synapseID, double time, double oldWeight, double newWeight);

    void exportSpikesToCSV(const std::string& filename);
    void exportWeightChangesToCSV(const std::string& filename);

    void exportLogsToText(const std::string& filename);

    int getSpikeCount() const;
    int getWeightChangeCount() const;

    // Método para habilitar/deshabilitar el logging en tiempo real
    void setRealTimeLogging(bool enable);

private:
    // Constructor privado para Singleton
    Logger() : realTimeLogging(false) {}

    // Evitar copia y asignación
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Almacenamiento de registros
    std::vector<SpikeRecord> spikeRecords;
    std::vector<WeightChangeRecord> weightChangeRecords;

    // Mutex para seguridad en hilos
    mutable std::mutex mtx;

    // Flag para logging en tiempo real
    bool realTimeLogging;
};

} // namespace BioNeuralNetwork

#endif // LOGGER_H
