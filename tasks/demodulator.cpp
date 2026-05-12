#include <cmath>

#include "utils.h"
#include "demodulator.h"

DemodulatorQAM::DemodulatorQAM(int modulation_mode,
                               int number_of_symbols,
                               const std::vector<std::complex<double>>& data)
    : number_of_symbols_(number_of_symbols),
      modulation_mode_(modulation_mode),
      norm_factor_(utils::GetNormalizationFactor(modulation_mode)),
      data_(data) {}

std::vector<int> DemodulatorQAM::GetBits() const {
    std::vector<int> result;

    if (modulation_mode_ == 1) {
        for (int i = 0; i < number_of_symbols_; ++i) {
            result.push_back(data_[i].real() >= 0 ? 1 : 0);
            result.push_back(data_[i].imag() >= 0 ? 1 : 0);
        }
    }

    else if (modulation_mode_ == 2) {
        const double norm = 1.0 / std::sqrt(10.0);
        const double norm_2 = 2.0 * norm;

        for (int i = 0; i < number_of_symbols_; ++i) {
            for (auto val : { data_[i].real(), data_[i].imag() }) {
                if (val >= 0) {
                    result.push_back(1);
                    result.push_back(val >= norm_2 ? 0 : 1);
                }
                else {
                    result.push_back(0);
                    result.push_back(val <= (-norm_2) ? 0 : 1);
                }
            }
        }
    }

    else {
        const double norm = 1.0 / std::sqrt(42.0);
        const double thr6 = 6.0 * norm;
        const double thr4 = 4.0 * norm;
        const double thr2 = 2.0 * norm;

        for (int i = 0; i < number_of_symbols_; ++i) {
            for (auto val : { data_[i].real(), data_[i].imag() }) {
                if (val >= 0) {
                    result.push_back(1);
                    if (val >= thr6) {
                        result.push_back(0); result.push_back(0);  // 1 0 0
                    }
                    else if (val >= thr4) {
                        result.push_back(0); result.push_back(1);  // 1 0 1
                    }
                    else if (val >= thr2) {
                        result.push_back(1); result.push_back(1);  // 1 1 1
                    }
                    else {
                        result.push_back(1); result.push_back(0);  // 1 1 0
                    }
                }
                else {
                    result.push_back(0);
                    if (val <= -thr6) {
                        result.push_back(0); result.push_back(0);  // 0 0 0
                    }
                    else if (val <= -thr4) {
                        result.push_back(0); result.push_back(1);  // 0 0 1
                    }
                    else if (val <= -thr2) {
                        result.push_back(1); result.push_back(1);  // 0 1 1
                    }
                    else {
                        result.push_back(1); result.push_back(0);  // 0 1 0
                    }
                }
            }
        }
    }

    return result;
}
