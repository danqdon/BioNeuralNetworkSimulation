// include/Neurons/LIFNeuron.h
#ifndef LIFNEURON_H
#define LIFNEURON_H

#include <memory>
#include <vector>
#include "Core/INeuron.h"
#include "Core/ISynapse.h"

namespace BioNeuralNetwork {

    /**
     * @brief Neurona de Integrate-and-Fire (LIF)
     */
    class LIFNeuron : public INeuron {
    public:
        LIFNeuron(double vRest = -65.0,
                  double vReset = -65.0,
                  double vThreshold = -50.0,
                  double rMembrane = 10.0,
                  double cMembrane = 100.0,
                  double refractoryTime = 2.0);

        // Sobrescribir funciones virtuales de INeuron
        double stepSimulation(double dt, double currentTime) override;

        double getPotential() const override;
        bool hasFired() const override;
        double getLastSpikeTime() const override;
        void setLastSpikeTime(double time) override;

        void injectCurrent(double current) override;
        void resetAccumulatedCurrent() override;

        void addIncomingSynapse(std::shared_ptr<ISynapse> synapse) override;
        void addOutgoingSynapse(std::shared_ptr<ISynapse> synapse) override;

        int getID() const override;
        const std::vector<std::shared_ptr<ISynapse>>& getOutgoingSynapses() const override;
        const std::vector<std::shared_ptr<ISynapse>>& getIncomingSynapses() const override;

        double getRecovery() const override;
        double getMembranePotential() const override;
        double getResetPotential() const override;

    private:
        static int id_counter;
        int id;

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
    };

}

#endif // LIFNEURON_H
