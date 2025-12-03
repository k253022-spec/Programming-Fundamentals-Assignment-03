#include<stdio.h>

void calculateFuel(int fuel, int consumption, int recharge, int solarBonus, int planet, int totalPlanets)
{
    if(fuel <= 0 || planet > totalPlanets)
	{
        if(fuel <= 0)
            printf("Fuel exhausted. Mission failed.\n");
        else
            printf("Mission successful.\n");
        return;
    }

    fuel = fuel - consumption;

    if(planet % 2 == 0)
        fuel += recharge;

    if (planet % 4 == 0)
        fuel += solarBonus;

    printf("Planet %d: Fuel Remaining = %d\n", planet, fuel);

    calculateFuel(fuel, consumption, recharge, solarBonus, planet + 1, totalPlanets);
}

int main()
{
    int fuel = 500;
    int consumption = 60;
    int recharge = 20;
    int solarBonus = 50;
    int totalPlanets = 10;

    calculateFuel(fuel, consumption, recharge, solarBonus, 1, totalPlanets);

    return 0;
}
