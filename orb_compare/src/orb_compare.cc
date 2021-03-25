#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#include "ORBextractor.h"

using namespace std;

int main (int argc, char** argv)
{
    if (argc != 2)
    {
        cout << "please input : ./node img_dir !!!" << endl;
        return 0;
    }
    cv::Mat img = cv::imread(argv[1]).clone();
    if (img.empty())
    {
        cout << "img in " << argv[1] << " is empty !!!" << endl;
        return 0;
    }

    const int feat_num = 1000; // featrue number
    const double scale_factor = 1.2; // pyramid scale factor
    const int levels_num = 8; // pyramid levels number
    const int default_fast_thresh = 20;
    const int min_fast_thresh = 8; // this threshold used when cant get enough keypoints by default fast threshold

    // transform to gray image
    cv::Mat gray_img;
    cv::cvtColor(img, gray_img, CV_BGR2GRAY);

    // test orb extractor in orb_slam2
    vector<cv::KeyPoint> slam_keys;
    cv::Mat slam_descriptors;
    ORB_SLAM2::ORBextractor slam_extractor(feat_num, scale_factor, levels_num, default_fast_thresh, min_fast_thresh);
    slam_extractor(gray_img, cv::Mat(), slam_keys, slam_descriptors);

    // test orb extractor in opencv
    vector<cv::KeyPoint> opencv_keys;
    cv::Ptr<cv::ORB> opencv_extractor = cv::ORB::create();
    opencv_extractor->detect(gray_img, opencv_keys);

    // draw keypoints
    cv::Mat slam_img, opencv_img;
    cv::drawKeypoints(img, slam_keys, slam_img, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);
    cv::drawKeypoints(img, opencv_keys, opencv_img, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);

    // show
    cv::imshow("slam orb", slam_img);
    cv::imshow("opencv orb", opencv_img);
    cv::waitKey();

    return 0;
}