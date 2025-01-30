// src/IzhikevichNeuron.cpp
#include "IzhikevichNeuron.h"
#include <stdexcept>

// Inicialización del contador de IDs
int IzhikevichNeuron::id_counter = 0;

IzhikevichNeuron::IzhikevichNeuron(double a, double b, double c, double d,
                                   double V_threshold, double V_reset,
                                   double R, double C, double refractoryTime)
    : a(a), b(b), c(c), d(d),
      V_threshold(V_threshold), V_reset(V_reset),
      R(R), C(C), refractoryPeriod(refractoryTime),
      V(c), u(b * c),
      fired(false), accumulatedCurrent(0.0),
      timeSinceLastSpike(refractoryPeriod), lastSpikeTime(-1.0)
{
    id = ++id_counter;
}

double IzhikevichNeuron::stepSimulation(double dt, double currentTime)
{
    timeSinceLastSpike += dt;
    fired = false;

    if (timeSinceLastSpike < refractoryPeriod) {
        // No se actualiza el potencial si aún está en periodo refractario
        return V;
    }

    // Dinámica del modelo Izhikevich
    V += (0.04 * V * V + 5 * V + 140 - u + R * accumulatedCurrent) * dt;
    u += (a * (b * V - u)) * dt;

    if (V >= V_threshold) {
        fired = true;
        V = V_reset;
        u += d;
        timeSinceLastSpike = 0.0;
        lastSpikeTime = currentTime;
    }

    resetAccumulatedCurrent();
    return V;
}

double IzhikevichNeuron::getPotential() const
{
    return V;
}

bool IzhikevichNeuron::hasFired() const
{
    return fired;
}

double IzhikevichNeuron::getLastSpikeTime() const
{
    return lastSpikeTime;
}

void IzhikevichNeuron::injectCurrent(double current)
{
    accumulatedCurrent += current;
}

void IzhikevichNeuron::resetAccumulatedCurrent()
{
    accumulatedCurrent = 0.0;
}

void IzhikevichNeuron::addIncomingSynapse(std::shared_ptr<ISynapse> synapse)
{
    incomingSynapses.push_back(synapse);
}

void IzhikevichNeuron::addOutgoingSynapse(std::shared_ptr<ISynapse> synapse)
{
    outgoingSynapses.push_back(synapse);
}

int IzhikevichNeuron::getID() const
{
    return id;
}

const std::vector<std::shared_ptr<ISynapse>>& IzhikevichNeuron::getOutgoingSynapses() const
{
    return outgoingSynapses;
}

const std::vector<std::shared_ptr<ISynapse>>& IzhikevichNeuron::getIncomingSynapses() const
{
    return incomingSynapses;
}

void IzhikevichNeuron::setLastSpikeTime(double time)
{
    lastSpikeTime = time;
}
