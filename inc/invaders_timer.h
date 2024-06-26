#pragma once

#include <chrono>

namespace Game {

    // get current elapsed time
    inline float time()
    {
        using namespace std::chrono;
        static const auto start = high_resolution_clock::now();
        return duration<float>(high_resolution_clock::now() - start).count();
    }

};
