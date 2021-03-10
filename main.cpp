#include <iostream>
#include <fstream>

#include <Fastor/Fastor.h>
using namespace Fastor;

extern float box_muller(float, float);
extern float covariance_case_1(float, float, Tensor<float, 2, 1>, float, float);
extern float covariance_case_3(float, float, Tensor<float, 2, 1>, Tensor<float, 2, 2>, float);
extern float euclidean_distance_classifier(float, float, Tensor<float, 2, 1>);
extern float bhattacharyya_bound(float, Tensor<float, 2, 2>, Tensor<float, 2, 2>, Tensor<float, 2, 1>, Tensor<float, 2, 1>);
extern float probability_of_error(float, float);

int main()
{
    // Data set A and B have the same mean for both categories
    Tensor<float, 2, 1> mean_1 = {{1.0}, {1.0}};
    Tensor<float, 2, 1> mean_2 = {{4.0}, {4.0}};

    // Data set A has equal covariance matrices
    Tensor<float, 2, 2> setA_covariance_matrix = {{1.0, 0.0}, {0.0, 1.0}};

    // Data set A has the same variance for each value of x and y for each normal distribution
    float setA_variance = 1.0;
    /* Standard deviation for box muller transformation
     Same for each x and y value of both normal distributions in data set A */
    float setA_standard_deviation = sqrt(setA_covariance_matrix(0, 0));

    std::ofstream DataSetA_out1;
    std::ofstream DataSetA_out2;
    std::ofstream DataSetA_out3;
    std::ofstream DataSetA_out4;
    std::ofstream DataSetA_out5;
    std::ofstream DataSetA_out6;

    // Output stream for generated data
    DataSetA_out1.open("./data/DataSetA_normal1_random.csv");
    DataSetA_out2.open("./data/DataSetA_normal2_random.csv");
    DataSetA_out3.open("./data/DataSetA_linear_discriminant_norm1.csv");
    DataSetA_out4.open("./data/DataSetA_linear_discriminant_norm2.csv");
    DataSetA_out5.open("./data/DataSetA_euclidean_norm1.csv");
    DataSetA_out6.open("./data/DataSetA_euclidean_norm2.csv");

    /* Prior probability. No evidence for a randomly generated value belonging to a particular class
     Treating a random sample with equal proability for both classes within both normal distributions */
    float probability_case_1 = 0.50,
          probability_case_2 = 0.50;

    float x1, y1, x2, y2;
    int setA_norm1_classify_class1 = 0, setA_norm1_classify_class2 = 0;
    int setA_norm1_euclidean_class1 = 0, setA_norm1_euclidean_class2 = 0;
    float setA_discriminant1 = 0.0, setA_discriminant2 = 0.0;
    float setA_euclidean1 = 0.0, setA_euclidean2 = 0.0;
    float dichotomizer = 0;

    // Generating 60,000 random samples for the first normal distribution
    for (int i = 0; i < 60000; i++)
    {
        // x gaussian samples with x's mean and x's standard deviation
        x1 = box_muller(mean_1(0, 0), setA_standard_deviation);

        // y gaussian samples with y's mean and y's standard deviation
        y1 = box_muller(mean_1(0, 0), setA_standard_deviation);

        // Outputing random samples to a file for a visualization plot
        DataSetA_out1 << x1 << "," << y1 << '\n';

        setA_discriminant1 = covariance_case_1(x1, y1, mean_1, setA_variance, probability_case_1);
        setA_discriminant2 = covariance_case_1(x1, y1, mean_2, setA_variance, probability_case_1);
        // Outputing linear discriminants to file
        DataSetA_out3 << setA_discriminant1 << "," << setA_discriminant2 << '\n';

        // Case of two categories, single discriminant function (dichotomizer) instead of two
        dichotomizer = setA_discriminant1 - setA_discriminant2;
        if (dichotomizer > 0)
        {
            // Counting number of correctly classified random samples that belong in class one
            setA_norm1_classify_class1++;
        }
        else
        {
            // Counting number of incorrectly classified random samples that actually belong to class 1, but were classified to class two
            setA_norm1_classify_class2++;
        }

        // Euclidean Distance classifier
        setA_euclidean1 = euclidean_distance_classifier(x1, y1, mean_1);
        setA_euclidean2 = euclidean_distance_classifier(x1, y1, mean_2);
        // Outputing euclidean discriminants to file
        DataSetA_out5 << setA_euclidean1 << "," << setA_euclidean2 << '\n';

        if (setA_euclidean1 > setA_euclidean2)
        {
            setA_norm1_euclidean_class1++;
        }
        else
        {
            setA_norm1_euclidean_class2++;
        }
    }

    // Calculation of misclassification rate for data set A normal distribution 1 using a linear discriminant classifier
    float percent_missclassified_linear_norm1 = (setA_norm1_classify_class2 / 60000.0f) * 100.0f;
    std::cout << "\nData set A\n"
              << "=====" << '\n';
    std::cout << "Total misclassified samples for class 1 with linear discriminant: " << setA_norm1_classify_class2 << "(" << percent_missclassified_linear_norm1 << "%)." << std::endl;

    // Calculation of misclassification rate for data set A normal distribution 1 using an euclidean classifier
    float percent_missclassified_euclidean_norm1 = (setA_norm1_euclidean_class2 / 60000.0f) * 100.0f;
    std::cout << "Total misclassified samples for class 1 with euclidean: " << setA_norm1_euclidean_class2 << "(" << percent_missclassified_euclidean_norm1 << "%)" << std::endl;

    // Generating 140,000 random samples for the second normal distribution
    int setA_norm2_classify_class1 = 0, setA_norm2_classify_class2 = 0;
    int setA_norm2_euclidean_class1 = 0, setA_norm2_euclidean_class2 = 0;
    for (int i = 0; i < 140000; i++)
    {
        x2 = box_muller(mean_2(0, 0), setA_standard_deviation);
        y2 = box_muller(mean_2(0, 0), setA_standard_deviation);

        DataSetA_out2 << x2 << "," << y2 << '\n';

        // Linear Discriminant
        setA_discriminant1 = covariance_case_1(x2, y2, mean_1, setA_variance, probability_case_1);
        setA_discriminant2 = covariance_case_1(x2, y2, mean_2, setA_variance, probability_case_2);
        DataSetA_out4 << setA_discriminant1 << "," << setA_discriminant2 << '\n';

        dichotomizer = setA_discriminant2 - setA_discriminant1;
        if (dichotomizer > 0)
        {
            setA_norm2_classify_class2++;
        }
        else
        {
            setA_norm2_classify_class1++;
        }

        // Euclidean distance classifier
        setA_euclidean1 = euclidean_distance_classifier(x2, y2, mean_1);
        setA_euclidean2 = euclidean_distance_classifier(x2, y2, mean_2);
        DataSetA_out6 << setA_euclidean1 << "," << setA_euclidean2 << '\n';

        if (setA_euclidean2 > setA_euclidean1)
        {
            setA_norm2_euclidean_class2++;
        }
        else
        {
            setA_norm2_euclidean_class1++;
        }
    }

    //Calculation of misclassification rate for data set A normal distribution 2 using a linear discriminant classifier
    float percent_missclassified_norm2 = (setA_norm2_classify_class1 / 140000.0) * 100.0f;
    std::cout << "\nTotal misclassified samples for class 2 with linear discriminant: " << setA_norm2_classify_class1 << "(" << percent_missclassified_norm2 << "%)." << std::endl;

    //Calculationg of misclassification rate for data set A normal distribution 2 using a euclidean classifier
    float percent_missclassified_euclidean_norm2 = (setA_norm2_euclidean_class1 / 140000.0f) * 100.0f;
    std::cout << "Total misclassified samples for class 2 with euclidean: " << setA_norm2_euclidean_class1 << "(" << percent_missclassified_euclidean_norm2 << "%)" << std::endl;

    // Calculation of total misclassification rate for data set A
    std::cout << '\n';
    float total_misclassified = setA_norm2_classify_class1 + setA_norm1_classify_class2;
    float percent_total_misclassified = (total_misclassified / 200000.0) * 100.f;
    std::cout << "Total misclassification rate = " << percent_total_misclassified << "%" << std::endl;
    DataSetA_out1.close();
    DataSetA_out2.close();
    DataSetA_out3.close();
    DataSetA_out4.close();
    DataSetA_out5.close();
    DataSetA_out6.close();

    // Calculation of the theoretical probability error (e.g., Bhattacharyya bound) for data set A
    float beta = 0.50;
    float kb = bhattacharyya_bound(beta, setA_covariance_matrix, setA_covariance_matrix, mean_1, mean_2);
    float setA_error = probability_of_error(kb, probability_case_1);
    float prob_error_dataSetA = setA_error * 100.0f;
    std::cout << "The probability of error <= " << setA_error << "(" << prob_error_dataSetA << "%)" << std::endl;

    std::cout << '\n';

    /* Data set B configurations */
    std::ofstream DataSetB_out1;
    std::ofstream DataSetB_out2;
    std::ofstream DataSetB_out3;
    std::ofstream DataSetB_out4;
    std::ofstream DataSetB_out5;
    std::ofstream DataSetB_out6;

    DataSetB_out1.open("./data/DataSetB_normal1_random.csv");
    DataSetB_out2.open("./data/DataSetB_normal2_random.csv");
    DataSetB_out3.open("./data/DataSetB_linear_discriminant_norm1.csv");
    DataSetB_out4.open("./data/DataSetB_linear_discriminant_norm2.csv");
    DataSetB_out5.open("./data/DataSetB_euclidean_norm1.csv");
    DataSetB_out6.open("./data/DataSetB_euclidean_norm2.csv");

    Tensor<float, 2, 2> setB_covariance_matrix1 = {{1, 0},
                                                   {0, 1}};
    Tensor<float, 2, 2> setB_covariance_matrix2 = {{4, 0}, {0, 8}};

    float standard_deviation_setB_1 = sqrt(setB_covariance_matrix2(0, 0));
    float standard_deviation_setB_2 = sqrt(setB_covariance_matrix2(1, 1));

    float x3, y3, x4, y4;
    float setB_discriminant1 = 0.0, setB_discriminant2 = 0.0;
    float setB_euclidean1 = 0.0, setB_euclidean2 = 0.0;

    int setB_norm1_classify_class1 = 0, setB_norm1_classify_class2 = 0;
    int setB_norm1_euclidean_class1 = 0, setB_norm1_euclidean_class2 = 0;

    // Generating 40,000 random samples from the first normal distribution in data set B
    for (int i = 0; i < 40000; i++)
    {
        x3 = box_muller(mean_1(0, 0), setA_standard_deviation);
        y3 = box_muller(mean_1(1, 0), setA_standard_deviation);

        DataSetB_out1 << x3 << "," << y3 << '\n';

        // Linear discriminant
        setB_discriminant1 = covariance_case_3(x3, y3, mean_1, setB_covariance_matrix1, probability_case_1);
        setB_discriminant2 = covariance_case_3(x3, y3, mean_2, setB_covariance_matrix2, probability_case_2);
        DataSetB_out3 << setB_discriminant1 << "," << setB_discriminant2 << '\n';

        dichotomizer = setB_discriminant1 - setB_discriminant2;
        if (dichotomizer > 0)
        {
            setB_norm1_classify_class1++;
        }
        else
        {
            setB_norm1_classify_class2++;
        }

        // Euclidean distance classifier
        setB_euclidean1 = euclidean_distance_classifier(x3, y3, mean_1);
        setB_euclidean2 = euclidean_distance_classifier(x3, y3, mean_2);
        DataSetB_out5 << setB_euclidean1 << "," << setB_euclidean2 << '\n';

        if (setB_euclidean1 > setB_euclidean2)
        {
            setB_norm1_euclidean_class1++;
        }
        else
        {
            setB_norm1_euclidean_class2++;
        }
    }

    float setB_percent_missclassified_class1 = (setB_norm1_classify_class2 / 40000.0f) * 100.0f;
    std::cout << "\nData set B\n"
              << "=====" << '\n';
    std::cout << "Total misclassified samples for class 1 with quadratic discriminant: " << setB_norm1_classify_class2 << "(" << setB_percent_missclassified_class1 << "%)." << std::endl;
    float setB_percent_missclassified_euclidean_norm1 = (setB_norm1_euclidean_class2 / 60000.0f) * 100.0f;
    std::cout << "Total misclassified samples for class 1 with euclidean: " << setB_norm1_euclidean_class2 << "(" << setB_percent_missclassified_euclidean_norm1 << "%) \n"
              << std::endl;

    int setB_norm2_classification_class_1 = 0;
    int setB_norm2_classification_class_2 = 0;
    int setB_norm2_euclidean_class1 = 0, setB_norm2_euclidean_class2 = 0;
    // Generating 160,000 random samples from the first second distribution in data set B
    int setB_norm2_classify_class1 = 0, setB_norm2_classify_class2 = 0;
    for (int i = 0; i < 160000; i++)
    {
        // Set B normal distribution one uses the same standard deviation from data set A
        x4 = box_muller(mean_2(0, 0), standard_deviation_setB_1);
        y4 = box_muller(mean_2(1, 0), standard_deviation_setB_2);
        DataSetB_out2 << x4 << "," << y4 << '\n';

        // Linear Discriminant
        setB_discriminant1 = covariance_case_3(x4, y4, mean_1, setB_covariance_matrix1, probability_case_2);
        setB_discriminant2 = covariance_case_3(x4, y4, mean_2, setB_covariance_matrix2, probability_case_2);
        DataSetB_out4 << setB_discriminant1 << "," << setB_discriminant2 << '\n';

        dichotomizer = setB_discriminant2 - setB_discriminant1;
        if (dichotomizer > 0)
        {
            setB_norm2_classify_class2++;
        }
        else
        {
            setB_norm2_classify_class1++;
        }

        // Euclidean distance classifier
        setB_euclidean1 = euclidean_distance_classifier(x4, y4, mean_1);
        setB_euclidean2 = euclidean_distance_classifier(x4, y4, mean_2);
        DataSetB_out6 << setB_euclidean1 << "," << setB_euclidean2 << '\n';

        if (setB_euclidean2 > setB_euclidean1)
        {
            setB_norm2_euclidean_class2++;
        }
        else
        {
            setB_norm2_euclidean_class1++;
        }
    }
    float setB_percent_missclassified_class2 = (setB_norm2_classify_class1 / 160000.0) * 100.0f;
    std::cout << "Total misclassified samples for class 2 with quadratic discriminant: " << setB_norm2_classify_class1 << "(" << setB_percent_missclassified_class2 << "%)." << std::endl;
    percent_missclassified_euclidean_norm2 = (setB_norm2_euclidean_class1 / 140000.0f) * 100.0f;
    std::cout << "Total misclassified samples for class 2 with euclidean: " << setB_norm2_euclidean_class1 << "(" << percent_missclassified_euclidean_norm2 << "%)" << std::endl;
    std::cout << '\n';

    // Set B total misclassification rate with linear discriminant
    float setB_total_misclassified = setB_norm1_classify_class2 + setB_norm2_classify_class1;
    float setB_percent_total_misclassified = (setB_total_misclassified / 200000.0) * 100.0f;
    std::cout << "Total misclassification rate with quadratic discriminant = " << setB_percent_total_misclassified << "%" << std::endl;

    // Set B total misclassification rate with euclidean discrminant
    float setB_euclidean_misclassifed = setB_norm1_classify_class1 + setB_norm2_classify_class1;
    float setB_euclidean_total_misclassified = (setB_euclidean_misclassifed / 200000.0) * 100.0f;
    std::cout << "Total misclassification rate with euclidean distance classifier = " << setB_euclidean_misclassifed << "(" << setB_euclidean_total_misclassified << "%)" << std::endl;

    /* Calculation for the theoretical probability error (e.g., Bhattacharyya bound) for data set B */
    float setB_kb = bhattacharyya_bound(beta, setB_covariance_matrix1, setB_covariance_matrix2, mean_1, mean_2);
    float setB_error = probability_of_error(setB_kb, probability_case_1);

    std::cout << "The probability of error <= " << setB_error << std::endl;

    std::cout << '\n';

    DataSetB_out1.close();
    DataSetB_out2.close();
    DataSetB_out3.close();
    DataSetB_out4.close();
    DataSetB_out5.close();
    DataSetB_out6.close();

    return 0;
}