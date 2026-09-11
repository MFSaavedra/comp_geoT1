/**
 * @file main.cpp
 * @brief Ejemplo de uso de Punto, Vector y Poligono.
 */

#include <iostream>

#include "Poligono.hpp"
#include "Punto.hpp"
#include "Vector.hpp"

int main() {
  std::cout << std::boolalpha;

  Punto<int> p(0, 0), q(3, 4);
  std::cout << "p = " << p << ", q = " << q << "\n"
            << "distancia(p, q) = " << p.distancia(q) << "\n"
            << "p == q: " << (p == q) << "\n\n";

  Vector<double> u(0.5, 1.5), v(Punto<double>(3, 4));
  std::cout << "u = " << u << ", v = " << v << "\n"
            << "u + v = " << u + v << "\n"
            << "u * 2 = " << u * 2 << "\n"
            << "|v| = " << v.magnitud() << "\n"
            << "u . v = " << u.productoPunto(v) << "\n"
            << "u x v (en z) = " << u.productoCruz(v).getZ() << "\n"
            << "u == v: " << (u == v) << "\n\n";

  Poligono<int> poli({{0, 0}, {0, 2}, {2, 2}, {2, 0}}); // en CW
  std::cout << "poli = " << poli << "\n"
            << "cantidad de puntos = " << poli.cantidadPuntos() << "\n"
            << "area = " << poli.area() << "\n"
            << "esCCW: " << poli.esCCW() << "\n";
  poli.hacerCCW();
  std::cout << "hacerCCW -> " << poli << ", esCCW: " << poli.esCCW() << "\n"
            << "poli[0] = " << poli[0] << "\n";
  return 0;
}
