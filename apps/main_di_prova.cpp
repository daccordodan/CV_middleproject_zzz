#include "opencv2/highgui.hpp"

int main(int argc, char** argv) {
    cv::Mat img=cv::imread(argv[1]);
    cv::namedWindow("pre");
    cv::imshow("pre",img);

    return 0;
}
