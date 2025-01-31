cmake_minimum_required(VERSION 3.10)
project(BioNeuralNetwork)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Incluir directorios
include_directories(include)
include_directories(include/ConnectivityStrategies) # Asegúrate de que las cabeceras están en subdirectorios
include_directories(include/Core)
include_directories(include/Neurons)
include_directories(include/Synapses)
include_directories(include/Network)

# Añadir ejecutable
add_executable(BioNeuralNetwork
        src/main.cpp
        src/Core/Logger.cpp
        src/Neurons/IzhikevichNeuron.cpp
        src/Neurons/LIFNeuron.cpp
        src/Synapses/ExcitatorySynapse.cpp
        src/Synapses/InhibitorySynapse.cpp
        src/ConnectivityStrategies/RandomConnectivityStrategy.cpp
        src/ConnectivityStrategies/SmallWorldConnectivityStrategy.cpp      # <-- Añadido
        src/ConnectivityStrategies/ScaleFreeConnectivityStrategy.cpp       # <-- Añadido
        src/Network/NetworkManager.cpp
        # Añade otros archivos .cpp según sea necesario
)

# Opcional: Si utilizas Threads u otras librerías, puedes enlazarlas aquí
# Por ejemplo, para Threads:
# find_package(Threads REQUIRED)
# target_link_libraries(BioNeuralNetwork PRIVATE Threads::Threads)
