# comp_geoT1

Tarea 1 del curso CC5502 Geometría Computacional (DCC): las clases `Punto`,
`Vector` y `Poligono` con templates, sus tests y un `main` de ejemplo.

## Estructura

```
.
├── CMakeLists.txt
├── Doxyfile
├── docs/html/             # documentación generada con Doxygen
├── include/
│   ├── Punto.hpp          # Punto<T>
│   ├── Vector.hpp         # Vector<T>, compone un Punto<T>
│   └── Poligono.hpp       # Poligono<T>, guarda un std::vector<Punto<T>>
├── src/
│   └── main.cpp           # instancia las tres clases y usa sus métodos
└── test/
    ├── Punto_test.cpp     # un TEST de Google Test por método público
    ├── Vector_test.cpp
    └── Poligono_test.cpp
```

Como las clases son templates, todo su código vive en los headers de
`include/`; `src/` solo contiene el `main`.

## Requisitos

- CMake ≥ 3.14 y un compilador con soporte de C++17 (`g++` o `clang++`).
- Google Test, opcional: si está instalado (`libgtest-dev` en Debian/Ubuntu,
  `gtest` en Arch) se usa; si no, CMake lo descarga de GitHub al configurar
  por primera vez.

## Compilar

Desde la raíz del repositorio:

```bash
cmake -S . -B build
cmake --build build
```

Esto genera `build/main` y `build/geo_tests`.

## Ejecutar los tests

```bash
cd build && ctest --output-on-failure
```

`ctest` lista cada caso (`Punto.Distancia`, `Poligono.Area`, ...) con su
resultado. Para ver la salida completa de Google Test, ejecutar el binario
directamente:

```bash
./build/geo_tests
```

## Ejecutar el main

```bash
./build/main
```

## Sin CMake

Con Google Test instalado en el sistema:

```bash
g++ -std=c++17 -Iinclude src/main.cpp -o main && ./main
g++ -std=c++17 -Iinclude test/*.cpp -lgtest -lgtest_main -pthread -o geo_tests && ./geo_tests
```

## Documentación

Los headers están comentados con Doxygen. Con `doxygen` instalado, desde la
raíz del repositorio:

```bash
doxygen
```

genera la documentación en `docs/html/index.html`.

## Notas de diseño

- `Punto<T>` guarda una coordenada `z` (0 al construir con dos valores) para
  que `Vector<T>::productoCruz` pueda retornar un vector, como pide el
  enunciado: el producto cruz de dos vectores 2D es un escalar sobre el eje z.
  `operator<<` imprime solo `(x,y)`.
- `distancia`, `magnitud` y `area` retornan `double` aunque `T` sea entero, y
  promueven a `double` antes de operar para no truncar ni desbordar.
- `operator==` es exacto con tipos enteros y tolerante con punto flotante.
- `esCCW`, `hacerCCW` y `area` se apoyan en la misma área con signo (fórmula
  del shoelace): el signo da la orientación, invertir los puntos la cambia y
  el valor absoluto es el área.

## Declaración de uso de IA

Se utilizó Claude Opus 5 para corregir y complementar el código, para discutir conceptos e ideas de diseño y generar la documentación.
