#include <cmath>
#include <iostream>

#include "awgn.h"

AWGN::AWGN(double mean) : mean_(mean) {}

std::vector<std::complex<double>>
AWGN::ApplyNoise(const std::vector<std::complex<double>>& tx_sig, double sigma) const {
    std::mt19937 rng(std::random_device{}());
    std::normal_distribution<double> noise(mean_, sigma);

    std::vector<std::complex<double>> ch_sig(tx_sig.size());
    for (size_t i = 0; i < tx_sig.size(); ++i) {
        const auto value = tx_sig[i] + std::complex<double>(noise(rng), noise(rng));
        ch_sig[i] = value;
    }

    return ch_sig;
}
