#include <vector>
#include <numeric>
#include <functional>
#include <cmath>
#include <algorithm>

/* 
 Discriminant calculation for a covariance matrix belonging to case 1.
 Covariance matrix is diagonal where the variance is multiplied with the identity matrix.
 Equal variance among each class
 
 @param x: random samples <x>
 @param y: random samples <y>
 @param mean of normal distribution
 @param variance
 @param probabiliity of sample based on prior evidence 
 @return a 'score' to every point in the input space for every class
 */
float covariance_case_1(float x, float y, std::vector<float> mean, float variance, float probability)
{
    std::vector<float> samples;
    samples.push_back(x);
    samples.push_back(y);
    float one_over_variance = 1 / variance;
    float constant{one_over_variance};
    std::transform(mean.begin(), mean.end(), mean.begin(), [&constant](auto &c) { return c * constant; });
    float witx = std::inner_product(mean.begin(), mean.end(), samples.begin(), 0.0);
    float w10 = std::inner_product(mean.begin(), mean.end(), mean.begin(), 0.0) * (-1 / (2 * variance)) + log(probability);

    float discriminant = witx + w10;

    return discriminant;
}