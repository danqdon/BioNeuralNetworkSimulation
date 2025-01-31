// src/Neurons/LIFNeuron.cpp
#include "Neurons/LIFNeuron.h"
#include "Core/Logger.h" // Incluir el Logger
#include <cmath>

namespace BioNeuralNetwork {


int LIFNeuron::id_counter = 0;

LIFNeuron::LIFNeuron(double vRest,
                     double vReset,
                     double vThreshold,
                     double rMembrane,
                     double cMembrane,
                     double refractoryTime)
    : V_rest(vRest),
      V_reset(vReset),
      V_threshold(vThreshold),
      R(rMembrane),
      C(cMembrane),
      refractoryPeriod(refractoryTime),
      V_current(vRest),
      fired(false),
      accumulatedCurrent(0.0),
      timeSinceLastSpike(refractoryTime),
      lastSpikeTime(-1.0)
{
    id = ++id_counter;
}

double LIFNeuron::stepSimulation(double dt, double currentTime)
{
    timeSinceLastSpike += dt;
    fired = false;

    if (timeSinceLastSpike < refractoryPeriod) {
        return V_current;
    }

    double dV = (-(V_current - V_rest) + R * accumulatedCurrent) / (R * C);
    V_current += dV * dt;

    if (V_current >= V_threshold) {
        fired = true;
        V_current = V_reset;
        timeSinceLastSpike = 0.0;
        lastSpikeTime = currentTime;

        Logger::getInstance().logSpike(id, lastSpikeTime);
    }

    resetAccumulatedCurrent();
    return V_current;
}

double LIFNeuron::getPotential() const {
    return V_current;
}

bool LIFNeuron::hasFired() const {
    return fired;
}

double LIFNeuron::getLastSpikeTime() const {
    return lastSpikeTime;
}

void LIFNeuron::setLastSpikeTime(double time) {
    lastSpikeTime = time;
}

void LIFNeuron::injectCurrent(double current) {
    accumulatedCurrent += current;
}

void LIFNeuron::resetAccumulatedCurrent() {
    accumulatedCurrent = 0.0;
}

void LIFNeuron::addIncomingSynapse(std::shared_ptr<ISynapse> synapse) {
    incomingSynapses.push_back(synapse);
}

void LIFNeuron::addOutgoingSynapse(std::shared_ptr<ISynapse> synapse) {
    outgoingSynapses.push_back(synapse);
}

int LIFNeuron::getID() const {
    return id;
}

const std::vector<std::shared_ptr<ISynapse>>& LIFNeuron::getOutgoingSynapses() const {
    return outgoingSynapses;
}

const std::vector<std::shared_ptr<ISynapse>>& LIFNeuron::getIncomingSynapses() const {
    return incomingSynapses;
}

double LIFNeuron::getRecovery() const {
    return 0.0;
}

double LIFNeuron::getMembranePotential() const {
    return V_current;
}

double LIFNeuron::getResetPotential() const {
    return V_reset;
}

}
