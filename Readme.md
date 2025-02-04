# BioNeuralNetworkSimulation

Este proyecto implementa una **simulación de redes neuronales bioinspiradas** en C++. Incluye:
- Diferentes **modelos de neurona** (Integrate-and-Fire e Izhikevich).
- **Sinapsis excitatorias e inhibitorias** con mecanismos de plasticidad sináptica (STDP).
- **Estrategias de conectividad** (Random, Small-World y Scale-Free), las cuales han sido validadas mediante pruebas unitarias.
- Un sistema de **logging** que registra spikes y cambios de peso sináptico.

El objetivo es **simular la actividad de una red neuronal** bajo distintos escenarios y configuraciones, grabar la dinámica en archivos de log y, opcionalmente, ejecutar pruebas unitarias con GoogleTest para verificar el correcto funcionamiento de cada módulo.

---

## Estructura de Directorios

```
danqdon-bioneuralnetworksimulation/
├── CMakeLists.txt                 # Script principal de CMake
├── include/                       # Cabeceras de la librería
│   ├── ConnectivityStrategies/    # Interfaces y estrategias de conectividad
│   ├── Core/                     # Componentes centrales (EventManager, Logger, NetworkConfig, SpikeEvent)
│   ├── Network/                  # NetworkManager
│   ├── Neurons/                  # Modelos de neuronas (LIF, Izhikevich)
│   └── Synapses/                 # Sinapsis excitatorias e inhibitorias
├── src/                           # Implementaciones en C++ (.cpp) y main.cpp
│   ├── main.cpp                  # Punto de entrada a la aplicación
│   ├── ConnectivityStrategies/   # Implementaciones de las estrategias de conectividad
│   ├── Core/                     # Implementaciones del Logger, etc.
│   ├── Network/                  # Implementación de NetworkManager
│   ├── Neurons/                  # Implementaciones de LIFNeuron e IzhikevichNeuron
│   └── Synapses/                 # Implementaciones de sinapsis (con STDP)
└── tests/                         # Pruebas unitarias con GoogleTest
    ├── CMakeLists.txt            # Configuración de tests
    ├── test_main.cpp             # Punto de entrada de pruebas
    ├── test_stdp.cpp             # Tests de plasticidad sináptica
    ├── Connectivity/            # Tests para las estrategias de conectivida
    ├── Core/                     # Tests del EventManager
    ├── Network/                  # Tests del NetworkManager
    ├── Neurons/                  # Tests para modelos de neuronas
    └── Synapses/                 # Tests para sinapsis
```

- **CMakeLists.txt**: Configuración del proyecto y enlace de la biblioteca principal.
- **include/**: Contiene las cabeceras de cada módulo (ConnectivityStrategies, Core, Network, Neurons, Synapses).
- **src/**: Contiene las implementaciones en C++ de cada módulo, así como el archivo principal (`main.cpp`).
- **tests/**: Contiene las pruebas unitarias (GoogleTest) que validan el comportamiento de los distintos módulos. Se ha añadido una carpeta específica para los tests de las estrategias de conectividad.

---

## Componentes Principales

### 1. Módulo Core
- **EventManager**: Gestiona eventos (spikes) en una cola de prioridad, permitiendo programar la llegada de un spike a la neurona post-sináptica.
- **Logger**: Registra spikes y cambios de peso en vectores internos y permite exportar estos registros a archivos CSV y de texto para análisis posterior.
- **NetworkConfig**: Encapsula los parámetros de la red (número total de neuronas, proporciones excitatorias/inhibitorias, estrategia de conectividad, etc.).
- **SpikeEvent**: Estructura que representa un evento de spike, incluyendo la hora y la sinapsis por la que se transmite.

### 2. NetworkManager
Es el núcleo de la simulación. Ofrece métodos para:
- Crear neuronas de distintos tipos (LIF o Izhikevich).
- Conectar neuronas con sinapsis excitatorias o inhibitorias (actualizando las listas internas de conexiones).
- Configurar la red a partir de un objeto `NetworkConfig`, que define los parámetros y la estrategia de conectividad a aplicar.
- Ejecutar la simulación mediante el método `runSimulation(tMax, dt)`: en cada paso se inyecta corriente, se actualiza la dinámica neuronal y se gestionan los eventos a través del `EventManager`.
- Exportar los registros de actividad (spikes y cambios de peso).

### 3. Módulo Neurons
Contiene los modelos neuronales:
- **LIFNeuron (Leaky Integrate-and-Fire)**: Modelo simple que dispara cuando el potencial supera el umbral.
- **IzhikevichNeuron**: Modelo que permite reproducir distintos patrones de disparo (regular, bursting, fast-spiking) mediante ajustes en sus parámetros.

### 4. Módulo Synapses
Implementa sinapsis con STDP:
- **ExcitatorySynapse**: Inyecta corriente positiva en la neurona post-sináptica y ajusta el peso sináptico según la diferencia temporal entre spikes pre y post.
- **InhibitorySynapse**: Inyecta corriente negativa y realiza ajustes similares.

### 5. Estrategias de Conectividad
Se implementaron las siguientes estrategias:
- **RandomConnectivityStrategy**: Conecta neuronas con una probabilidad fija.
- **SmallWorldConnectivityStrategy**: Genera conexiones de mundo pequeño con parámetros de reconexión.
- **ScaleFreeConnectivityStrategy**: Utiliza la regla de preferencia de conexión (Barabási-Albert).  
  Además, se han añadido tests unitarios específicos para verificar que cada estrategia establece las conexiones de forma correcta.

---

## Ejemplo de Uso

```cpp
NetworkConfig config;
config.totalNeurons = 1000;
config.excitatoryRatio = 0.8;
config.inhibitoryRatio = 0.2;
config.connectivityStrategy = "Random"; // Opciones: "Random", "SmallWorld", "ScaleFree"

NetworkManager manager;
manager.createNetwork(config);
manager.runSimulation(1000.0, 1.0);
```

---

## Compilación y Ejecución con CLion

El proyecto está configurado para compilarse y ejecutarse directamente en **CLion**, aprovechando su integración con CMake. No es necesario utilizar compiladores como GCC, Clang o MinGW de manera directa. CLion detecta el archivo `CMakeLists.txt` y proporciona una interfaz gráfica para compilar y ejecutar el proyecto, así como para ejecutar las pruebas unitarias mediante CTest.

---

## Conclusiones

El proyecto **BioNeuralNetworkSimulation** ejemplifica una arquitectura modular y extensible para la simulación de redes neuronales, combinando:
- **Modelos de neurona** (LIF e Izhikevich).
- **Plasticidad sináptica** (STDP).
- **Estrategias de conectividad** (Random, Small-World y Scale-Free), validadas mediante pruebas unitarias.
- **Registro de datos** (spikes y cambios de peso) en CSV, facilitando su análisis posterior.

