# tests/CMakeLists.txt

# Incluir el módulo FetchContent de CMake para GoogleTest
include(FetchContent)

# Declarar el repositorio de GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Agregar el ejecutable de pruebas con los archivos de test
add_executable(NeuralSimulationTests
        test_main.cpp
        test_stdp.cpp
        Core/test_event_manager.cpp
        Network/test_network_manager.cpp
        Neurons/test_izhikevich_neuron.cpp
        Neurons/test_lif_neuron.cpp
        Synapses/test_synapse.cpp
        Connectivity/test_connectivity_strategies.cpp  # NUEVO fichero agregado
)

# Enlazar el ejecutable de pruebas con GoogleTest y con la biblioteca principal
target_link_libraries(NeuralSimulationTests
        PRIVATE
        GTest::gtest_main
        NeuralSimulationLib
)

# Incluir directorios de cabeceras para los tests (si no se heredan del proyecto principal)
target_include_directories(NeuralSimulationTests PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Descubrir y registrar pruebas con CTest
include(GoogleTest)
gtest_discover_tests(NeuralSimulationTests)
