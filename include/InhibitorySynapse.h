// include/InhibitorySynapse.h
#ifndef INHIBITORYSYNAPSE_H
#define INHIBITORYSYNAPSE_H

#include "ISynapse.h"
#include "INeuron.h" // Incluir la definición completa de INeuron
#include <memory>
#include <cmath>
#include <stdexcept>

class InhibitorySynapse : public ISynapse {
public:
    InhibitorySynapse(std::shared_ptr<INeuron> pre, std::shared_ptr<INeuron> post,
                     double weight = 1.0, double delay = 1.0,
                     double A_plus = 0.1, double A_minus = -0.12,
                     double tau_plus = 20.0, double tau_minus = 20.0,
                     double min_weight = 0.0, double max_weight = 10.0);

    // Implementación de los métodos heredados
    void deliverSpikeFromPre(const std::shared_ptr<INeuron>& pre, double eventTime) override;
    void deliverSpikeFromPost(const std::shared_ptr<INeuron>& post, double eventTime) override;

    double getDelay() const override { return delay; }

    // Métodos adicionales para pruebas
    double getWeight() const { return weight; }
    int getPreNeuronID() const { return pre_neuron->getID(); }
    int getPostNeuronID() const { return post_neuron->getID(); }

private:
    std::shared_ptr<INeuron> pre_neuron;
    std::shared_ptr<INeuron> post_neuron;
    double weight;
    double delay;

    // Parámetros de STDP
    double A_plus;
    double A_minus;
    double tau_plus;
    double tau_minus;

    // Parámetros de Homeostasis
    double min_weight;
    double max_weight;

    // Método interno para ajustar el peso
    void adjustWeight(double delta_t);
};

#endif // INHIBITORYSYNAPSE_H
