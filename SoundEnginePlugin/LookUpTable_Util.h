#pragma once

#include <AK/SoundEngine/Common/IAkPlugin.h>
#include <array>
#include <cmath>
#include <limits>

class UtilLookupTable {
public:
    static constexpr AkUInt32 TABLE_SIZE = 1024;
    static constexpr AkReal32 PI = 3.14159265358979323846f;
    static constexpr AkReal32 TWO_PI = 2.0f * PI;

    UtilLookupTable() {
        for (AkUInt32 i = 0; i < TABLE_SIZE; ++i) {
            AkReal32 ratio = static_cast<AkReal32>(i) / TABLE_SIZE;

            // Sine value
            AkReal32 angle = ratio * TWO_PI;
            sinTable[i] = std::sin(angle);

            // Triangle value
            if (ratio < 0.25f) {
                triangleTable[i] = ratio * 4.0f;
            }
            else if (ratio < 0.75f) {
                triangleTable[i] = 2.0f - ratio * 4.0f;
            }
            else {
                triangleTable[i] = ratio * 4.0f - 4.0f;
            }

            // Saw value
            sawTable[i] = 2.0f * ratio - 1.0f;
        }
    }

    AkReal32 sin(AkReal32 angle) const {
        return lookup(angle, sinTable);
    }

    AkReal32 cos(AkReal32 angle) const {
        return sin(angle + 0.25f);
    }

    AkReal32 tan(AkReal32 angle) const {
        AkReal32 cosVal = cos(angle);
        if (std::abs(cosVal) < 1e-6f) {
            return (cosVal < 0.0f) ? -std::numeric_limits<AkReal32>::infinity() : std::numeric_limits<AkReal32>::infinity();
        }
        return sin(angle) / cosVal;
    }


    AkReal32 saw(AkReal32 angle) const {
        return lookup(angle, sawTable);
    }

    AkReal32 triangle(AkReal32 angle) const {
        return lookup(angle, triangleTable);
    }

    AkReal32 Oscillator(AkUInt32 OscNumber, AkReal32 angle)
    {
        switch (OscNumber)
        {
        case 0 :
            return sin(angle);
        case 1 :
            return saw(angle);
        case 2 :
            return triangle(angle);
        default:
            break;
        }
    }

    AkUInt32 Timer = 0;
private:
    std::array<AkReal32, TABLE_SIZE> sinTable;
    std::array<AkReal32, TABLE_SIZE> triangleTable;
    std::array<AkReal32, TABLE_SIZE> sawTable;

    

    AkReal32 lookup(AkReal32 angle, const std::array<AkReal32, TABLE_SIZE>& table) const {
        AkReal32 indexAkReal32 = angle * TABLE_SIZE;
        AkUInt32 index = static_cast<AkUInt32>(indexAkReal32);

        AkReal32 mix = indexAkReal32 - index;
        AkReal32 a = table[index];
        AkReal32 b = table[(index + 1) % TABLE_SIZE];

        // linear interpolation
        return a * (1 - mix) + b * mix;
    }
};