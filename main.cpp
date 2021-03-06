#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

extern float box_muller(float, float);
extern float covariance_case_1(float, float, std::vector<float>, float, float);

int main()
{
    std::vector<float> mean_1 = {1.0, 1.0};
    std::vector<float> mean_2 = {4.0, 4.0};

    // Data set A has equal covariance matrices
    std::vector<std::vector<float>> covariance_matrix_A{{1.0, 0.0}, {0.0, 1.0}};

    // Data set A has the same variance for each value of x and y for each normal distribution
    float variance = 1.0;

    // Standard deviation for box muller transformation
    // Same for each x and y value of both normal distributions
    float standard_deviation = sqrt(covariance_matrix_A[0][0]);

    std::ofstream out;
    std::ofstream out2;

    out.open("DataSetA_1.csv");
    out2.open("DataSetA_2.csv");
    // Prior probability. No evidence for a randomly generated value belonging to a particular class
    // Treating a random sample with equal proability for both classes
    float probability = 0.50;
    float x1, y1, x2, y2;
    int norm1_classification_class_1 = 0, norm1_classification_class_2 = 0;
    float discriminant_1 = 0.0, discriminant_2 = 0.0;
    float dichotomizer = 0;

    // Generating 60,000 random samples for the first normal distribution
    for (int i = 0; i < 60000; i++)
    {
        // x gaussian samples with x's mean and x's standard deviation
        x1 = box_muller(mean_1[0], standard_deviation);

        // y gaussian samples with y's mean and y's standard deviation
        y1 = box_muller(mean_1[1], standard_deviation);

        // Outputing random samples to a file for a visualization plot
        out << x1 << "," << y1 << '\n';

        discriminant_1 = covariance_case_1(x1, y1, mean_1, variance, probability);
        discriminant_2 = covariance_case_1(x1, y1, mean_2, variance, probability);

        // Case of two categories,
        // Single discriminant function (dichotomizer) instead of two
        dichotomizer = discriminant_1 - discriminant_2;

        if (dichotomizer > 0)
        {
            // Counting number of correctly classified random samples that belong in class one
            norm1_classification_class_1++;
        }
        else
        {
            // Counting number of incorrectly classified random samples that actually belong to class two, but were classified to class one
            norm1_classification_class_2++;
        }
    }
    float percent_missclassified_class1 = (norm1_classification_class_2 / 60000.0f) * 100.0f;
    std::cout << '\n';
    std::cout << "Total misclassified samples for class 1: " << norm1_classification_class_2 << "(" << percent_missclassified_class1 << "%)." << std::endl;

    int norm2_classification_class_1 = 0, norm2_classification_class_2 = 0;

    // Generating 140,000 random samples for the second normal distribution
    for (int i = 0; i < 140000; i++)
    {
        x2 = box_muller(mean_2[0], standard_deviation);
        y2 = box_muller(mean_2[1], standard_deviation);
        out2 << x2 << "," << y2 << '\n';

        discriminant_1 = covariance_case_1(x2, y2, mean_1, variance, probability);
        discriminant_2 = covariance_case_1(x2, y2, mean_2, variance, probability);

        dichotomizer = discriminant_2 - discriminant_1;

        if (dichotomizer > 0)
        {
            norm2_classification_class_2++;
        }
        else
        {
            norm2_classification_class_1++;
        }
    }
    float percent_missclassified_class2 = (norm2_classification_class_1 / 140000.0) * 100.0f;
    std::cout << "Total misclassified samples for class 2: " << norm2_classification_class_1 << "(" << percent_missclassified_class2 << "%)." << std::endl;

    std::cout << '\n';
    float total_misclassified = norm2_classification_class_1 + norm1_classification_class_2;
    float percent_total_misclassified = (total_misclassified / 200000.0) * 100.f;
    std::cout << "Total misclassification rate = " << percent_total_misclassified << "%" << std::endl;
    out.close();
    out2.close();

    return 0;
}