#ifndef ___UNIFORM_RANDOM_GENERATOR__HPP___
#define ___UNIFORM_RANDOM_GENERATOR__HPP___

#include <limits>
#include <iostream>
#include <stdint.h>

template <class T=unsigned int>
class UniformRandomGeneratorBase {
protected:
    /* Portable random number generator defined below.  Taken from ANSI C by  *
    * K & R.  Not a great generator, but fast, and good enough for my needs. */

    static const T IA = 1103515245u;
    static const T IC = 12345u;
    static const T IM = 2147483648u;

    T current_random;
public:
    UniformRandomGeneratorBase() : current_random(0) {}
    UniformRandomGeneratorBase(T seed) : current_random(seed) {}

    T current_value() const {
        return current_random;
    }

    void seed(T seed) {
        current_random = (T)seed;
    }

    T irand() {
        return irand(std::numeric_limits<T>::max());
    }

    T irand(T imax) {
        /* Creates a random integer between 0 and imax, inclusive.  i.e. [0..imax] */
        T ival;

        current_random = current_random * IA + IC;	/* Use overflow to wrap */
        ival = current_random & (IM - 1);	/* Modulus */
        ival = (T)((float)ival * (float)(imax + 0.999) / (float)IM);

#if 0
        using namespace std;

        cout << _("[UniformRandomGeneratorBase] irand(%u)=%u")
                % imax % ival << endl;
#endif
        return (ival);
    }

    float frand(void) {
        /* Creates a random float between 0 and 1.  i.e. [0..1).        */
        float fval;
        T ival;

        current_random = current_random * IA + IC;	/* Use overflow to wrap */
        ival = current_random & (IM - 1);	/* Modulus */
        fval = (float)ival / (float)IM;

#if 0
        using namespace std;

        cout << _("[UniformRandomGeneratorBase] frand=%.2f") % fval << endl;
#endif
        return (fval);
    }
};

typedef UniformRandomGeneratorBase<uint32_t> UniformRandomGenerator;


template <class int_t, class real_t>
class ParkMillerRNGBase {
protected:
    static int_t const a = 16807;      //ie 7**5
    static int_t const m = 2147483647; //ie 2**31-1
    real_t const reciprocal_m;
    real_t const reciprocal_m_sub_1;
    int_t seed;
public:
    ParkMillerRNGBase() : seed(0),
        reciprocal_m(1.0 / m), reciprocal_m_sub_1(1.0 / (m - 1)) {}

    ParkMillerRNGBase(int_t seed) : seed(seed),
        reciprocal_m(1.0 / m), reciprocal_m_sub_1(1.0 / (m - 1)) {}

    void set_seed(int_t i_seed) {
        seed = i_seed;
    }

    int_t MOD(real_t value, real_t divisor, real_t divisor_inv) {
        return value - (int_t)floor(value * divisor_inv) * divisor;
    }

    int_t current_value() {
        return seed;
    }

    int_t get_value() {
        real_t temp = seed * a;
        seed = MOD(temp, m, reciprocal_m);
        return seed;
    }

    int_t rand_int(int_t max_value) {
        return get_value() % (max_value + 1);
    }

    real_t rand_real() {
        return get_value() * reciprocal_m_sub_1;
    }
};

typedef ParkMillerRNGBase<uint32_t, float> ParkMillerRNG;

#endif
