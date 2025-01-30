# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(BioNeuralNetwork)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Incluir directorios
include_directories(include)

# Añadir ejecutable
add_executable(BioNeuralNetwork
        src/main.cpp
        src/Core/Logger.cpp
        src/Neurons/IzhikevichNeuron.cpp
        src/Neurons/LIFNeuron.cpp
        src/Synapses/ExcitatorySynapse.cpp
        src/Synapses/InhibitorySynapse.cpp
        src/ConnectivityStrategies/RandomConnectivityStrategy.cpp
        src/Network/NetworkManager.cpp
        # Añade otros archivos .cpp según sea necesario
)

# No es necesario enlazar nlohmann_json ya que ya no se usa
