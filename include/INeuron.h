// include/INeuron.h
#ifndef INEURON_H
#define INEURON_H

#include <memory>
#include <vector>

// Declaración adelantada de ISynapse
class ISynapse;

// Clase abstracta que define la interfaz de una neurona
class INeuron {
public:
    virtual ~INeuron() = default;

    // Ejecuta un paso de simulación y retorna el potencial de membrana actual
    virtual double stepSimulation(double dt, double currentTime) = 0;

    // Obtener el potencial de membrana actual
    virtual double getPotential() const = 0;

    // Verificar si la neurona ha disparado un spike en el último paso
    virtual bool hasFired() const = 0;

    // Obtener el tiempo del último spike
    virtual double getLastSpikeTime() const = 0;

    // Inyectar corriente en la neurona
    virtual void injectCurrent(double current) = 0;

    // Resetear la corriente acumulada después de cada paso de simulación
    virtual void resetAccumulatedCurrent() = 0;

    // Añadir sinapsis entrante y saliente
    virtual void addIncomingSynapse(std::shared_ptr<ISynapse> synapse) = 0;
    virtual void addOutgoingSynapse(std::shared_ptr<ISynapse> synapse) = 0;

    // Obtener el ID de la neurona
    virtual int getID() const = 0;

    // Obtener las sinapsis salientes
    virtual const std::vector<std::shared_ptr<ISynapse>>& getOutgoingSynapses() const = 0;

    // Obtener las sinapsis entrantes (Añadido)
    virtual const std::vector<std::shared_ptr<ISynapse>>& getIncomingSynapses() const = 0;

    // Añadir método para setear el tiempo del último spike (para NetworkManager)
    virtual void setLastSpikeTime(double time) = 0;
};

#endif // INEURON_H
