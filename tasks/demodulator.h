#pragma once

#include <vector>
#include <complex>

class DemodulatorQAM {
public:
	DemodulatorQAM(int modulation_mode,
                   int number_of_symbols,
		           const std::vector<std::complex<double>>& data);
    ~DemodulatorQAM() = default;

    std::vector<int> GetBits() const;

protected:
    int number_of_symbols_ = 0;
    int modulation_mode_ = 0;
    double norm_factor_ = 1.0;
    std::vector<std::complex<double>> data_;
};
