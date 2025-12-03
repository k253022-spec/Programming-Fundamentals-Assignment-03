#include<stdio.h>

float calculateRepayment(float loan, float interestRate, int years, float installment)
{
    if(loan <= 0 || years == 0)
        return 0;

    loan = loan - installment;
    if(loan < 0) loan = 0;

    loan = loan + (loan * interestRate);

    printf("Year %d: Remaining loan = %.2f\n", years, loan);

    return installment + calculateRepayment(loan, interestRate, years - 1, installment);
}

int main()
{
    float loan = 100000;
    float interestRate = 0.05;
    int years = 3;
    float installment = 30000;

    float total = calculateRepayment(loan, interestRate, years, installment);

    printf("Total repayment = %.2f", total);
    return 0;
}
