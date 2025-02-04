cmake_minimum_required(VERSION 3.10)
project(BioNeuralNetwork)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Incluir directorios
include_directories(include)
include_directories(include/ConnectivityStrategies)
include_directories(include/Core)
include_directories(include/Neurons)
include_directories(include/Synapses)
include_directories(include/Network)

# Crear la biblioteca (sin main.cpp)
add_library(NeuralSimulationLib
        src/Core/Logger.cpp
        src/Neurons/IzhikevichNeuron.cpp
        src/Neurons/LIFNeuron.cpp
        src/Synapses/ExcitatorySynapse.cpp
        src/Synapses/InhibitorySynapse.cpp
        src/ConnectivityStrategies/RandomConnectivityStrategy.cpp
        src/ConnectivityStrategies/SmallWorldConnectivityStrategy.cpp
        src/ConnectivityStrategies/ScaleFreeConnectivityStrategy.cpp
        src/Network/NetworkManager.cpp
        # Agregar otros .cpp según sea necesario
)

add_executable(BioNeuralNetwork src/main.cpp)
target_link_libraries(BioNeuralNetwork PRIVATE NeuralSimulationLib)

enable_testing()
add_subdirectory(tests)
