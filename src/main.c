#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../data/dehli_raw.h"
#include "../data/turbine_raw.h"

#include "main.h"

int main(void)
{

    clock_t start_time, end_time, total;
    double total_time;
    double rmse;

    start_time = clock();
    // rmse = linear_regression_train_and_test(turbine_raw);
    end_time = clock();

    total_time += ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    printf("LINEAR REGRESSION:\n");
    printf("TIME ELAPSED: %.4f\n", total_time);
    printf("RMSE: %.2f\n", rmse);

    return 0;
}