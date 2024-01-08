#pragma once
#include <iostream>
#include <string>
#include <bitset>
#include <chrono>
#include <random>

class NBGaloisField {
private:
    std::bitset<281> element;

public:
    static constexpr unsigned int degree = 281;

    // Constructors
    NBGaloisField();
    NBGaloisField(const std::bitset<281>& input);
    NBGaloisField(const std::string& str, bool isHex = false);

    // Operations
    NBGaloisField add(const NBGaloisField& other) const;
    NBGaloisField mult(const NBGaloisField& u, const NBGaloisField& v) const;
    NBGaloisField square() const;
    NBGaloisField pow(const NBGaloisField& exponent) const;
    NBGaloisField inverse() const;
    NBGaloisField trace() const;

    // Convertor
    static NBGaloisField convertFromString(const std::string& inputString);
    static NBGaloisField convertFromHex(const std::string& hexString);
    std::string convertToString() const;
    std::string convertToHex() const;

    // Constants
    void zero();
    void one();

    // Generator
    void generateRandomBinaryString(size_t size);

};