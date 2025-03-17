#include <iostream>
#include <iomanip>
#include <algorithm>

// -------------------------------------------------------
// FRACTRAN visualization (verbose, not used in fast mode)
// -------------------------------------------------------
unsigned long long enumerate(unsigned int numPrimes, bool displaySteps = true)
{
    const int NumFractions = 14;
    const int NumExponents = 10;
    const char Fractions[NumFractions][NumExponents] =
        {
            // Represent fractions as exponents of these primes:
            //         2, 3, 5, 7, 11, 13, 17, 19, 23, 29
            // 17/91 =         1/7   *1/13*17
            {0, 0, 0, -1, 0, -1, +1, 0, 0, 0},
            // 78/85 = 2 * 3 *1/5        *13*1/17
            {+1, +1, -1, 0, 0, +1, -1, 0, 0, 0},
            // 19/51 =     1/3               *1/17*19
            {0, -1, 0, 0, 0, 0, -1, +1, 0, 0},
            // 23/38 = 1/2                   *1/19*23
            {-1, 0, 0, 0, 0, 0, 0, -1, +1, 0},
            // 29/33 =     1/3    *1/11          *29
            {0, -1, 0, 0, -1, 0, 0, 0, 0, +1},
            // 77/29 =           7*11            *1/29
            {0, 0, 0, +1, +1, 0, 0, 0, 0, -1},
            // 95/23 =       5          *19*1/23
            {0, 0, +1, 0, 0, 0, 0, +1, -1, 0},
            // 77/19 =           7*11   *1/19
            {0, 0, 0, +1, +1, 0, 0, -1, 0, 0},
            //  1/17 =                  *1/17
            {0, 0, 0, 0, 0, 0, -1, 0, 0, 0},
            // 11/13 =         11*1/13
            {0, 0, 0, 0, +1, -1, 0, 0, 0, 0},
            // 13/11 =        1/11*13
            {0, 0, 0, 0, -1, +1, 0, 0, 0, 0},
            // 15/2  = 1/2 * 3 * 5
            {-1, +1, +1, 0, 0, 0, 0, 0, 0, 0},
            //  1/7  =          1/7
            {0, 0, 0, -1, 0, 0, 0, 0, 0, 0},
            // 55/1  =       5    *11
            {0, 0, +1, 0, +1, 0, 0, 0, 0, 0}};

    int current[NumExponents] = {+1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned long long steps = 0;
    unsigned int numFound = 0;

    while (numFound < numPrimes)
    {
        if (displaySteps)
        {
            std::cout << "step " << std::setw(3) << steps << ": ";
            for (auto x : current)
                std::cout << std::setw(2) << (x == 0 ? "-" : std::to_string(x)) << " ";
            std::cout << std::endl;
        }
        for (const auto &fraction : Fractions)
        {
            bool match = true;
            for (int i = 0; i < NumExponents; i++)
                match &= (current[i] + fraction[i]) >= 0;
            if (match)
            {
                for (int i = 0; i < NumExponents; i++)
                    current[i] += fraction[i];
                break;
            }
        }
        steps++;
        bool isPrimeState = true;
        for (int i = 1; i < NumExponents; i++)
            isPrimeState &= (current[i] == 0);
        if (isPrimeState)
        {
            numFound++;
            if (displaySteps)
                std::cout << "prime " << current[0] << " @ step " << steps << std::endl;
        }
    }
    return steps;
}

// -------------------------------------------------------
// Optimized state-machine implementation
// -------------------------------------------------------
unsigned long long search(unsigned int numPrimes)
{
#define OPTIMIZED

    enum State
    {
        S_,
        S11,
        S13,
        S17,
        S19,
        S23,
        S29
    };
    State state = S_;

    int two = 1, three = 0, five = 0, seven = 0;
    unsigned long long steps = 0;
    unsigned int numFound = 0;

    while (true)
    {
        switch (state)
        {
        case S_:
            if (three == 0 && five == 0 && seven == 0 && steps > 0)
            {
                numFound++;
                if (numFound == numPrimes)
                    return steps;
            }
            if (two > 0)
            {
                two--;
                three++;
                five++;
                break;
            }
            if (seven > 0)
            {
                seven--;
                break;
            }
            five++;
            state = S11;
            break;

        case S11:
            if (three > 0)
            {
#ifdef OPTIMIZED
                steps += 2 * three;
                seven += three;
                three = 0;
                continue;
#endif
                three--;
                state = S29;
                break;
            }
            state = S13;
            break;

        case S13:
            if (seven > 0)
            {
#ifdef OPTIMIZED
                if (five > 0)
                {
                    int smallest = std::min(five, seven);
                    steps += 2 * smallest;
                    two += smallest;
                    three += smallest;
                    five -= smallest;
                    seven -= smallest;
                    continue;
                }
#endif
                seven--;
                state = S17;
                break;
            }
            state = S11;
            break;

        case S17:
            if (five > 0)
            {
                five--;
                two++;
                three++;
                state = S13;
                break;
            }
            if (three > 0)
            {
                three--;
                state = S19;
                break;
            }
            state = S_;
            break;

        case S19:
            if (two > 0)
            {
#ifdef OPTIMIZED
                steps += 2 * two;
                five += two;
                two = 0;
                continue;
#endif
                two--;
                state = S23;
                break;
            }
            seven++;
            state = S11;
            break;

        case S23:
            five++;
            state = S19;
            break;

        case S29:
            seven++;
            state = S11;
            break;
        }
        steps++;
    }
}

int main()
{
    unsigned int numPrimes = 10001;
    // Uncomment the next line to view the visualization
    // std::cout << enumerate(numPrimes, true) << std::endl;

    std::cout << search(numPrimes) << std::endl;
    return 0;
}
