#include <iostream>
#include <fstream>
#include <vector>

extern float box_muller(float, float);
extern float covariance_case_1(std::vector<int>, std::vector<int>, int, float);

int main()
{
    std::vector<int> mean_1 = {1, 1};
    std::vector<int> mean_2 = {4, 4};

    // Data set A has equal covariance matrices
    const std::vector<std::vector<int>> covariance_matrix_A{{1, 0}, {0, 1}};

    std::ofstream out;
    std::ofstream out2;

    out.open("DataSetA_1.csv");
    out2.open("DataSetA_2.csv");

    double x1, y1, x2, y2;

    // Generating 60,000 random samples for the first normal distribution
    for (int i = 0; i < 60000; i++)
    {
        // x gaussian samples with x's mean and x's variance
        x1 = box_muller(mean_1[0], covariance_matrix_A[0][0]);

        // y gaussian samples with y's mean and y's variance
        y1 = box_muller(mean_1[1], covariance_matrix_A[1][1]);

        // Outputing random samples to a file for a visualization plot
        out << x1 << "," << y1 << '\n';
    }

    // Generating 140,000 random samples for the second normal distribution
    for (int i = 0; i < 140000; i++)
    {
        x2 = box_muller(mean_2[0], covariance_matrix_A[0][0]);
        y2 = box_muller(mean_2[1], covariance_matrix_A[1][1]);
        out2 << x2 << "," << y2 << '\n';
    }

    out.close();
    out2.close();

    return 0;
}