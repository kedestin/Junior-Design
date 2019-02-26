#ifndef JD_VECTOR_3D
#define JD_VECTOR_3D

#include <type_traits>

class Vector3 {
public:
        using Number_t = double;

        Number_t data[3];

        constexpr Vector3() : data{0, 0, 0} {}
        constexpr Vector3(Number_t x, Number_t y, Number_t z)
            : data{x, y, z} {}

        constexpr Number_t dot(const Vector3& v) {
                return data[0] * v.data[0] + data[1] * v.data[1] +
                       data[2] * v.data[2];
        }
        constexpr Number_t norm() {
                return data[0] * data[0] + data[1] * data[1] +
                       data[2] * data[2];
        }

        Vector3& normalize(){
                Number_t m = norm();
                if (m != 0)
                        for (Number_t& i : data)
                                i /= m;

                return *this;
        }

        constexpr Number_t cosine(const Vector3& v) {
                return (norm() * v.norm() != 0) ? dot(v) / (norm() * v.norm())
                                                : 0.0 / 0.0;
        }

        bool operator==(const Vector3& toComp) const {
                if (&toComp == this)
                        return true;

                for (unsigned i = 0; i < 3; i++)
                        if (data[i] != toComp.data[i])
                                return false;

                return true;
        }

        template <class T>
        typename std::enable_if<not std::is_same<T, Vector3>::value,
                                Vector3&>::type
        operator+=(const T& toAdd) {
                for (unsigned i = 0; i < 3; i++)
                        data[i] += toAdd;
                return *this;
        }
        Vector3& operator+=(const Vector3& toAdd) {
                for (unsigned i = 0; i < 3; i++)
                        data[i] += toAdd.data[i];
                return *this;
        }

        template <class T>
        typename std::enable_if<not std::is_same<T, Vector3>::value,
                                Vector3&>::type
        operator-=(const T& toAdd) {
                for (unsigned i = 0; i < 3; i++)
                        data[i] -= toAdd;

                return *this;
        }
        Vector3& operator-=(const Vector3& toAdd) {
                for (unsigned i = 0; i < 3; i++)
                        data[i] -= toAdd.data[i];

                return *this;
        }

        template <class T>
        typename std::enable_if<not std::is_same<T, Vector3>::value,
                                Vector3&>::type
        operator*=(const T& toAdd) {
                for (unsigned i = 0; i < 3; i++)
                        data[i] *= toAdd;

                return *this;
        }

        Vector3& operator*=(const Vector3& toAdd) {
                for (unsigned i = 0; i < 3; i++)
                        data[i] *= toAdd.data[i];

                return *this;
        }

        template <class T>
        Vector3 operator+(const T& toAdd) const& {
                return Vector3(*this) += toAdd;  // make a copy
        }

        template <class T>
        Vector3 operator-(const T& toAdd) const& {
                return Vector3(*this) -= toAdd;  // make a copy
        }

        template <class T>
        Vector3 operator*(const T& toAdd) const& {
                return Vector3(*this) *= toAdd;  // make a copy
        }

        template <class T>
        Vector3&& operator+(const T& toAdd) && {
                return static_cast<Vector3&&>(*this += toAdd);
        }
        template <class T>
        Vector3&& operator-(const T& toAdd) && {
                return static_cast<Vector3&&>(*this -= toAdd);
        }

        template <class T>
        Vector3&& operator*(const T& toAdd) && {
                return static_cast<Vector3&&>(*this *= toAdd);
        }


        const Number_t& operator[](unsigned ind) const {
                return data[ind];
        }

        Number_t& operator[](unsigned ind) {s
                return data[ind];
        }
};

#endif