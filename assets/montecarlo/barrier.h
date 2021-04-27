#ifndef barrier_h
#define barrier_h

namespace barrierpricer {
    struct ValueMinMax {
        double value;
        double minimum;
        double maximum;
    };

    double box_muller_rand();

    void path_final_min_max(
        const double initial,
        const long steps,
        const double sdt,
        const double drift,
        const double volatility,
        ValueMinMax* output);

    double price_option(
        const double strike,
        const double spot,
        const double time,
        const double volatility,
        const double risk_free,
        const char call_or_put,
        const double knockin,
        const double knockout,
        const long simulations,
        const int steps_per_unit);
}

#endif