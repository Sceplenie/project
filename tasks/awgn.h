#pragma once

#include <random>
#include <vector>
#include <complex>

class AWGN {
public:
	explicit AWGN(double mean);

	std::vector<std::complex<double>>
		ApplyNoise(const std::vector<std::complex<double>>& data, double sigma) const;

private:
	double mean_ = 0.0;
};