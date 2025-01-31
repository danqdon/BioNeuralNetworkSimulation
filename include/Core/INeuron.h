// include/Core/INeuron.h
#ifndef INEURON_H
#define INEURON_H

#include <memory>
#include <vector>
#include "ISynapse.h"

namespace BioNeuralNetwork {

    /**
     * @brief Interfaz para Neuronas
     */
    class INeuron {
    public:
        virtual ~INeuron() = default;

        virtual double stepSimulation(double dt, double currentTime) = 0;

        virtual double getPotential() const = 0;
        virtual bool hasFired() const = 0;
        virtual double getLastSpikeTime() const = 0;
        virtual void setLastSpikeTime(double time) = 0;

        virtual void injectCurrent(double current) = 0;
        virtual void resetAccumulatedCurrent() = 0;

        virtual void addIncomingSynapse(std::shared_ptr<ISynapse> synapse) = 0;
        virtual void addOutgoingSynapse(std::shared_ptr<ISynapse> synapse) = 0;

        virtual int getID() const = 0;
        virtual const std::vector<std::shared_ptr<ISynapse>>& getOutgoingSynapses() const = 0;
        virtual const std::vector<std::shared_ptr<ISynapse>>& getIncomingSynapses() const = 0;

        // Nuevas funciones virtuales añadidas para que LIFNeuron e IzhikevichNeuron puedan sobrescribirlas
        virtual double getRecovery() const = 0;
        virtual double getMembranePotential() const = 0;
        virtual double getResetPotential() const = 0;
    };

}

#endif // INEURON_H
