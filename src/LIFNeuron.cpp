// src/LIFNeuron.cpp
#include "LIFNeuron.h"
#include <cmath>
#include <stdexcept>

// Definición de la variable estática
int LIFNeuron::id_counter = 0;

// Implementación del constructor
LIFNeuron::LIFNeuron(double vRest, double vReset, double vThreshold,
                     double rMembrane, double cMembrane, double refractoryTime)
    : V_rest(vRest), V_reset(vReset), V_threshold(vThreshold),
      R(rMembrane), C(cMembrane), refractoryPeriod(refractoryTime),
      V_current(vRest), fired(false), accumulatedCurrent(0.0),
      timeSinceLastSpike(refractoryPeriod), lastSpikeTime(-1.0)
{
    id = ++id_counter;
}

// Implementación de stepSimulation
double LIFNeuron::stepSimulation(double dt, double currentTime) {
    timeSinceLastSpike += dt;
    fired = false;

    if (timeSinceLastSpike < refractoryPeriod) {
        // No se actualiza el potencial si aún está en periodo refractario
        return V_current;
    }

    // dV = (-(V_current - V_rest) + R * I) / (R * C)
    double dV = (-(V_current - V_rest) + R * accumulatedCurrent) / (R * C);
    V_current += dV * dt;

    if (V_current >= V_threshold) {
        fired = true;
        V_current = V_reset;
        timeSinceLastSpike = 0.0;
        lastSpikeTime = currentTime;

        // La transmisión de spikes ahora es manejada por NetworkManager
    }

    resetAccumulatedCurrent();
    return V_current;
}
