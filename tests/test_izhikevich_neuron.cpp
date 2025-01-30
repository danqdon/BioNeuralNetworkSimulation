// tests/test_izhikevich_neuron.cpp

#include "gtest/gtest.h"
#include "IzhikevichNeuron.h"  // Ensure the correct path
#include <memory>
#include <iostream>
#include <cmath>

// Helper function to calculate absolute difference
double abs_diff(double a, double b) {
    return std::abs(a - b);
}

TEST(IzhikevichNeuronTest, RecoveryVariableAccumulation) {
    auto izhiNeuron = std::make_shared<IzhikevichNeuron>(
        0.02, 0.2, -65.0, 8.0, 30.0, -65.0  // standard RS parameters
    );
    double initialRecovery = izhiNeuron->getRecovery(); // Typically u = b * V_reset = 0.2 * (-65) = -13

    izhiNeuron->injectCurrent(600.0);  // Moderate current
    for (int i = 0; i < 10; i++) {
        izhiNeuron->stepSimulation(1.0, i);
        std::cout << "Step " << i+1 << " | u: " << izhiNeuron->getRecovery() << std::endl;
    }

    double finalRecovery = izhiNeuron->getRecovery();
    // Check if the absolute difference is at least 5.0
    EXPECT_GE(abs_diff(finalRecovery, initialRecovery), 5.0) <<
        "Recovery variable didn't change enough (change was " << (finalRecovery - initialRecovery) << ").";
}

TEST(IzhikevichNeuronTest, RecoveryVariablePostFiring) {
    auto izhiNeuron = std::make_shared<IzhikevichNeuron>(
        0.02, 0.2, -65.0, 8.0, 30.0, -65.0
    );
    izhiNeuron->injectCurrent(1015.0);  // Enough to fire

    izhiNeuron->injectCurrent(1015.0); // Ensure current is injected each step
    izhiNeuron->stepSimulation(1.0, 0.0);
    if (izhiNeuron->hasFired()) {
        double expectedU = (0.2 * izhiNeuron->getResetPotential()) + 8.0; // b*c + d
        double actualU   = izhiNeuron->getRecovery();
        EXPECT_NEAR(actualU, expectedU, 0.5) <<
            "Recovery variable after firing isn't close to b*c + d within tolerance.";
    } else {
        FAIL() << "The neuron did not fire when enough current was injected.";
    }
}

TEST(IzhikevichNeuronTest, BurstFiring) {
    // Known “bursting” parameters (e.g., chattering)
    auto izhiNeuron = std::make_shared<IzhikevichNeuron>(
        0.02, 0.2, -50.0, 2.0, 30.0, -50.0
    );
    double injectedCurrent = 1000.0; // High enough to provoke bursting

    int spikeCount = 0;
    for (int i = 0; i < 50; i++) { // Simulate 50 ms
        izhiNeuron->injectCurrent(injectedCurrent); // Inject current at each step
        izhiNeuron->stepSimulation(1.0, i);
        if (izhiNeuron->hasFired()) {
            spikeCount++;
            std::cout << "Spike fired at time " << i << " ms" << std::endl;
        }
    }

    // Expect more than 1 spike if truly bursting
    EXPECT_GT(spikeCount, 1)
        << "Expected bursting behavior (multiple spikes), got only " << spikeCount;
}

TEST(IzhikevichNeuronTest, ThresholdAndReset) {
    auto izhiNeuron = std::make_shared<IzhikevichNeuron>(
        0.02, 0.2, -65.0, 8.0, 30.0, -65.0
    );
    double injectedCurrent = 1015.0; // High enough to fire

    izhiNeuron->injectCurrent(injectedCurrent); // Inject current at each step
    izhiNeuron->stepSimulation(1.0, 0.0);

    if (izhiNeuron->hasFired()) {
        // The potential should be near reset
        EXPECT_NEAR(izhiNeuron->getMembranePotential(),
                    izhiNeuron->getResetPotential(), 1e-3);

        // The sub-step logic might give lastSpikeTime ~0.1
        EXPECT_NEAR(izhiNeuron->getLastSpikeTime(), 0.0, 0.2)
            << "Spike time differs from 0.0 by more than 0.2 ms (sub-step).";
    } else {
        FAIL() << "Neuron did not fire when expected with high current.";
    }
}

// tests/test_izhikevich_neuron.cpp

#include "gtest/gtest.h"
#include "IzhikevichNeuron.h"  // Ensure the correct path
#include <memory>
#include <iostream>
#include <cmath>


TEST(IzhikevichNeuronTest, DifferentFiringPatterns) {
    // Regular spiking neuron with longer refractory period
    auto regularSpikingNeuron = std::make_shared<IzhikevichNeuron>(
        0.02,   // a
        0.2,    // b
        -65.0,  // c
        8.0,    // d
        30.0,   // V_threshold
        -65.0,  // V_reset
        1.0,    // R
        1.0,    // C
        2.0     // refractoryTime
    );

    // Fast spiking neuron with shorter refractory period
    auto fastSpikingNeuron = std::make_shared<IzhikevichNeuron>(
        0.1,    // a
        0.2,    // b
        -65.0,  // c
        2.0,    // d
        30.0,   // V_threshold
        -65.0,  // V_reset
        1.0,    // R
        1.0,    // C
        1.0     // refractoryTime
    );

    double regularCurrent = 800.0; // Sufficient to produce regular spikes
    double fastCurrent = 800.0;    // Same current for comparison

    int regularSpikes = 0, fastSpikes = 0;

    // Simulate 100 ms
    for (int i = 0; i < 100; i++) {
        // Inject current at each step for sustained input
        regularSpikingNeuron->injectCurrent(regularCurrent);
        fastSpikingNeuron->injectCurrent(fastCurrent);

        regularSpikingNeuron->stepSimulation(1.0, i);
        fastSpikingNeuron->stepSimulation(1.0, i);

        if (regularSpikingNeuron->hasFired()) {
            regularSpikes++;
            std::cout << "Regular neuron spike at " << i << " ms\n";
        }
        if (fastSpikingNeuron->hasFired()) {
            fastSpikes++;
            std::cout << "Fast neuron spike at " << i << " ms\n";
        }
    }

    std::cout << "Regular Spikes: " << regularSpikes
              << " | Fast Spikes: " << fastSpikes << "\n";

    EXPECT_GT(fastSpikes, regularSpikes)
        << "Fast spiking neuron should produce more spikes (" << fastSpikes
        << ") than regular spiking neuron (" << regularSpikes << ").";
}

