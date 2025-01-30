# CMakeLists.txt (Directorio Raíz)

cmake_minimum_required(VERSION 3.10)
project(BioNeuralNetworkSimulation)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Habilitar testing
enable_testing()

# Añadir la biblioteca principal
add_library(NeuralSimulationLib
        src/LIFNeuron.cpp
        src/ExcitatorySynapse.cpp
        src/InhibitorySynapse.cpp
        src/IzhikevichNeuron.cpp
        src/NetworkManager.cpp
)

# Directorios de inclusión para la biblioteca
target_include_directories(NeuralSimulationLib PUBLIC include)

# Añadir el ejecutable principal
add_executable(SimulationExecutable src/main.cpp)

# Enlazar la biblioteca con el ejecutable
target_link_libraries(SimulationExecutable PRIVATE NeuralSimulationLib)

# Añadir el subdirectorio de pruebas
add_subdirectory(tests)
