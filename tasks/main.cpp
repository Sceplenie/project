#include <vector>
#include <iostream>
#include <utility>
#include <cmath>
#include <map>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <random>
#include <string>

#include "awgn.h"
#include "demodulator.h"
#include "modulator.h"

const int FRAME_SIZE = 2400;
const int MAX_ERRORS = 400;
const double AWGN_MEAN = 0.0;
const int MAX_BITS = static_cast<int>(std::pow(10, 5));

const std::string BER_RESULTS_FILENAME = "ber_results.json";

const std::vector<double> SIGMAS = {
    0.5, 0.48, 0.46, 0.44, 0.42, 0.40, 0.38, 0.36, 0.34, 0.32,
    0.30, 0.28, 0.26, 0.24, 0.22, 0.20, 0.18, 0.16, 0.14, 0.12, 0.10
};

const std::vector<std::pair<int, std::string>> MODULATION_MODES = {
    {1, "QPSK"},
    {2, "16QAM"},
    {3, "64QAM"},
};

std::vector<int> GenerateRandomBits(int size) {
    std::vector<int> result(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1);

    for (int i = 0; i < size; ++i)
        result[i] = dist(gen);

    return result;
}

int CountErrors(const std::vector<int>& input,
    const std::vector<int>& output,
    int number_of_bits) {
    int result = 0;
    for (int i = 0; i < number_of_bits; ++i)
        if (input[i] != output[i])
            ++result;
    return result;
}

/// Returns pair {errors, bits}.
std::pair<int, int> CalculateErrors(double sigma, int modulationMode, int frame_size) {
    int errors = 0;
    int bits = 0;

    while ((errors < MAX_ERRORS) && (bits < MAX_BITS)) {
        const auto input = GenerateRandomBits(frame_size);

        const auto modulator = ModulatorQAM(modulationMode, frame_size, input);
        const auto tx_sig = modulator.TxSig();

        const auto awgn = AWGN(AWGN_MEAN);
        const auto ch_sig = awgn.ApplyNoise(tx_sig, sigma);

        const auto demodulator = DemodulatorQAM(modulationMode, modulator.GetNumberOfSymbols(), ch_sig);
        const auto output = demodulator.GetBits();

        errors += CountErrors(input, output, static_cast<int>(input.size()));
        bits += FRAME_SIZE;
    }

    return { errors, bits };
}

void WriteJson(const std::string& filename,
               const std::map<int, std::map<double, double>>& data) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Failed to open " << filename << " for writing\n";
        return;
    }

    out << "{\n";
    bool firstMode = true;

    for (const auto& [mode, sigmaMap] : data) {
        if (!firstMode) out << ",\n";
        firstMode = false;

        out << "  \"" << mode << "\": {\n";

        bool firstSigma = true;
        for (const auto& [sigmaStr, ratio] : sigmaMap) {
            if (!firstSigma) out << ",\n";
            firstSigma = false;

            out << "    \"" << sigmaStr << "\": " << std::setprecision(15) << ratio;
        }

        out << "\n  }";
    }

    out << "\n}\n";
}

int main() {
    std::map<int, std::map<double, double>> results;

    for (const auto& [mode, name] : MODULATION_MODES) {
        std::cout << "Calculating for modulation mode: " << name << std::endl;

        for (double sigma : SIGMAS) {
            const auto [errors, bits] = CalculateErrors(sigma, mode, FRAME_SIZE);
            const double ratio = (bits > 0) ? static_cast<double>(errors) / bits : 0.0;

            results[mode][sigma] = ratio;

            std::cout << "\n\tSigma = " << sigma
                << "\tErrors = " << errors
                << "\tBits = " << bits
                << "\tErrors/Bits ratio = " << ratio;
        }

        std::cout << std::endl;
    }

    WriteJson(BER_RESULTS_FILENAME, results);

    std::cout << "\nJSON written to: "
        << std::filesystem::absolute(BER_RESULTS_FILENAME)
        << std::endl;

    return 0;
}