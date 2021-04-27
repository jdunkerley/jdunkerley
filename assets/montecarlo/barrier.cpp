#include <cstdlib>
#include <random>
#include <iostream>
#include "barrier.h"

namespace barrierpricer {
    static std::random_device rd;
    static std::mt19937_64 rand_generator(rd());
    static std::normal_distribution<> dis;

    double box_muller_rand() {
        return dis(rand_generator);
    }

    void path_final_min_max(
            const double initial,
            const long steps,
            const double sdt,
            const double drift,
            const double volatility,
            ValueMinMax *output) {

        output->value = initial;
        output->minimum = initial;
        output->maximum = initial;

        for (long i = 0L; i < steps - 1; i++) {
            double normal_random = box_muller_rand();
            output->value = output->value * drift * std::exp(sdt * volatility * normal_random);
            if (output->minimum > output->value) {
                output->minimum = output->value;
            }
            if (output->maximum < output->value) {
                output->maximum = output->value;
            }
        }
    }

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
            const int steps_per_unit) {

        if (knockin && knockout) {
            throw std::invalid_argument("Unable to cope with 2 barriers!");
        }

        const int cp = call_or_put == 'c' ? 1 : -1;
        const double dt = 1.0 / static_cast<double>(steps_per_unit);
        const long steps = static_cast<long>(time * steps_per_unit);
        const double sdt = std::sqrt(dt);
        const double drift = std::exp((risk_free - 0.5 * volatility * volatility) * dt);

        auto vmm = ValueMinMax{0, 0, 0};

        double total = 0;
        for (long i = 0l; i < simulations; i++) {
            path_final_min_max(spot, steps, sdt, drift, volatility, &vmm);
            if (knockin && knockin > spot && vmm.maximum < knockin) {
            } else if (knockin && knockin < spot && vmm.minimum > knockin) {
            } else if (knockout && knockout < spot && vmm.minimum < knockin) {
            } else if (knockout && knockout > spot && vmm.maximum > knockout) {
            } else {
                double v = cp * (vmm.value - strike);
                if (v > 0) {
                    total += v;
                }
            }
        }

        return total / simulations * std::exp(-time * risk_free);
    }
}

int main() {
  srand((unsigned)time_t(NULL));
  std::cout << barrierpricer::price_option(
          105,
          100,
          1,
          0.2,
          0.05,
          'c',
          0,
          0,
          50000,
          365
          );
  return 0;
}
