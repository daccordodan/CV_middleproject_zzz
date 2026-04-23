#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <utils.h>


float evaluate_mIoU(std::vector<cv::Point>& predicted_points, std::vector<cv::Point>& ground_truth_points, cv::InputOutputArray& output_image=cv::noArray()){
    return 0.0f;
}

void detectSIFTPoints(const cv::Mat& gray, cv::Ptr<cv::SIFT>& sift, std::vector<cv::Point2f>& points)
{
    // Estrae keypoint SIFT e li converte in punti 2D tracciabili con Optical Flow.
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    sift->detectAndCompute(gray, cv::noArray(), keypoints, descriptors);

    for (size_t i = 0; i < keypoints.size(); ++i)
    {
        points.push_back(keypoints[i].pt);
    }
}

bool computeBoundingBoxFromPoints(const std::vector<cv::Point2f>& points, const cv::Size& imageSize, cv::Rect& box)
{
    // Calcola bbox axis-aligned e la limita ai bordi immagine.
    if (points.empty())
    {
        return false;
    }

    float minX = points[0].x;
    float maxX = points[0].x;
    float minY = points[0].y;
    float maxY = points[0].y;

    for (size_t i = 1; i < points.size(); ++i)
    {
        minX = std::min(minX, points[i].x);
        maxX = std::max(maxX, points[i].x);
        minY = std::min(minY, points[i].y);
        maxY = std::max(maxY, points[i].y);
    }

    int x1 = std::max(0, cvRound(minX));
    int y1 = std::max(0, cvRound(minY));
    int x2 = std::min(imageSize.width - 1, cvRound(maxX));
    int y2 = std::min(imageSize.height - 1, cvRound(maxY));

    if (x2 <= x1 || y2 <= y1)
    {
        return false;
    }

    box = cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2));
    return true;
}

int featureFilter(std::vector<cv::Point2f>& newPoints, std::vector<cv::Point2f>& activePoints, std::vector<uchar>& active, std::vector<cv::Point2f>& savedPoints, bool verbose=false){
    const float minMovement = 0.20f;
    const float maxMovement = 100.0f;
    int survived=0;
    
    for (size_t i = 0; i < newPoints.size(); ++i)
    {
        if (!active[i])
        {
            continue;
        }

        float dx = newPoints[i].x - activePoints[i].x;
        float dy = newPoints[i].y - activePoints[i].y;
        float motion = std::sqrt(dx * dx + dy * dy);

        if (motion > minMovement && motion < maxMovement)
        {
            // La box del frame 0 deve usare punti del frame 0, non del frame 1.
            savedPoints.push_back(activePoints[i]);
            ++survived;

            if (verbose)
            {
                std::cout << "[DEBUG][Frame 0->1] feature " << i
                          << " prev=(" << activePoints[i].x << "," << activePoints[i].y << ")"
                          << " next=(" << newPoints[i].x << "," << newPoints[i].y << ")"
                          << " motion=" << motion << std::endl;
            }
        }
    }

    return survived;
}

bool saveFrame(const std::string& folder, const cv::Mat& frame, const cv::Rect& box, const std::vector<cv::Point2f>& savedPoints, int frameCounter, bool showSavedFeatures=false){
    // Ogni categoria salva output in una cartella dedicata accanto all'input.
    const std::string outputFolder = folder + "_annotate";

    if (!cv::utils::fs::exists(outputFolder)) {
        cv::utils::fs::createDirectories(outputFolder);
    }

    cv::Mat output = frame.clone();
    cv::rectangle(output, box, cv::Scalar(0, 255, 0), 2);
    if (showSavedFeatures)
    {
        // Visualizza sul frame corrente le feature mantenute dopo i filtri.
        for (size_t i = 0; i < savedPoints.size(); ++i)
        {
            cv::circle(output, savedPoints[i], 2, cv::Scalar(0, 0, 255), -1);
        }
    }
    std::string outputPath = outputFolder + cv::format("/frame_%04d.png", frameCounter);
    if (!cv::imwrite(outputPath, output)) {
        std::cerr << "Errore: impossibile salvare " << outputPath << std::endl;
        return false;
    }
    return true;
}

bool keepDebugOutput(const std::string& path){
    // Flag debug per categoria: abilita/disabilita i log dettagliati del movimento feature.
    const bool logBird = false;
    const bool logCar = false;
    const bool logFrog = true;
    const bool logSheep = false;
    const bool logSquirrel = false;

    bool logFeatureMotion = false;
    if (path.find("bird") != std::string::npos)
    {
        logFeatureMotion = logBird;
    }
    else if (path.find("car") != std::string::npos)
    {
        logFeatureMotion = logCar;
    }
    else if (path.find("frog") != std::string::npos)
    {
        logFeatureMotion = logFrog;
    }
    else if (path.find("sheep") != std::string::npos)
    {
        logFeatureMotion = logSheep;
    }
    else if (path.find("squirrel") != std::string::npos)
    {
        logFeatureMotion = logSquirrel;
    }

    return logFeatureMotion;
}

// TODO: are those used??

/*
static void drawBoundingBoxFromPoints(cv::Mat& image, const std::vector<cv::Point2f>& points)
{
    if (points.empty())
    {
        return;
    }

    float minX = points[0].x;
    float maxX = points[0].x;
    float minY = points[0].y;
    float maxY = points[0].y;

    for (size_t i = 1; i < points.size(); ++i)
    {
        minX = std::min(minX, points[i].x);
        maxX = std::max(maxX, points[i].x);
        minY = std::min(minY, points[i].y);
        maxY = std::max(maxY, points[i].y);
    }

    cv::Rect box(
        cv::Point(static_cast<int>(minX), static_cast<int>(minY)),
        cv::Point(static_cast<int>(maxX), static_cast<int>(maxY))
    );

    cv::rectangle(image, box, cv::Scalar(0, 255, 0), 2);
}
*/