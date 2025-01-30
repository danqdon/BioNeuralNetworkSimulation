// tests/test_main.cpp
#include <gtest/gtest.h>

// Punto de entrada principal para las pruebas
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
