/**
 * @file Vector_test.cpp
 * @brief Tests de la clase Vector.
 */

#include <cmath>
#include <sstream>

#include <gtest/gtest.h>

#include "Vector.hpp"

TEST(Vector, Constructores) {
  Vector<int> v(1, 2);
  EXPECT_EQ(v.getX(), 1);
  EXPECT_EQ(v.getY(), 2);
  EXPECT_EQ(v.getZ(), 0);
  EXPECT_EQ(Vector<int>(1, 2, 3).getZ(), 3);
  EXPECT_EQ(Vector<int>(Punto<int>(1, 2)), v); // desde un punto
}

TEST(Vector, Copia) {
  Vector<int> v(1, 2, 3);
  Vector<int> c(v);
  Vector<int> a(0, 0);
  a = v;
  EXPECT_EQ(c, v);
  EXPECT_EQ(a, v);
}

TEST(Vector, Magnitud) {
  EXPECT_DOUBLE_EQ(Vector<int>(3, 4).magnitud(), 5.0);
  EXPECT_DOUBLE_EQ(Vector<int>(1, 1).magnitud(),
                   std::sqrt(2.0)); // no trunca con T entero
}

TEST(Vector, ProductoPunto) {
  EXPECT_EQ(Vector<int>(1, 2).productoPunto(Vector<int>(3, 4)), 11);
}

TEST(Vector, ProductoCruz) {
  EXPECT_EQ(Vector<int>(1, 2).productoCruz(Vector<int>(3, 4)),
            Vector<int>(0, 0, -2)); // escalar en z
}

TEST(Vector, Suma) {
  EXPECT_EQ(Vector<int>(1, 2) + Vector<int>(3, 4), Vector<int>(4, 6));
}

TEST(Vector, ProductoPorEscalar) {
  EXPECT_EQ(Vector<int>(1, 2) * 3, Vector<int>(3, 6));
}

TEST(Vector, Igualdad) {
  EXPECT_EQ(Vector<int>(1, 2), Vector<int>(1, 2));
  EXPECT_NE(Vector<int>(1, 2), Vector<int>(2, 1));
}

TEST(Vector, Salida) {
  std::ostringstream os;
  os << Vector<int>(1, 2);
  EXPECT_EQ(os.str(), "(1,2)");
}
