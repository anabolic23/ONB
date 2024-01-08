#include <iostream>
#include "ONB.h" 

int main() {
    // Test constructor with string input (binary)
    std::string testBinaryString1 = "10100111001101110111101111001111110101000001000010100011101001101011010010100101110100110011011011001011101101001101100101101110011100100010110000110011111110001110011100000010110000111111000000000001001011100000001000101010000000101010110001110010111111011101011000000001010101011";
    NBGaloisField field1(testBinaryString1, false);
    std::cout << "Element initialized with binary string1: " << field1.convertToString() << std::endl;

    // Test constructor with string input (binary)
    std::string testBinaryString2 = "11111101100010010100011001011100111101111101010101010100001110110011001101001011001111101100011011111010001001000100011100011000111001000011101001100000000001001010000111101001110100010010111001000110010111111011111001011101100111000010110001000100011100100011001100110011010001111";
    NBGaloisField field2(testBinaryString2, false);
    std::cout << "Element initialized with binary string2: " << field2.convertToString() << std::endl;

    // Test hex convert
    std::cout << "Hexadecimal representation of field1: " << field1.convertToHex() << std::endl;

    // Test hex convert
    std::cout << "Hexadecimal representation of field2: " << field2.convertToHex() << std::endl;

    // Test addition operation
    NBGaloisField sum = field1.add(field2);
    std::cout << "Sum of field1 and field2: " << sum.convertToString() << std::endl;
    std::cout << "Sum of field1 and field2(hex): " << sum.convertToHex() << std::endl;

    // Test square operation
    NBGaloisField squaredField1 = field1.square();
    std::cout << "Square of field1: " << squaredField1.convertToString() << std::endl;
    std::cout << "Square of field1(hex): " << squaredField1.convertToHex() << std::endl;
    
    // Test trace
    NBGaloisField traceField1 = field1.trace();
    std::cout << "Trace of field1: " << traceField1.convertToString() << std::endl;
    std::cout << "Trace of field1(hex): " << traceField1.convertToHex() << std::endl;

    // Test mult operation
    NBGaloisField mult = field1.mult(field1, field2);
    std::cout << "Mult of field1 and field2: " << mult.convertToString() << std::endl;
    std::cout << "Mult of field1 and field2(hex): " << mult.convertToHex() << std::endl;
    
    // Test pow operation
    NBGaloisField pow = field1.pow(field2);
    std::cout << "Pow of field1 and field2: " << pow.convertToString() << std::endl;
    std::cout << "Pow of field1 and field2(hex): " << pow.convertToHex() << std::endl;

    // Test inverse operation
    NBGaloisField inverse = field1.inverse();
    std::cout << "Field1 inversed: " << inverse.convertToString() << std::endl;
    std::cout << "Field1 inversed(hex): " << inverse.convertToHex() << std::endl;

    NBGaloisField  result1, result2;

    NBGaloisField a;
    a.generateRandomBinaryString(281);

    NBGaloisField b;
    b.generateRandomBinaryString(281);

    NBGaloisField c;
    c.generateRandomBinaryString(281);

    result1 = (a.add(b)).mult((a.add(b)),c); // (a + b) * c
    result2 = (a.mult(a, c)).add(b.mult(b, c)); // a * c + b * c

    if (result1.convertToString() == result2.convertToString()) {
        std::cout << "Right Distributivity Test Passed" << std::endl;
    }
    else {
        std::cout << "Right Distributivity Test Failed" << std::endl;
    }

    // Left Distributivity Test: c * (a + b) == c * a + c * b
    result1 = c.mult(c, a.add(b)); // c * (a + b)
    result2 = c.mult(c, a).add(c.mult(c, b)); // c * a + c * b

    if (result1.convertToString() == result2.convertToString()) {
        std::cout << "Left Distributivity Test Passed" << std::endl;
    }
    else {
        std::cout << "Left Distributivity Test Failed" << std::endl;
    }

    // Commutativity of Addition Test: a + b == b + a
    result1 = a.add(b); // a + b
    result2 = b.add(a); // b + a

    if (result1.convertToString() == result2.convertToString()) {
        std::cout << "Commutativity of Addition Test Passed" << std::endl;
    }
    else {
        std::cout << "Commutativity of Addition Test Failed" << std::endl;
    }

    // Commutativity of Multiplication Test: a * b == b * a
    result1 = a.mult(a, b); // a * b
    result2 = b.mult(b, a); // b * a

    if (result1.convertToString() == result2.convertToString()) {
        std::cout << "Commutativity of Multiplication Test Passed" << std::endl;
    }
    else {
        std::cout << "Commutativity of Multiplication Test Failed" << std::endl;
    }

    // Square Test: a.ToSquare() == a * a
    NBGaloisField squaredA = a.square(); // a.ToSquare()
    result1 = a.mult(a, a); // a * a

    if (squaredA.convertToString() == result1.convertToString()) {
        std::cout << "Square Test Passed" << std::endl;
    }
    else {
        std::cout << "Square Test Failed" << std::endl;
    }

    // Inverse Element Test: a * a.InverseElement() == 1
    NBGaloisField inverseA = a.inverse(); // a.InverseElement()
    result1 = a.mult(a, inverseA); // a * a.InverseElement()
    NBGaloisField identity;
    identity.one();

    if (result1.convertToString() == identity.convertToString()) {
        std::cout << "Inverse Element Test Passed" << std::endl;
    }
    else {
        std::cout << "Inverse Element Test Failed" << std::endl;
    }


    return 0;
}