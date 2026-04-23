#ifndef FEATURE_CV_ZZZ_H
#define FEATURE_CV_ZZZ_H
#include <opencv2/highgui.hpp>

class Feature
{
    public:
        Feature();
        Feature(cv::Point point, bool presence = true, bool movement = true);

        cv::Point getPoint() const;

        void setFeaturePresence(int index, bool presence);
        void setFeaturePosition(int index, bool movement);
        bool isFeaturePresent(int index) const;
        bool isFeatureMoving(int index) const;
    private:
        cv::Point point;
        bool presence;
        bool movement;
};

#endif