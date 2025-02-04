# BioNeuralNetworkSimulation

Este proyecto implementa una **simulación de redes neuronales bioinspiradas** en C++. Incluye:
- Diferentes **modelos de neurona** (Integrate-and-Fire y Izhikevich).
- **Sinapsis excitatorias e inhibitorias** con mecanismos de plasticidad sináptica (STDP).
- **Estrategias de conectividad** (Aleatoria, Small-World y Scale-Free).
- Un sistema de **logging** que registra spikes y cambios de peso sináptico.

El objetivo es **simular la actividad de una red neuronal** bajo distintos escenarios y configuraciones, grabar la dinámica en archivos de log y, opcionalmente, realizar pruebas unitarias con GoogleTest.

---

## **Estructura de Directorios**

```
danqdon-bioneuralnetworksimulation/
├── CMakeLists.txt                 # Script principal de CMake
├── include/                       # Cabeceras de la librería
│   ├── ConnectivityStrategies/
│   ├── Core/
│   ├── Network/
│   ├── Neurons/
│   └── Synapses/
├── src/                           # Implementaciones de la librería y el main
│   ├── main.cpp                   # Punto de entrada a la aplicación
│   ├── ConnectivityStrategies/
│   ├── Core/
│   ├── Network/
│   ├── Neurons/
│   └── Synapses/
└── tests/                         # Pruebas unitarias con GoogleTest
    ├── CMakeLists.txt
    ├── test_main.cpp
    ├── test_stdp.cpp
    ├── Core/
    ├── Network/
    ├── Neurons/
    └── Synapses/
```

- **CMakeLists.txt**: Configuración del proyecto y enlazado de ejecutables.
- **include/**: Cabeceras de cada módulo:
    - **ConnectivityStrategies/**: Define interfaces y estrategias de conectividad (`IConnectivityStrategy.h`, etc.).
    - **Core/**: Componentes centrales de la simulación (EventManager, Logger, configuración, etc.).
    - **Network/**: Cabecera de `NetworkManager.h`, el gestor principal de la red.
    - **Neurons/**: Definición de modelos de neuronas (LIF, Izhikevich).
    - **Synapses/**: Definición de sinapsis excitatorias e inhibitorias.
- **src/**: Implementaciones en C++ (`.cpp`) de las clases descritas en `include/`.
    - **main.cpp**: Punto de entrada, configura y ejecuta una simulación de ejemplo.
    - **ConnectivityStrategies/**: Implementaciones específicas (Random, SmallWorld, ScaleFree).
    - **Core/**: Implementaciones de logger, etc.
    - **Network/**: Implementación de `NetworkManager`.
    - **Neurons/**: Implementaciones de LIF e Izhikevich.
    - **Synapses**: Implementaciones de sinapsis excitatorias e inhibitorias (con STDP).
- **tests/**: Pruebas unitarias (GoogleTest) que validan el comportamiento de los distintos módulos.

---

## **Componentes Principales**

### **1. Módulo Core**
1. **EventManager**
    - Gestiona eventos de spikes ordenados en el tiempo (cola de prioridad).
    - Permite programar la llegada de un spike a la neurona post-sináptica.

2. **Logger**
    - Registra spikes y cambios de peso en vectores internos.
    - Exporta registros a CSV y a un archivo de texto para su posterior análisis.

3. **NetworkConfig**
    - Encapsula los parámetros de la red neuronal, como:
        - Número total de neuronas.
        - Proporciones de excitatorias e inhibitorias.
        - Tipo de estrategia de conectividad.
        - Configuración de conexiones excitatorias/inhibitorias (probabilidad, peso, etc.).

4. **SpikeEvent**
    - Estructura simple que indica cuándo y a través de qué sinapsis se disparará un spike.

---

### **2. NetworkManager**
- Núcleo de la simulación. Ofrece métodos para:
    1. **Crear neuronas** de distintos tipos (`LIF` o `Izhikevich`).
    2. **Conectar neuronas** con sinapsis excitatorias o inhibitorias.
    3. **Configurar la red** a partir de un `NetworkConfig`, que internamente crea la cantidad de neuronas excitatorias e inhibitorias y aplica la estrategia de conectividad seleccionada.
    4. **`runSimulation(tMax, dt)`**:
        - Itera en pasos de tiempo (por ejemplo, 1000 ms con dt=1).
        - Inyecta corriente en cada paso.
        - Actualiza la dinámica neuronal.
        - Gestiona llegada de spikes a través de `EventManager`.
    5. **Exportar logs** al finalizar (spikes y cambios de peso).

---

### **3. Módulo Neurons**
- **LIFNeuron** (Leaky Integrate-and-Fire):
    - Modelo simple de neurona con umbral y potencial de reposo.
- **IzhikevichNeuron**:
    - Modelo más avanzado que maneja diferentes patrones de disparo (regular spiking, bursting, etc.).
    - El logger registra los spikes cada vez que `V` supera el umbral `V_threshold`.

---

### **4. Módulo Synapses**
- **ExcitatorySynapse / InhibitorySynapse**:
    - Implementan STDP (plasticidad sináptica):
        - `adjustWeight(delta_t, eventTime)`: Cambia el peso según la diferencia temporal entre spike pre y spike post.
    - **Diferencia**: Excitatoria inyecta `+weight` y la Inhibitoria inyecta `-weight`.

---

### **5. Estrategias de Conectividad**
- **RandomConnectivityStrategy**: Conexiones aleatorias con probabilidad fija.
- **SmallWorldConnectivityStrategy**: Modelo de mundo pequeño con reconexión aleatoria.
- **ScaleFreeConnectivityStrategy**: Modelo basado en preferencia de conexión (Barabási-Albert).

---

## **Ejemplo de Uso**

```cpp
NetworkConfig config;
config.totalNeurons = 1000;
config.excitatoryRatio = 0.8;
config.inhibitoryRatio = 0.2;
config.connectivityStrategy = "Random";

NetworkManager manager;
manager.createNetwork(config);
manager.runSimulation(1000.0, 1.0);
```

---

## **Pruebas Unitarias**

Para ejecutar las pruebas con GoogleTest:
```bash
cd build
ctest
```

---

## **Cómo Compilar**

1. **Requisitos**:
    - CMake >= 3.10
    - C++17
    - (Opcional) GoogleTest para las pruebas.

2. **Procedimiento**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make -j4
   ```
    - Generará el ejecutable `BioNeuralNetwork` que corre la simulación principal.

3. **Ejecución**:
   ```bash
   ./BioNeuralNetwork
   ```

---

## **Posibles Extensiones**

1. **Nuevas Estrategias de Conectividad**
    - Añadir más clases que hereden de `IConnectivityStrategy`.

2. **Integración con Python**
    - Usar `pybind11` para conectar con Python.

3. **Optimización**
    - Se puede paralelizar la actualización de neuronas o usar técnicas de GPU.

---

## **Conclusiones**

El proyecto **BioNeuralNetworkSimulation** ejemplifica una arquitectura modular y extensible para simular redes neuronales, combinando:

- **Modelos de neurona** (LIF, Izhikevich).
- **Plasticidad sináptica** con STDP.
- **Estrategias de conectividad** (Random, Small World, Scale-Free).
- **Registro de datos** (spikes y cambios de peso) en CSV.