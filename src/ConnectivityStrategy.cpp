#include "RandomConnectivityStrategy.h"

void RandomConnectivityStrategy::connectNeurons(std::vector<std::shared_ptr<INeuron>>& neurons)
{
    if (neurons.empty()) return;

    // Generador aleatorio
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    // Para cada par de neuronas (i, j), decidimos si conectar
    for (size_t i = 0; i < neurons.size(); ++i) {
        for (size_t j = i + 1; j < neurons.size(); ++j) {
            double r = dist(gen); // numero aleatorio entre 0 y 1
            if (r < connectionProbability) {
                // Conectar la neurona i con j
                // Aquí asumimos que NetworkManager está disponible
                // de alguna manera, o que tenemos métodos estáticos.
                //
                // Si no, se puede delegar la conexión a una función libre
                // o a un callback que reciba la 'manager'.

                // Ejemplo (if excitatory):
                if (excitatory) {
                    // Conexión excitatoria i -> j
                    // Suponiendo que tienes un método global connectExcitatory
                    // o te pasen un puntero a NetworkManager para crear la sinapsis.
                    // connectExcitatory(neurons[i], neurons[j], defaultWeight);
                } else {
                    // Conexión inhibitoria i -> j
                    // connectInhibitory(neurons[i], neurons[j], defaultWeight);
                }
            }
        }
    }
}
