#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <queue>
#include <vector>
#include <stdexcept>
#include "Core/SpikeEvent.h"

namespace BioNeuralNetwork {

    /**
     * @brief Clase que gestiona la cola de eventos de spike ordenados por tiempo.
     */
    class EventManager {
    public:
        void pushEvent(const SpikeEvent &event) {
            eventQueue.push(event);
        }

        bool empty() const {
            return eventQueue.empty();
        }


        SpikeEvent popEvent() {
            if (eventQueue.empty()) {
                throw std::runtime_error("No hay eventos en la cola.");
            }
            SpikeEvent e = eventQueue.top();
            eventQueue.pop();
            return e;
        }

        double nextEventTime() const {
            if (eventQueue.empty()) {
                return 1e20; // Indica que no hay eventos pendientes
            }
            return eventQueue.top().time;
        }

    private:
        struct CompareEvent {
            bool operator()(const SpikeEvent &e1, const SpikeEvent &e2) {
                return e1.time > e2.time;
            }
        };

        std::priority_queue<SpikeEvent, std::vector<SpikeEvent>, CompareEvent> eventQueue;
    };

}

#endif // EVENTMANAGER_H
