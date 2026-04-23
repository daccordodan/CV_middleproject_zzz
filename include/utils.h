#ifndef UTILS_CV_ZZZ_H
#define UTILS_CV_ZZZ_H
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

float evaluate_mIoU(std::vector<cv::Point>& predicted_points, std::vector<cv::Point>& ground_truth_points, cv::InputOutputArray& output_image=cv::noArray());

#endif // UTILS_DI_PROVA_HPP