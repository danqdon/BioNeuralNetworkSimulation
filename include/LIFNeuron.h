// include/LIFNeuron.h
#ifndef LIFNEURON_H
#define LIFNEURON_H

#include "INeuron.h"
#include "ISynapse.h"
#include <vector>
#include <memory>

// Clase que representa una neurona según el modelo LIF (Leaky Integrate-and-Fire)
class LIFNeuron : public INeuron, public std::enable_shared_from_this<LIFNeuron> {
public:
    LIFNeuron(double vRest = -65.0,
              double vReset = -70.0,
              double vThreshold = -50.0,
              double rMembrane = 10.0,
              double cMembrane = 1.0,
              double refractoryTime = 2.0);

    // Métodos heredados de INeuron
    double stepSimulation(double dt, double currentTime) override;
    double getPotential() const override { return V_current; }
    bool hasFired() const override { return fired; }
    double getLastSpikeTime() const override { return lastSpikeTime; }

    void injectCurrent(double current) override {
        accumulatedCurrent += current;
    }
    void resetAccumulatedCurrent() override {
        accumulatedCurrent = 0.0;
    }

    void addIncomingSynapse(std::shared_ptr<ISynapse> synapse) override {
        incomingSynapses.push_back(synapse);
    }
    void addOutgoingSynapse(std::shared_ptr<ISynapse> synapse) override {
        outgoingSynapses.push_back(synapse);
    }

    int getID() const override { return id; }

    // Obtener las sinapsis salientes
    const std::vector<std::shared_ptr<ISynapse>>& getOutgoingSynapses() const override {
        return outgoingSynapses;
    }

    // Obtener las sinapsis entrantes
    const std::vector<std::shared_ptr<ISynapse>>& getIncomingSynapses() const override {
        return incomingSynapses;
    }

    // Método para setear el tiempo del último spike
    void setLastSpikeTime(double time) override {
        lastSpikeTime = time;
    }

private:
    double V_rest;
    double V_reset;
    double V_threshold;
    double R;
    double C;
    double refractoryPeriod;

    double V_current;
    bool fired;
    double accumulatedCurrent;

    double timeSinceLastSpike;
    double lastSpikeTime;

    std::vector<std::shared_ptr<ISynapse>> incomingSynapses;
    std::vector<std::shared_ptr<ISynapse>> outgoingSynapses;

    int id;
    static int id_counter;
};

#endif // LIFNEURON_H
