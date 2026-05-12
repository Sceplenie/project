#include "utils.h"

namespace utils {

double GetNormalizationFactor(int modulation_mode) {
    switch (modulation_mode) {
    case 1:  // QPSK
        return 1.0 / std::sqrt(2);
    case 2:  // 16-QAM
        return 1.0 / std::sqrt(10);
    case 3:  // 64-QAM
        return 1.0 / std::sqrt(42);
    default:
        throw std::invalid_argument("Unsupported modulation mode");
    }
}

}  // namespace utils