#include <cmath>

#include "utils.h"
#include "modulator.h"

ModulatorQAM::ModulatorQAM(int modulation_mode, int frame_size, const std::vector<int>& input)
    : modulation_mode_(modulation_mode),
      number_of_symbols_(frame_size / (2 * modulation_mode)),
      norm_factor_(utils::GetNormalizationFactor(modulation_mode))
{
    data_.resize(number_of_symbols_);

    const size_t symbol_length = 2 * modulation_mode;

    size_t j = 0;
    for (auto i = 0; i < data_.size(); ++i) {
        data_[i].resize(symbol_length);

        for (auto k = 0; k < symbol_length; ++k) {
            data_[i][k] = input.at(j + k);
        }
        j += symbol_length;
    }
}

std::vector<std::complex<double>> ModulatorQAM::TxSig() const {
    std::vector<std::complex<double>> result(number_of_symbols_);

    if (modulation_mode_ == 1) {
        for (int i = 0; i < number_of_symbols_; ++i) {
            result[i] = std::complex<double>(
                data_[i][0] == 0 ? -norm_factor_ : norm_factor_,
                data_[i][1] == 0 ? -norm_factor_ : norm_factor_);
        }
    }

    else if (modulation_mode_ == 2) {
        for (int i = 0; i < number_of_symbols_; ++i) {
            double real_part = (2 * data_[i][0] - 1) * (3 - 2 * data_[i][1]);
            double imag_part = (2 * data_[i][2] - 1) * (3 - 2 * data_[i][3]);
            result[i] = std::complex<double>(real_part * norm_factor_,
                                             imag_part * norm_factor_);
        }
    }

    else {
        for (int i = 0; i < number_of_symbols_; ++i) {
            int b0 = data_[i][0], b1 = data_[i][1], b2 = data_[i][2];
            double real_part = (2 * b0 - 1) * (7 - 6 * b1 - 2 * b2 + 4 * b1 * b2);

            int b3 = data_[i][3], b4 = data_[i][4], b5 = data_[i][5];
            double imag_part = (2 * b3 - 1) * (7 - 6 * b4 - 2 * b5 + 4 * b4 * b5);

            result[i] = std::complex<double>(real_part * norm_factor_,
                imag_part * norm_factor_);
        }
    }

    return result;
}
