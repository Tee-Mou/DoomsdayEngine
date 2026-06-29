#pragma once

using u64 = unsigned long long;

class Utils
{
    public:
        static int ctz(u64 num); // Count trailing zeroes
        static int csb(u64 num); // Count set bits
        static u64 gen64(u64 seed); // Generate a seeded 64 bit number
        static u64 genMagic(); // Generate a magic number for sliding moves
};