#pragma once

#include <vector>
#include <complex>

class ModulatorQAM {
public:
    ModulatorQAM(int modulation_mode, int frame_size, const std::vector<int>& input);
    ~ModulatorQAM() = default;

    std::vector<std::complex<double>> TxSig() const;

    inline int GetNumberOfSymbols() const { return number_of_symbols_; }

    inline int GetModulationMode() const { return modulation_mode_; }

protected:
    int modulation_mode_ = 0;
    int number_of_symbols_ = 0;
    double norm_factor_ = 1.0;
    std::vector<std::vector<int>> data_;
};
