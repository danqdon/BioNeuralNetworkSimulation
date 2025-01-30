// src/InhibitorySynapse.cpp
#include "InhibitorySynapse.h"
#include "INeuron.h"
#include <iostream>

InhibitorySynapse::InhibitorySynapse(std::shared_ptr<INeuron> pre, std::shared_ptr<INeuron> post,
                                       double weight, double delay,
                                       double A_plus, double A_minus,
                                       double tau_plus, double tau_minus,
                                       double min_weight, double max_weight)
    : pre_neuron(pre), post_neuron(post),
      weight(weight), delay(delay),
      A_plus(A_plus), A_minus(A_minus),
      tau_plus(tau_plus), tau_minus(tau_minus),
      min_weight(min_weight), max_weight(max_weight)
{
    if (!pre_neuron || !post_neuron) {
        throw std::invalid_argument("Las neuronas pre y post no pueden ser nullptr");
    }
}

void InhibitorySynapse::deliverSpikeFromPre(const std::shared_ptr<INeuron>& pre, double eventTime)
{
    if (pre_neuron == pre && post_neuron) {
        // Inyectar corriente inhibidora en la neurona post
        post_neuron->injectCurrent(-weight); // Corriente negativa para inhibición

        // Obtener los tiempos de spikes
        double preTime  = pre->getLastSpikeTime();
        double postTime = post_neuron->getLastSpikeTime();

        // Calcular delta_t = postTime - preTime
        double delta_t = postTime - preTime;

        // Ajustar el peso sináptico
        adjustWeight(delta_t);
    }
}

void InhibitorySynapse::deliverSpikeFromPost(const std::shared_ptr<INeuron>& post, double eventTime)
{
    if (post_neuron == post && pre_neuron) {
        // Obtener los tiempos de spikes
        double preTime  = pre_neuron->getLastSpikeTime();
        double postTime = post->getLastSpikeTime();

        // Calcular delta_t = postTime - preTime
        double delta_t = postTime - preTime;

        // Ajustar el peso sináptico
        adjustWeight(delta_t);
    }
}

void InhibitorySynapse::adjustWeight(double delta_t)
{
    if (delta_t > 0) {
        // Potenciación (LTP)
        weight += A_plus * std::exp(-delta_t / tau_plus);
    }
    else if (delta_t < 0) {
        // Depresión (LTD)
        weight += A_minus * std::exp(delta_t / tau_minus);
    }
    // Si delta_t == 0, no hay cambio

    // Aplicar límites al peso
    if (weight > max_weight) {
        weight = max_weight;
    }
    if (weight < min_weight) {
        weight = min_weight;
    }
}
