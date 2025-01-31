// include/Synapses/InhibitorySynapse.h
#ifndef INHIBITORYSYNAPSE_H
#define INHIBITORYSYNAPSE_H

#include <memory>
#include <cmath>
#include <stdexcept>
#include <sstream>

#include "Core/ISynapse.h"
#include "Core/INeuron.h"

namespace BioNeuralNetwork {

    /**
     * @brief Sinapsis Inhibitoria con posible STDP
     */
    class InhibitorySynapse : public ISynapse {
    public:
        InhibitorySynapse(std::shared_ptr<INeuron> pre,
                          std::shared_ptr<INeuron> post,
                          double weight = 1.0,
                          double delay = 1.0,
                          double A_plus = 0.1,
                          double A_minus = -0.12,
                          double tau_plus = 20.0,
                          double tau_minus = 20.0,
                          double min_weight = 0.0,
                          double max_weight = 10.0);

        void deliverSpikeFromPre(const std::shared_ptr<INeuron>& pre, double eventTime) override;
        void deliverSpikeFromPost(const std::shared_ptr<INeuron>& post, double eventTime) override;

        double getDelay() const override { return delay; }

        double getWeight() const { return weight; }

    private:
        // Actualizar la firma para incluir eventTime
        void adjustWeight(double delta_t, double eventTime);

        std::shared_ptr<INeuron> pre_neuron;
        std::shared_ptr<INeuron> post_neuron;
        double weight;
        double delay;

        // STDP
        double A_plus;
        double A_minus;
        double tau_plus;
        double tau_minus;

        // Homeostasis
        double min_weight;
        double max_weight;
    };

}

#endif // INHIBITORYSYNAPSE_H
