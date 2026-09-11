/**
 * @file Vector.hpp
 * @brief Clase Vector: vector matematico en el plano.
 */

#pragma once

#include <ostream>

#include "Punto.hpp"

/**
 * @brief Vector en el plano, guardado como el Punto que va del origen a su
 * extremo.
 * @tparam T Tipo numerico de las componentes.
 * @note La componente z existe para el resultado de productoCruz; las demas
 * operaciones la propagan para no perderla.
 */
template <typename T> class Vector {
private:
  Punto<T> p; ///< Componentes del vector, almacenadas como un Punto.

public:
  /**
   * @brief Construye un vector 2D, con z = 0.
   * @param x Componente x.
   * @param y Componente y.
   */
  Vector(T x, T y) : p(x, y) {}

  /**
   * @brief Construye un vector 3D.
   * @param x Componente x.
   * @param y Componente y.
   * @param z Componente z.
   */
  Vector(T x, T y, T z) : p(x, y, z) {}

  /**
   * @brief Construye el vector que va del origen a un punto.
   * @param p Extremo del vector.
   */
  explicit Vector(const Punto<T> &p) : p(p) {}

  /**
   * @brief Constructor de copia.
   * @param other Vector a copiar.
   */
  Vector(const Vector<T> &other) : p(other.p) {}

  /**
   * @brief Asignacion por copia.
   * @param other Vector a copiar.
   * @return Referencia a este vector.
   */
  Vector<T> &operator=(const Vector<T> &other) = default;

  // getters
  /** @brief Retorna la componente x. */
  T getX() const { return p.getX(); }

  /** @brief Retorna la componente y. */
  T getY() const { return p.getY(); }

  /** @brief Retorna la componente z. */
  T getZ() const { return p.getZ(); }

  /**
   * @brief Magnitud (norma euclidiana) del vector.
   * @return La magnitud en double.
   */
  double magnitud() const {
    return p.distancia(Punto<T>()); // distancia al origen, ya en double
  }

  /**
   * @brief Producto punto entre este vector y otro.
   * @param other Segundo vector.
   * @return La suma de los productos componente a componente.
   */
  T productoPunto(const Vector<T> &other) const {
    return getX() * other.getX() + getY() * other.getY() +
           getZ() * other.getZ();
  }

  /**
   * @brief Producto cruz entre este vector y otro.
   * @param other Segundo vector.
   * @return Un nuevo vector (0,0,z) con el producto cruz en z.
   * @note En 2D el producto cruz es un escalar sobre el eje z; se entrega
   *       como vector porque el enunciado pide retornar un vector.
   */
  Vector<T> productoCruz(const Vector<T> &other) const {
    return Vector<T>(T{}, T{}, getX() * other.getY() - getY() * other.getX());
  }

  /**
   * @brief Suma componente a componente.
   * @param other Vector a sumar.
   * @return Un nuevo vector con la suma.
   */
  Vector<T> operator+(const Vector<T> &other) const {
    return Vector<T>(getX() + other.getX(), getY() + other.getY(),
                     getZ() + other.getZ());
  }

  /**
   * @brief Producto por un escalar.
   * @param scalar Factor que multiplica cada componente.
   * @return Un nuevo vector escalado.
   */
  Vector<T> operator*(T scalar) const {
    return Vector<T>(getX() * scalar, getY() * scalar, getZ() * scalar);
  }

  /**
   * @brief Compara dos vectores componente a componente.
   * @param other Vector a comparar.
   * @return true si todas las componentes coinciden.
   */
  bool operator==(const Vector<T> &other) const { return p == other.p; }

  /**
   * @brief Verifica si 2 vectores son distintos.
   * @param other Vector a comparar.
   * @return true si difieren en alguna componente.
   */
  bool operator!=(const Vector<T> &other) const { return !(*this == other); }

  /**
   * @brief Imprime el vector en formato (x,y).
   * @param os Flujo de salida.
   * @param v Vector a imprimir.
   * @return El mismo flujo os.
   */
  friend std::ostream &operator<<(std::ostream &os, const Vector<T> &v) {
    return os << v.p;
  }
};
