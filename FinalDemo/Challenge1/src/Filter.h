#ifndef JD_FILTER_H
#define JD_FILTER_H


/**
 * @brief Simple Exponential Filter
 */
template <class T>
class ExponentialFilter {
        double m_newWeight;
        T      m_curr;

public:
        ExponentialFilter(double newWeight, const T& initialCondition)
            : m_newWeight(newWeight), m_curr(initialCondition) {}

        // Updates current value with new data
        void update(const T& newVal) {
                m_curr = newVal * m_newWeight + m_curr * (1.0 - m_newWeight);
        }

        T current() const { return m_curr; }

        void setWeight(T NewWeight) { m_newWeight = NewWeight; }
};

#endif
