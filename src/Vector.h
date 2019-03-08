#ifndef JD_VECTOR_3D
#define JD_VECTOR_3D

#include <math.h>

namespace JD {

/*
 * Porting type_traits 
 *
 */ 
template <bool B, class T = void>
struct enable_if {};

template <class T>
struct enable_if<true, T> {
        typedef T type;
};

struct true_type {
        enum { value = true };
};

struct false_type {
        enum { value = false };
};

template <class T, class U>
struct is_same : false_type {};

template <class T>
struct is_same<T, T> : true_type {};

/*
 *
 * Vector Class
 *
 */


template <unsigned size>
class Vector {
        static_assert(size != 0, "Cannot have a 0 dimensional vector");

public:
        using Number_t = double;

        Number_t data[size];

        constexpr Vector() : data() {}
        Vector(const Vector& v) {
                for (unsigned i = 0; i < size; i++) {
                        data[i] = v[i];
                }
        }

        // Enable only if number of parameters is equal to size of vector
        template <class... T>
        constexpr Vector(T... args) : data{static_cast<Number_t>(args)...} {
                static_assert(sizeof...(args) == size,
                              "Number of arguments must match vector size");
        }

        Vector & operator=(const Vector& v){
                if (this != &v){
                        for (int i = 0; i < size; i++)
                                data[i] = v[i];
                }
                
                return *this;
        }

        Number_t dot(const Vector& v) const {
                Number_t dot_prod = 0;
                for (unsigned i = 0; i < size; i++)
                        dot_prod += data[i] * v[i];

                return dot_prod;
        }
        Number_t norm() const { return sqrt(this->dot(*this)); }

        Vector& normalize() {
                Number_t m = norm();
                if (m != 0)
                        for (Number_t& i : data)
                                i /= m;
                return *this;
        }

        constexpr Number_t cosine(const Vector& v) const {
                return (norm() * v.norm() != 0) ? dot(v) / (norm() * v.norm())
                                                : 0.0 / 0.0;
        }

        bool operator==(const Vector& toComp) const {
                if (&toComp == this)
                        return true;

                for (unsigned i = 0; i < size; i++)
                        if (data[i] != toComp.data[i])
                                return false;

                return true;
        }

        template <class T>
        typename enable_if<not is_same<T, Vector>::value, Vector&>::type
        operator+=(const T& toAdd) {
                for (unsigned i = 0; i < size; i++)
                        data[i] += toAdd;
                return *this;
        }
        Vector& operator+=(const Vector& toAdd) {
                for (unsigned i = 0; i < size; i++)
                        data[i] += toAdd.data[i];
                return *this;
        }

        template <class T>
        typename enable_if<not is_same<T, Vector>::value, Vector&>::type
        operator-=(const T& toAdd) {
                for (unsigned i = 0; i < size; i++)
                        data[i] -= toAdd;

                return *this;
        }
        Vector& operator-=(const Vector& toAdd) {
                for (unsigned i = 0; i < size; i++)
                        data[i] -= toAdd.data[i];

                return *this;
        }

        template <class T>
        typename enable_if<not is_same<T, Vector>::value, Vector&>::type
        operator*=(const T& toAdd) {
                for (unsigned i = 0; i < size; i++)
                        data[i] *= toAdd;

                return *this;
        }

        Vector& operator*=(const Vector& toAdd) {
                for (unsigned i = 0; i < size; i++)
                        data[i] *= toAdd.data[i];

                return *this;
        }

        template <class T>
        Vector operator+(const T& toAdd) const& {
                return Vector(*this) += toAdd;  // make a copy
        }

        template <class T>
        Vector operator-(const T& toAdd) const& {
                return Vector(*this) -= toAdd;  // make a copy
        }

        template <class T>
        Vector operator*(const T& toAdd) const& {
                return Vector(*this) *= toAdd;  // make a copy
        }

        template <class T>
        Vector&& operator+(const T& toAdd) && {
                return static_cast<Vector&&>(*this += toAdd);
        }
        template <class T>
        Vector&& operator-(const T& toAdd) && {
                return static_cast<Vector&&>(*this -= toAdd);
        }

        template <class T>
        Vector&& operator*(const T& toAdd) && {
                return static_cast<Vector&&>(*this *= toAdd);
        }

        constexpr const Number_t& operator[](unsigned ind) const {
                return data[ind];
        }

        Number_t& operator[](unsigned ind) { return data[ind]; }
};
}  // namespace JD
#endif