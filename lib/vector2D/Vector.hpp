// Dana Louw 2025-03-22
// A 2D vector structure to be used in physics calculations and graphics

#ifndef vECTOR2D_H
#define vECTOR2D_H

#include <SFML/System/Vector2.hpp>
#include <string>


struct PolarVector2D {
  double magnitude;
  long double angle;

  PolarVector2D(double magnitude = 0, long double angle = 0) : magnitude(magnitude), angle(angle) {};

  // Member functions
  template <typename T> PolarVector2D operator*(const T scalar) const;
  template <typename T> PolarVector2D operator/(const T scalar) const;
  template <typename T> void operator*=(const T& scalar);
  template <typename T> void operator/=(const T& scalar);

  // Getters
  template <typename T> T x() const;
  template <typename T> T y() const;
  std::string toString() const;
};


template <typename T>
struct vector2 {
  T x {}, y {};

  // Constructors
  vector2(T x = 0, T y = 0) : x(std::move(x)), y(std::move(y)) {};
  vector2(const PolarVector2D &polar);

  // Member functions
  vector2<T> operator+(const vector2<T>& other) const;
  vector2<T> operator-(const vector2<T>& other) const;
  vector2<T> operator*(const vector2<T>& other) const;
  vector2<T> operator/(const vector2<T>& other) const;
  vector2<T> operator*(const T& scalar) const;
  vector2<T> operator/(const T& scalar) const;

  void operator+=(const vector2<T>& other);
  void operator-=(const vector2<T>& other);
  void operator*=(const T& scalar);
  void operator/=(const T& scalar);

  template <typename U> U dot(const vector2<T>& other) const;
  vector2<T> normalized() const;


  // Setters
  // TODO

  // Getters
  vector2<T> to(const vector2<T>& other) const;
  PolarVector2D polarized() const;
  double magnitude() const;
  double angle() const;
  std::string toString() const;
};


using vector2d = vector2<double>;
using vector2f = vector2<float>;
using vector2i = vector2<int>;
using vector2u = vector2<unsigned int>;

#endif