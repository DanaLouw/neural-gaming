// Dana Louw 2025-03-22

#include "Vector.hpp"
#include <cmath>


// PolarVector2D
// Member functions
template <typename T> PolarVector2D PolarVector2D::operator*(const T scalar) const {
  return Polarvector2D(magnitude * scalar, angle);
}
template <typename T> PolarVector2D PolarVector2D::operator/(const T scalar) const {
  return Polarvector2D(magnitude / scalar, angle);
}
template <typename T> void PolarVector2D::operator*=(const T& scalar) {
  magnitude *= scalar;
}
template <typename T> void PolarVector2D::operator/=(const T& scalar) {
  magnitude /= scalar;
}

// Getters
template <typename T> T PolarVector2D::x() const {
  return magnitude * std::cos(angle);
}
template <typename T> T PolarVector2D::y() const {
  return magnitude * std::sin(angle);
}
std::string PolarVector2D::toString() const {
  return "(" + std::to_string(magnitude) + ", " + std::to_string(angle) + ")";
}



// vector2
// Constructors
template <typename T> vector2<T>::vector2(const PolarVector2D& polar) {
  this->x = polar.magnitude * std::cos(polar.angle);
  this->y = polar.magnitude * std::sin(polar.angle);
};
// Member functions
template <typename T> vector2<T> vector2<T>::operator+(const vector2<T>& other) const {
  return vector2<T>(x + other.x, y + other.y);
}
template <typename T> vector2<T> vector2<T>::operator-(const vector2<T>& other) const {
  return vector2<T>(x - other.x, y - other.y);
}
template <typename T> vector2<T> vector2<T>::operator*(const vector2<T>& other) const {
  return vector2<T>(x * other.x, y * other.y);
}
template <typename T> vector2<T> vector2<T>::operator/(const vector2<T>& other) const {
  return vector2<T>(x / other.x, y / other.y);
}
template <typename T> vector2<T> vector2<T>::operator*(const T& scalar) const {
  return vector2<T>(x * scalar, y * scalar);
}
template <typename T> vector2<T> vector2<T>::operator/(const T& scalar) const {
  return vector2<T>(x / scalar, y / scalar);
}

template <typename T> void vector2<T>::operator+=(const vector2<T>& other) {
  x += other.x;
  y += other.y;
}
template <typename T> void vector2<T>::operator-=(const vector2<T>& other) {
  x -= other.x;
  y -= other.y;
}
template <typename T> void vector2<T>::operator*=(const T& scalar) {
  x *= scalar;
  y *= scalar;
}
template <typename T> void vector2<T>::operator/=(const T& scalar) {
  x /= scalar;
  y /= scalar;
}


template <typename T> template <typename U> U vector2<T>::dot(const vector2<T>& other) const {
  return x * other.x + y * other.y;
}
template <typename T> vector2<T> vector2<T>::normalized() const {
  return vector2<T>(x / magnitude(), y / magnitude());
}

// Getters
template <typename T> vector2<T> vector2<T>::to(const vector2<T>& other) const {
  return vector2<T>(other.x - x, other.y - y);
}
template <typename T> PolarVector2D vector2<T>::polarized() const {
  return PolarVector2D(magnitude(), angle());
}
template <typename T> double vector2<T>::magnitude() const {
  return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}
template <typename T> double vector2<T>::angle() const {
  return std::atan2(y, x);
}
template <typename T> std::string vector2<T>::toString() const {
  return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

template struct vector2<int>;
template struct vector2<float>;
template struct vector2<double>;
