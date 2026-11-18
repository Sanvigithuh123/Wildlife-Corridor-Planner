
#include <stdio.h>
#include <math.h>
#include "fast_power.h"

// Recursive exponentiation by squaring
double fastPower(double base, int exponent) {

    if (exponent == 0)
        return 1.0;

    if (exponent < 0) {
        long long positiveExponent = -(long long)exponent;
        double result = 1.0;
        double factor = base;

        if (base == 0.0)
            return NAN;

        while (positiveExponent > 0) {
            if (positiveExponent % 2 == 1)
                result *= factor;

            factor *= factor;
            positiveExponent /= 2;
        }

        return 1.0 / result;
    }

    double half = fastPower(base, exponent / 2);

    if (exponent % 2 == 0)
        return half * half;

    return base * half * half;
}

void populationProjection(
    double initialPopulation,
    double growthRate,
    int years
) {
    if (initialPopulation < 0 ||
        growthRate < -1.0 ||
        years < 0 ||
        !isfinite(initialPopulation) ||
        !isfinite(growthRate)) {

        printf("Invalid population projection input.\n");
        return;
    }

    double factor = fastPower(
        1.0 + growthRate,
        years
    );

    double projected = initialPopulation * factor;

    if (!isfinite(projected)) {
        printf("Projection exceeds numeric range.\n");
        return;
    }

    printf("\n===== POPULATION PROJECTION =====\n");
    printf("Initial Population: %.2f\n",
           initialPopulation);
    printf("Growth Rate: %.2f%%\n",
           growthRate * 100.0);
    printf("Years: %d\n", years);
    printf("Projected Population: %.2f\n",
           projected);
}
