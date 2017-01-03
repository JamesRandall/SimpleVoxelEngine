#pragma once
#include <vector>

class PerlinNoise
{
	int _repeat;
	std::vector<unsigned int> _permutation;
public:
	PerlinNoise(int repeat=-1);
	~PerlinNoise();
	double octavePerlin(double x, double y, double z, int octaves, double persistence);

private:
	double perlin(double x, double y, double z);
	int inc(int num) const;
	static double grad(int hash, double x, double y, double z);
	static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
	static double lerp(double a, double b, double x) { return a + x * (b - a); }
};

