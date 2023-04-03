#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <math.h>
#include "../data/delhi.h"
#include "../data/turbine.h"

#include "def.h"

double polynomial_regression_train_and_test(double data[SAMPLE_SIZE][DATASET_FEATURES], int feature_size, int sample_size, int degree);
double polynomial_regression_train_and_test_optimized(double data[SAMPLE_SIZE][DATASET_FEATURES], int feature_size, int sample_size, int degree);
void normalize_features(double data[SAMPLE_SIZE][DATASET_FEATURES], int feature_size, int sample_size);


int main(void)
{
	struct timeval start, end;
	long mtime, secs, usecs;
	double rmse;

	//clock function that calls the system clock
	gettimeofday(&start, NULL);

	// rmse = polynomial_regression_train_and_test(DATASET, DATASET_FEATURES, SAMPLE_SIZE, POLY_DEGREE);
	rmse = polynomial_regression_train_and_test_optimized(DATASET, DATASET_FEATURES, SAMPLE_SIZE, POLY_DEGREE);


	//call system clock again and calculate difference, in microseconds
	gettimeofday(&end, NULL);
	secs  = end.tv_sec  - start.tv_sec;
	usecs = end.tv_usec - start.tv_usec;
	mtime = ((secs) * 1000000 + usecs) + 0.5;

	//output metrics to standard output
	printf("POLYNOMIAL REGRESSION: DEGREE %d\n", POLY_DEGREE);
	printf("TIME ELAPSED: %lu\n", mtime);
	printf("RMSE: %.5f\n", rmse);
}

double polynomial_regression_train_and_test(double data[SAMPLE_SIZE][DATASET_FEATURES], int feature_size, int sample_size, int degree)
{
    // Calculate the number of terms for the polynomial regression
    int NUM_TERMS = (int)pow(degree + 1, feature_size - 1);

    // Allocate memory for the feature matrix and label array
    double **x = malloc(sample_size * sizeof(double *));
    double *y = malloc(sample_size * sizeof(double));

    // Populate feature matrix and label array
    for (int i = 0; i < sample_size; i++)
    {
        x[i] = malloc(NUM_TERMS * sizeof(double));
        int index = 0;

        for (int comb = 0; comb < NUM_TERMS; comb++)
        {
            int temp_comb = comb;
            double value = 1;

            // Calculate the product of features raised to their respective powers
            for (int feature = 0; feature < feature_size - 1; feature++)
            {
                int power = temp_comb % (degree + 1);
                value *= pow(data[i][feature], power);
                temp_comb /= (degree + 1);
            }

            x[i][index++] = value;
        }

        // Store label value
        y[i] = data[i][feature_size - 1];
    }

    // Allocate memory for x transpose matrix, x transpose times x matrix, and x transpose times y matrix
    double **x_transpose = malloc(NUM_TERMS * sizeof(double *));
    double **x_transpose_x = malloc(NUM_TERMS * sizeof(double *));
    double *x_transpose_y = malloc(NUM_TERMS * sizeof(double));

    // Compute x transpose matrix and x transpose times x matrix
    for (int i = 0; i < NUM_TERMS; i++)
    {
        x_transpose[i] = malloc(sample_size * sizeof(double));
        x_transpose_x[i] = malloc(NUM_TERMS * sizeof(double));

        // Compute x transpose matrix
        for (int j = 0; j < sample_size; j++)
        {
            x_transpose[i][j] = x[j][i];
        }

        // Compute x transpose times x matrix
        for (int j = 0; j < NUM_TERMS; j++)
        {
            x_transpose_x[i][j] = 0;

            for (int k = 0; k < sample_size; k++)
            {
                x_transpose_x[i][j] += x_transpose[i][k] * x[k][j];
            }
        }

        // Compute x transpose times y matrix
        x_transpose_y[i] = 0;

        for (int j = 0; j < sample_size; j++)
        {
            x_transpose_y[i] += x_transpose[i][j] * y[j];
        }
    }

    // Allocate memory for coefficients
    double *coefficients = malloc(NUM_TERMS * sizeof(double));

    // Gaussian elimination to solve the system of equations
    for (int i = 0; i < NUM_TERMS; i++)
    {
        for (int j = i + 1; j < NUM_TERMS; j++)
        {
            double ratio = x_transpose_x[j][i] / x_transpose_x[i][i];

            for (int k = i; k < NUM_TERMS; k++)
            {
                x_transpose_x[j][k] -= ratio * x_transpose_x[i][k];
            }

            x_transpose_y[j] -= ratio * x_transpose_y[i];
		}

    }

    // Back-substitution to find the coefficients
    for (int i = NUM_TERMS - 1; i >= 0; i--)
    {
        double sum = 0;

        for (int j = i + 1; j < NUM_TERMS; j++)
        {
            sum += x_transpose_x[i][j] * coefficients[j];
        }

        coefficients[i] = (x_transpose_y[i] - sum) / x_transpose_x[i][i];
    }

    // Calculate RMSE on training data
    double rmse = 0;

    for (int i = 0; i < sample_size; i++)
    {
        double y_pred = 0;

        for (int j = 0; j < NUM_TERMS; j++)
        {
            y_pred += coefficients[j] * x[i][j];
        }

        rmse += pow(y_pred - data[i][feature_size - 1], 2);
    }

    rmse = sqrt(rmse / sample_size);

    // Free memory allocated for arrays
    for (int i = 0; i < sample_size; i++)
    {
        free(x[i]);
    }

    free(x);
    free(y);

    for (int i = 0; i < NUM_TERMS; i++)
    {
        free(x_transpose[i]);
        free(x_transpose_x[i]);
    }

    free(x_transpose);
    free(x_transpose_x);
    free(x_transpose_y);
    free(coefficients);

    return rmse;
}

double polynomial_regression_train_and_test_optimized(double data[SAMPLE_SIZE][DATASET_FEATURES], int feature_size, int sample_size, int degree)
{
    // Calculate the number of terms for the polynomial regression
    int NUM_TERMS = 1;
    for (int i = 0; i < feature_size - 1; i++)
	{
		NUM_TERMS *= (degree + 1);
	}
        
    // Allocate memory for the feature matrix and label array
    double **x = malloc(sample_size * sizeof(double *));
    double *y = malloc(sample_size * sizeof(double));

    // Populate feature matrix and label array
    for (int i = 0; i < sample_size; i++)
    {
        x[i] = malloc(NUM_TERMS * sizeof(double));
        int index = 0;

        for (int comb = 0; comb < NUM_TERMS; comb++)
        {
            int temp_comb = comb;
            double value = 1;

            // Calculate the product of features raised to their respective powers
            for (int feature = 0; feature < feature_size - 1; feature++)
            {
                int power = temp_comb % (degree + 1);
                double temp = 1;

                // Replace pow with multiplication loop
                for (int p = 0; p < power; p++)
                {
                    temp *= data[i][feature];
                }

                value *= temp;
                temp_comb /= (degree + 1);
            }

            x[i][index++] = value;
        }

        // Store label value
        y[i] = data[i][feature_size - 1];
    }

    // Allocate memory for x transpose matrix, x transpose times x matrix, and x transpose times y matrix
    double **x_transpose = malloc(NUM_TERMS * sizeof(double *));
    double **x_transpose_x = malloc(NUM_TERMS * sizeof(double *));
    double *x_transpose_y = malloc(NUM_TERMS * sizeof(double));

    // Compute x transpose matrix and x transpose times x matrix
    for (int i = 0; i < NUM_TERMS; i++)
    {
        x_transpose[i] = malloc(sample_size * sizeof(double));
        x_transpose_x[i] = malloc(NUM_TERMS * sizeof(double));

        // Compute x transpose matrix
        for (int j = 0; j < sample_size; j++)
        {
            x_transpose[i][j] = x[j][i];
        }

        // Compute x transpose times x matrix
        for (int j = 0; j < NUM_TERMS; j++)
        {
            double sum = 0;

            for (int k = 0; k < sample_size; k++)
            {
                sum += x_transpose[i][k] * x[k][j];
            }

            x_transpose_x[i][j] = sum;
        }

        // Compute x transpose times y matrix
        double sum = 0;

        for (int j = 0; j < sample_size; j++)
        {
            sum += x_transpose[i][j] * y[j];
        }

        x_transpose_y[i] = sum;
    }

    // Allocate memory for coefficients
    double *coefficients = malloc(NUM_TERMS * sizeof(double));

    // Gaussian elimination to solve the system of equations
    for (int i = 0; i < NUM_TERMS; i++)
    {
         for (int j = i + 1; j < NUM_TERMS; j++)
        {
            double ratio = x_transpose_x[j][i] / x_transpose_x[i][i];

            for (int k = i; k < NUM_TERMS; k++)
            {
                x_transpose_x[j][k] -= ratio * x_transpose_x[i][k];
            }

            x_transpose_y[j] -= ratio * x_transpose_y[i];
        }
    }

    // Back-substitution to find the coefficients
    for (int i = NUM_TERMS - 1; i >= 0; i--)
    {
        double sum = 0;

        for (int j = i + 1; j < NUM_TERMS; j++)
        {
            sum += x_transpose_x[i][j] * coefficients[j];
        }

        coefficients[i] = (x_transpose_y[i] - sum) / x_transpose_x[i][i];
    }

    // Calculate RMSE on training data
    double rmse = 0;

    for (int i = 0; i < sample_size; i++)
    {
        double y_pred = 0;

        for (int j = 0; j < NUM_TERMS; j++)
        {
            y_pred += coefficients[j] * x[i][j];
        }

        rmse += (y_pred - data[i][feature_size - 1]) * (y_pred - data[i][feature_size - 1]);
    }

    rmse = sqrt(rmse / sample_size);

    // Free memory allocated for arrays
    for (int i = 0; i < sample_size; i++)
    {
        free(x[i]);
    }

    free(x);
    free(y);

    for (int i = 0; i < NUM_TERMS; i++)
    {
        free(x_transpose[i]);
        free(x_transpose_x[i]);
    }

    free(x_transpose);
    free(x_transpose_x);
    free(x_transpose_y);
    free(coefficients);

    return rmse;
}