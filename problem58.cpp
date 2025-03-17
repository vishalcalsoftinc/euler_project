#include <bits/stdc++.h>
using namespace std;

bool isPrime(long long n)
{
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return (n == 2);
    for (long long i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;
    return true;
}

int main()
{
    int primeCount = 0; // Count of primes on the diagonals
    int totalCount = 1; // Count of numbers on the diagonals (start with center 1)
    int sideLength = 1; // Starting side length
    int curr = 1;

    while (true)
    {
        sideLength += 2; // Each new layer increases the side length by 2

        // Calculate the four corner numbers of the current layer
        curr += (sideLength - 1);
        long long topRight = curr;
        curr += (sideLength - 1);
        long long topLeft = curr;
        curr += (sideLength - 1);
        long long bottomLeft = curr;
        curr += (sideLength - 1);
        long long bottomRight = curr;

        // Count the primes
        if (isPrime(topRight))
            primeCount++;
        if (isPrime(topLeft))
            primeCount++;
        if (isPrime(bottomLeft))
            primeCount++;
        if (isPrime(bottomRight))
            primeCount++;

        totalCount += 4;

        // Check if the ratio of primes falls below 10%
        if ((double)primeCount / totalCount < 0.10 && sideLength > 1)
        {
            cout << sideLength << endl;
            break;
        }
    }

    return 0;
}
