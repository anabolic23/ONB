#include <iostream>
#include "ONB.h"

// Default constructor
NBGaloisField::NBGaloisField() : element(0) {}

// Constructor with a std::bitset input
NBGaloisField::NBGaloisField(const std::bitset<281>& input) : element(input) {}

// Constructor with a string input (assuming it's a binary string)
NBGaloisField::NBGaloisField(const std::string& str, bool isHex) {
    if (isHex) {
        *this = convertFromHex(str);
    }
    else {
        *this = convertFromString(str);
    }
}

NBGaloisField NBGaloisField::convertFromHex(const std::string& hexString) {
    size_t requiredSize = (degree + 3) / 4; // Calculate the required size of the hex string
    if (hexString.size() != requiredSize) {
        std::cerr << "Invalid hexadecimal string format or length." << std::endl;
        return NBGaloisField();
    }

    std::string binaryString;
    for (size_t i = 2; i < hexString.size(); ++i) {
        char c = std::toupper(hexString[i]);
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))) {
            std::cerr << "Invalid character in hexadecimal string: " << c << std::endl;
            return NBGaloisField();
        }

        int value = (c >= 'A') ? (c - 'A' + 10) : (c - '0');
        binaryString += std::bitset<4>(value).to_string();
    }

    return NBGaloisField(binaryString, false);
}

NBGaloisField NBGaloisField::convertFromString(const std::string& binaryString) {
    if (binaryString.size() != degree) {
        std::cerr << "Input string length must be 281 bits." << std::endl;
        return NBGaloisField();
    }

    NBGaloisField gf;
    for (size_t i = 0; i < degree; ++i) {
        if (binaryString[i] == '1') {
            gf.element.set(degree - 1 - i, true);
        }
        else if (binaryString[i] != '0') {
            std::cerr << "Invalid character in binary string: " << binaryString[i] << std::endl;
            return NBGaloisField();
        }
    }

    return gf;
}

std::string NBGaloisField::convertToString() const {
    std::string elementString;
    for (int i = 280; i >= 0; --i) {
        elementString += (element[i] ? '1' : '0');
    }
    return elementString;
}

std::string NBGaloisField::convertToHex() const {
    std::string hexRepresentation;
    const size_t bits = degree;

    for (int i = 0; i < bits; i += 4) {
        int hexValue = 0;
        constexpr int hexDigitSize = 4;

        for (int j = 0; j < hexDigitSize && (i + j) < bits; ++j) {
            if (element.test(i + j)) {
                hexValue |= 1 << j;
            }
        }

        const char hexChar = hexValue < 10 ? '0' + hexValue : 'A' + hexValue - 10;
        hexRepresentation = hexChar + hexRepresentation;
    }

    return hexRepresentation;
}

NBGaloisField NBGaloisField::add(const NBGaloisField& other) const {
    std::bitset<281> result = this->element ^ other.element; // XOR operation
    return NBGaloisField(result);
}

NBGaloisField NBGaloisField::square() const {
    std::bitset<281> result = element;
    result = (result >> 1) | (result << 280); // Perform a cyclic right shift
    return NBGaloisField(result);
}

NBGaloisField NBGaloisField::trace() const {
    int trace = 0;
    for (int i = 0; i < degree; ++i) {
        if (element.test(i)) {
            trace = (trace + 1) % 2; // Addition in GF(2) - equivalent to XOR
        }
    }
    // Create an NBGaloisField object with the trace value
    std::bitset<281> traceBitset;
    traceBitset.set(0, trace);
    return NBGaloisField(traceBitset);
}

std::vector<std::vector<int>> computeMultiplicationMatrix() {
    std::vector<std::vector<int>> multiplicationMatrix(NBGaloisField::degree, std::vector<int>(NBGaloisField::degree));
    int array[NBGaloisField::degree]{ 0 };
    int p = NBGaloisField::degree * 2 + 1;
    array[0] = 1;

    for (int i = 1; i < NBGaloisField::degree; ++i) {
        array[i] = (array[i - 1] * 2) % p;
    }

    for (int i = 0; i < NBGaloisField::degree; ++i) {
        for (int j = 0; j < NBGaloisField::degree; ++j) {
            int cond1 = ((array[i] + array[j]) % p == 1) ? 1 : 0;
            int cond2 = ((array[i] - array[j] + p) % p == 1) ? 1 : 0;
            int cond3 = ((- array[i] + array[j] + p) % p == 1) ? 1 : 0;
            int cond4 = ((- array[i] - array[j] + p + p) % p == 1) ? 1 : 0;

            if (cond1 || cond2 || cond3 || cond4) {
                multiplicationMatrix[NBGaloisField::degree - i - 1][NBGaloisField::degree - j - 1] = 1;
            }
        }
    }

    return multiplicationMatrix;
}

NBGaloisField NBGaloisField::mult(const NBGaloisField& u, const NBGaloisField& v) const {
    std::vector<std::vector<int>> multiplicationMatrix = computeMultiplicationMatrix();
    std::bitset<281> result(0);

    for (int i = 0; i < NBGaloisField::degree; ++i) {
        std::bitset<281> shiftedU = (u.element << i) | (u.element >> (NBGaloisField::degree - i));
        std::bitset<281> uTimes(0);

        for (int j = 0; j < NBGaloisField::degree; ++j) {
            if (shiftedU.test(j)) {
                for (int k = 0; k < NBGaloisField::degree; ++k) {
                    if (multiplicationMatrix[j][k] == 1) {
                        uTimes.flip(k);
                    }
                }
            }
        }

        std::bitset<281> shiftedV = (v.element << i) | (v.element >> (NBGaloisField::degree - i));

        for (int j = 0; j < NBGaloisField::degree; ++j) {
            if (uTimes.test(j) && shiftedV.test(j)) {
                result.flip(NBGaloisField::degree - i - 1);
            }
        }
    }

    return NBGaloisField(result);
}

NBGaloisField NBGaloisField::pow(const NBGaloisField& exponent) const {
    NBGaloisField result;
    result.one();
    NBGaloisField base = *this; // The current element serving as the base

    std::bitset<281> expBits = exponent.element;

    for (int i = 0 ; i < NBGaloisField::degree; ++i) {
        if (expBits.test(i)) {
            result = result.mult(base, result); // Multiply by the base when the corresponding bit is set
        }
        base = base.square();
    }

    return result;
}

void NBGaloisField::zero() {
    element = std::bitset<281>();
}

void NBGaloisField::one() {
    element = std::bitset<281>("11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
}

NBGaloisField NBGaloisField::inverse() const {
    NBGaloisField b(element);
    NBGaloisField temp;

    int k = 1;
    // m - 1 = 280 = 100011000 (t = 8)
    for (int i = 7; i >= 0; --i) {
        temp = b;
        for (int j = 0; j < k; ++j) {
            b = b.square();
        }
        b = b.mult(temp, b);
        k = 2 * k;

        if (((NBGaloisField::degree - 1) >> i) & 1) {
            b = b.square();
            b = b.mult(*this, b);
            k = k + 1;
        }
    }
    return b.square();
}

void NBGaloisField::generateRandomBinaryString(size_t size) {
    if (size > degree)
        size = degree;

    float probability = 0.5;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(probability);
    for (size_t i = 0; i < size; i++)
        element[i] = d(gen);
}