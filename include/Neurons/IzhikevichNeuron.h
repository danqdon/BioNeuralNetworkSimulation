// include/Neurons/IzhikevichNeuron.h
#ifndef IZHIKEVICHNEURON_H
#define IZHIKEVICHNEURON_H

#include <memory>
#include <vector>
#include "Core/INeuron.h"
#include "Core/ISynapse.h"

namespace BioNeuralNetwork {

/**
 * @brief Modelo de Neurona de Izhikevich
 */
class IzhikevichNeuron : public INeuron {
public:
    IzhikevichNeuron(double a = 0.02,
                     double b = 0.2,
                     double c = -65.0,
                     double d = 8.0,
                     double V_threshold = 30.0,
                     double V_reset = -65.0,
                     double R = 1.0,
                     double C = 1.0,
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

    double a;
    double b;
    double c;
    double d;
    double V_threshold;
    double V_reset;
    double R;
    double C;
    double refractoryPeriod;

    double V;
    double u;
    bool fired;
    double accumulatedCurrent;
    double timeSinceLastSpike;
    double lastSpikeTime;

    std::vector<std::shared_ptr<ISynapse>> incomingSynapses;
    std::vector<std::shared_ptr<ISynapse>> outgoingSynapses;
};

}

#endif // IZHIKEVICHNEURON_H
