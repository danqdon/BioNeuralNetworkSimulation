// src/Synapses/InhibitorySynapse.cpp
#include "Synapses/InhibitorySynapse.h"
#include "Core/Logger.h" // Incluir el Logger
#include <sstream>
#include <cmath>

namespace BioNeuralNetwork {

InhibitorySynapse::InhibitorySynapse(std::shared_ptr<INeuron> pre,
                                     std::shared_ptr<INeuron> post,
                                     double weight,
                                     double delay,
                                     double A_plus,
                                     double A_minus,
                                     double tau_plus,
                                     double tau_minus,
                                     double min_weight,
                                     double max_weight)
    : pre_neuron(pre),
      post_neuron(post),
      weight(weight),
      delay(delay),
      A_plus(A_plus),
      A_minus(A_minus),
      tau_plus(tau_plus),
      tau_minus(tau_minus),
      min_weight(min_weight),
      max_weight(max_weight)
{
    if (!pre_neuron || !post_neuron) {
        throw std::invalid_argument("Neurona pre o post nula en InhibitorySynapse.");
    }
}

void InhibitorySynapse::deliverSpikeFromPre(const std::shared_ptr<INeuron>& pre, double eventTime)
{
    if (pre == pre_neuron) {
        // Inyectar corriente inhibidora (negativa)
        post_neuron->injectCurrent(-weight);

        // STDP
        double preTime = pre_neuron->getLastSpikeTime();
        double postTime = post_neuron->getLastSpikeTime();
        double delta_t = postTime - preTime;

        adjustWeight(delta_t, eventTime); // Pasar eventTime
    }
}

void InhibitorySynapse::deliverSpikeFromPost(const std::shared_ptr<INeuron>& post, double eventTime)
{
    if (post == post_neuron) {
        // STDP
        double preTime = pre_neuron->getLastSpikeTime();
        double postTime = post_neuron->getLastSpikeTime();
        double delta_t = postTime - preTime;

        adjustWeight(delta_t, eventTime); // Pasar eventTime
    }
}

void InhibitorySynapse::adjustWeight(double delta_t, double eventTime) // Añadido eventTime
{
    if (std::isnan(delta_t)) {
        return;
    }

    double oldWeight = weight; // Guardar el peso antes del ajuste

    if (delta_t > 0) {
        // LTP
        weight += A_plus * std::exp(-delta_t / tau_plus);
    }
    else if (delta_t < 0) {
        // LTD
        weight += A_minus * std::exp(delta_t / tau_minus);
    }

    // Aplicar límites al peso
    if (weight < min_weight) {
        weight = min_weight;
    }
    else if (weight > max_weight) {
        weight = max_weight;
    }

    // Registrar el cambio de peso si hubo modificación significativa
    if (std::abs(weight - oldWeight) > 1e-6) { // Evitar registros innecesarios
        std::ostringstream synID;
        synID << pre_neuron->getID() << "_" << post_neuron->getID();
        Logger::getInstance().logWeightChange(synID.str(), eventTime, oldWeight, weight);
    }
}

} // namespace BioNeuralNetwork
