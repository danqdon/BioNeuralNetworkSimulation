// tests/test_event_manager.cpp
#include <gtest/gtest.h>
#include "Core/EventManager.h"
#include "Core/SpikeEvent.h"
#include "Neurons/LIFNeuron.h"
#include "Synapses/ExcitatorySynapse.h"
namespace BioNeuralNetwork {
    // Verificar que los eventos se procesan en orden correcto
    TEST(EventManagerTest, EventProcessingOrder) {
        EventManager manager;
        auto neuron = std::make_shared<LIFNeuron>();
        auto synapse = std::make_shared<ExcitatorySynapse>(neuron, neuron);

        SpikeEvent event1(10.0, neuron, synapse);
        SpikeEvent event2(5.0, neuron, synapse);

        manager.pushEvent(event1);
        manager.pushEvent(event2);

        EXPECT_EQ(manager.popEvent().time, 5.0);
        EXPECT_EQ(manager.popEvent().time, 10.0);
    }
}