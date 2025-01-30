# tests/CMakeLists.txt

# Incluir el módulo FetchContent de CMake
include(FetchContent)

# Declarar el repositorio de GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE  # Para evitar la advertencia de CMake
)

# Prevenir que CMake sobreescriba las configuraciones del proyecto padre en Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Hacer que GoogleTest esté disponible para el proyecto
FetchContent_MakeAvailable(googletest)

# Ahora, añade el ejecutable de pruebas con los nuevos archivos de test
add_executable(NeuralSimulationTests
        test_main.cpp
        test_neuron.cpp
        test_synapse.cpp
        test_event_manager.cpp
        test_network_manager.cpp
        test_stdp.cpp
)

# Linkear el ejecutable de pruebas contra GoogleTest y la biblioteca principal
target_link_libraries(NeuralSimulationTests
        GTest::gtest_main
        NeuralSimulationLib  # Asegúrate de linkear contra la biblioteca correcta
)

# Descubrir y registrar pruebas con CTest
include(GoogleTest)
gtest_discover_tests(NeuralSimulationTests)
