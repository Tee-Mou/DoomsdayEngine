#include "../inc/Utils.h"
#include <random>

int Utils::ctz(u64 num) {
    /**
    * @brief Count the trailing zeroes in a binary number.
    * @param num: Binary number.
    *
    * @return An integer count of trailing zeroes.
    */
    int count = 0;
    while (num > 0) {
        if (num & 1) { break; }
        num >>= 1;
        ++count;
    }
    return count;
}

int Utils::csb(u64 num) {
    /**
    * @brief Count the set bits (1s) in a binary number.
    * @param num: Binary number.
    *
    * @return An integer count of set bits.
    */
    int count = 0;
    while (num > 0) {
        count++;
        num &= num - 1;
    }
    return count;
}

u64 Utils::gen64(u64 seed) {
    /**
    * @brief Generate a random number using multiply/module method.
    * @param seed: Initial seed to use for generation.
    *
    * @return A randomly generated 64 bit number.
    */
    u64 m = 1ULL << 32;
    u64 a = 0x93d765ddULL;
    seed = (seed * a) % m;
    seed = seed + (seed << 32);
    return seed;
}

u64 Utils::genMagic() {
    /** 
    * @brief Generate a magic number for use in calculating sliding moves.
    * Uses the Mersenne Twister algorithm to generate random magic numbers.
    * Take the AND of three generated numbers to improve reliability.
    *
    * @return A randomly generated 64 bit magic number.
    */
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<u64> dist;

    u64 magicNumber = dist(gen) & dist(gen) & dist(gen);
    return magicNumber;
}