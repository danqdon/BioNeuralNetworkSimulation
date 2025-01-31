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

        virtual void deliverSpikeFromPre(const std::shared_ptr<INeuron>& pre, double eventTime) = 0;
        virtual void deliverSpikeFromPost(const std::shared_ptr<INeuron>& post, double eventTime) = 0;

        virtual double getDelay() const = 0;
    };

}

#endif // ISYNAPSE_H
