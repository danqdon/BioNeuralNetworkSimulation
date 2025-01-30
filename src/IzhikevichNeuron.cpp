// src/IzhikevichNeuron.cpp
#include "IzhikevichNeuron.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

// Initialize the static ID counter
int IzhikevichNeuron::id_counter = 0;

IzhikevichNeuron::IzhikevichNeuron(double a, double b, double c, double d,
                                   double V_threshold, double V_reset,
                                   double R, double C, double refractoryTime)
    : a(a), b(b), c(c), d(d),
      V_threshold(V_threshold), V_reset(V_reset),
      R(R), C(C), refractoryPeriod(refractoryTime),
      V(c), u(b * c),
      fired(false), accumulatedCurrent(0.0),
      timeSinceLastSpike(refractoryPeriod),
      lastSpikeTime(-1.0)
{
    id = ++id_counter;
}

double IzhikevichNeuron::stepSimulation(double dt, double currentTime)
{
    // Subdivide dt into smaller steps to improve accuracy
    const double subdt = 0.1;           // Internal time step in ms
    int nSteps = static_cast<int>(std::ceil(dt / subdt));
    double actualSubdt = dt / nSteps;

    fired = false;

    for (int i = 0; i < nSteps; i++) {
        timeSinceLastSpike += actualSubdt;

        // If in refractory period, skip updating
        if (timeSinceLastSpike < refractoryPeriod) {
            continue;
        }

        // Euler integration for Izhikevich model
        double dv = (0.04 * V * V + 5.0 * V + 140.0 - u + R * accumulatedCurrent) * actualSubdt;
        V += dv;

        double du = (a * (b * V - u)) * actualSubdt;
        u += du;

        // Check for spike
        if (V >= V_threshold) {
            fired = true;
            V = V_reset;
            u += d; // Spike reset
            timeSinceLastSpike = 0.0;
            lastSpikeTime = currentTime + (i + 1) * actualSubdt; // Accurate spike time
            std::cout << "  ** Spike fired at time " << lastSpikeTime << " ms **" << std::endl;
            break; // Prevent multiple spikes in one step
        }
    }

    // Reset accumulated current after all sub-steps
    resetAccumulatedCurrent();

    // Return the final membrane potential
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

// Additional Getters
double IzhikevichNeuron::getRecovery() const
{
    return u;
}

double IzhikevichNeuron::getMembranePotential() const
{
    return V;
}

double IzhikevichNeuron::getResetPotential() const
{
    return V_reset;
}
