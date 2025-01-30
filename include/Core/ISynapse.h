#ifndef ISYNAPSE_H
#define ISYNAPSE_H

#include <memory>

namespace BioNeuralNetwork {

    class INeuron;

    /**
     * @brief Interfaz para las sinapsis
     */
    class ISynapse {
    public:
        virtual ~ISynapse() = default;

        // Métodos para manejar spikes desde la neurona pre y post-sináptica
        virtual void deliverSpikeFromPre(const std::shared_ptr<INeuron>& pre, double eventTime) = 0;
        virtual void deliverSpikeFromPost(const std::shared_ptr<INeuron>& post, double eventTime) = 0;

        // Obtener el retraso de la sinapsis
        virtual double getDelay() const = 0;
    };

} // namespace BioNeuralNetwork

#endif // ISYNAPSE_H
