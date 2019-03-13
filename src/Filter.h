#ifndef JD_FILTER_H
#define JD_FILTER_H

// Specialization for floating point math.
template <class T>
class ExponentialFilter {
        double m_newWeight;
        T      m_curr;

public:
        ExponentialFilter(double fWeightNew, T fInitial)
            : m_newWeight(fWeightNew), m_curr(fInitial) {}

        void update(const T& newVal) {
                m_curr = newVal * m_newWeight + m_curr * (1.0 - m_newWeight);
        }

        T current() const { return m_curr; }

        void setWeight(T NewWeight) { m_newWeight = NewWeight; }
};

#endif
