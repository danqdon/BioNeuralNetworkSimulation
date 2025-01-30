// src/Neurons/IzhikevichNeuron.cpp
#include "Neurons/IzhikevichNeuron.h"
#include "Core/Logger.h" // Incluir el Logger
#include <iostream>
#include <stdexcept>
#include <cmath>

namespace BioNeuralNetwork {

// Inicializar el contador estático
int IzhikevichNeuron::id_counter = 0;

IzhikevichNeuron::IzhikevichNeuron(double a_param,
                                   double b_param,
                                   double c_param,
                                   double d_param,
                                   double V_threshold_param,
                                   double V_reset_param,
                                   double R_param,
                                   double C_param,
                                   double refractoryTime_param)
    : a(a_param),
      b(b_param),
      c(c_param),
      d(d_param),
      V_threshold(V_threshold_param),
      V_reset(V_reset_param),
      R(R_param),
      C(C_param),
      refractoryPeriod(refractoryTime_param),
      V(c_param),
      u(b_param * c_param),
      fired(false),
      accumulatedCurrent(0.0),
      timeSinceLastSpike(refractoryTime_param),
      lastSpikeTime(-1.0)
{
    id = ++id_counter;
}

double IzhikevichNeuron::stepSimulation(double dt, double currentTime)
{
    // Subdividir dt para mayor precisión
    const double subdt = 0.1;
    int nSteps = static_cast<int>(std::ceil(dt / subdt));
    double actualSubdt = dt / nSteps;

    fired = false;

    for (int i = 0; i < nSteps; ++i) {
        timeSinceLastSpike += actualSubdt;

        if (timeSinceLastSpike < refractoryPeriod) {
            continue;
        }

        // Ecuaciones de Izhikevich (Euler simple)
        double dV = (0.04 * V * V + 5.0 * V + 140.0 - u + R * accumulatedCurrent) * actualSubdt;
        V += dV;

        double du = a * (b * V - u) * actualSubdt;
        u += du;

        // Revisión de disparo
        if (V >= V_threshold) {
            fired = true;
            V = V_reset;
            u += d;

            timeSinceLastSpike = 0.0;
            lastSpikeTime = currentTime + (i + 1) * actualSubdt;

            // Registrar el spike
            Logger::getInstance().logSpike(id, lastSpikeTime);
        }
    }

    resetAccumulatedCurrent();
    return V;
}

double IzhikevichNeuron::getPotential() const {
    return V;
}

bool IzhikevichNeuron::hasFired() const {
    return fired;
}

double IzhikevichNeuron::getLastSpikeTime() const {
    return lastSpikeTime;
}

void IzhikevichNeuron::setLastSpikeTime(double time) {
    lastSpikeTime = time;
}

void IzhikevichNeuron::injectCurrent(double current) {
    accumulatedCurrent += current;
}

void IzhikevichNeuron::resetAccumulatedCurrent() {
    accumulatedCurrent = 0.0;
}

void IzhikevichNeuron::addIncomingSynapse(std::shared_ptr<ISynapse> synapse) {
    incomingSynapses.push_back(synapse);
}

void IzhikevichNeuron::addOutgoingSynapse(std::shared_ptr<ISynapse> synapse) {
    outgoingSynapses.push_back(synapse);
}

int IzhikevichNeuron::getID() const {
    return id;
}

const std::vector<std::shared_ptr<ISynapse>>& IzhikevichNeuron::getOutgoingSynapses() const {
    return outgoingSynapses;
}

const std::vector<std::shared_ptr<ISynapse>>& IzhikevichNeuron::getIncomingSynapses() const {
    return incomingSynapses;
}

double IzhikevichNeuron::getRecovery() const {
    return u;
}

double IzhikevichNeuron::getMembranePotential() const {
    return V;
}

double IzhikevichNeuron::getResetPotential() const {
    return V_reset;
}

} // namespace BioNeuralNetwork
