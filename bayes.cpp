#include <Fastor/Fastor.h>
using namespace Fastor;

/* 
 Discriminant calculation for a covariance matrix belonging to case 1.
 Covariance matrix is diagonal where the variance is multiplied with the identity matrix.
 Equal variance among each class
 
 @param x: random samples <x>
 @param y: random samples <y>
 @param mean of normal distribution <x,y>
 @param variance
 @param probabiliity of sample based on prior evidence 
 @return a 'score' to every point in the input space for every class
 */
float covariance_case_1(float x, float y, Tensor<float, 2, 1> mean, float variance, float probability)
{
    Tensor<float, 2, 1> samples = {{x}, {y}};

    Tensor<float, 1, 1> witx = matmul(transpose((1 / variance) * mean), samples);
    Tensor<float, 1, 1> mean_mult = matmul(transpose(mean), mean);
    double wi0 = (-1 / (2 * variance)) * mean_mult(0, 0) + log(probability);

    return (witx(0, 0) + wi0);
}

/* 
 Special case of Chernoff bound where beta = 0.5
 A computationally simpler, less tight bound can be derived by using the Bhattacharyya bound on the error

 @param beta
 @param cova_1: covariance matrix for the first normal distribution
 @param cova_2: covariance matrix for the second normal distribution
 @param mean_1: mean for first normal distribution
 @param mean_2: mean for second normal distribtuion
 @return a value used to estimate the error bound for gaussian distributions
 */

float bhattacharyya_bound(float beta, Tensor<float, 2, 2> cova_1, Tensor<float, 2, 2> cova_2, Tensor<float, 2, 1> mean_1, Tensor<float, 2, 1> mean_2)
{
    float constant = (beta * (1 - beta)) / 2;

    Tensor<float, 2, 2> beta_cova = ((1 - beta) * cova_1) + (beta * cova_2);
    Tensor<float, 1, 2> first_part = matmul((constant * transpose(mean_1 - mean_2)), inverse(beta_cova));
    Tensor<float, 1, 1> before_onehalf = matmul(first_part, (mean_1 - mean_2));

    float in_ln = (determinant(beta_cova) / ((pow(determinant(cova_1), 1 - beta) * pow(determinant(cova_2), beta))));

    float kb = before_onehalf(0, 0) + ((1 / 2) * log(in_ln));

    return kb;
}

/* 
  Using bhattacharyya bound on the error, determine the frequency at which the test will lead to an error
  
  @param kb: Special case of Chernoff bound where beta = 0.5. bhattacharyya_bound calculation kb 
  @param probability: In the case of two categories, both classes have equal probability
  @return error probability*/
float probability_of_error(float kb, float probability)
{
    return sqrt(probability * probability) * exp(-kb);
}