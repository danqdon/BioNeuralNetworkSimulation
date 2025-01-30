// include/EventManager.h
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <queue>
#include <vector>
#include "SpikeEvent.h"

// Clase que gestiona la cola de eventos de spike ordenados por tiempo
class EventManager {
public:
    // Añade un nuevo evento a la cola
    void pushEvent(const SpikeEvent &event) {
        eventQueue.push(event);
    }

    // Verifica si la cola de eventos está vacía
    bool empty() const {
        return eventQueue.empty();
    }

    // Obtiene y elimina el siguiente evento cronológicamente
    SpikeEvent popEvent() {
        if (eventQueue.empty()) {
            throw std::runtime_error("No hay eventos en la cola.");
        }
        SpikeEvent e = eventQueue.top();
        eventQueue.pop();
        return e;
    }

    // Devuelve el tiempo del próximo evento (o un valor muy grande si no hay eventos)
    double nextEventTime() const {
        if (eventQueue.empty()) {
            return 1e20; // Indica que no hay eventos pendientes
        }
        return eventQueue.top().time;
    }

private:
    // Comparador para la cola de prioridad: eventos con menor 'time' tienen mayor prioridad
    struct CompareEvent {
        bool operator()(const SpikeEvent &e1, const SpikeEvent &e2) {
            return e1.time > e2.time;
        }
    };

    std::priority_queue<SpikeEvent, std::vector<SpikeEvent>, CompareEvent> eventQueue;
};

#endif // EVENTMANAGER_H
