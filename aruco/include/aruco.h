/* @brief: calibrate extrinsics between lidar and camera by aruco marker
 * author: chennuo0125@163.com
 * date: 20210105
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

using namespace std;

class Aruco
{
public:
    Aruco(string dict_name);
    ~Aruco();

    // generate calibrate target
    void generateMarker(cv::Mat& img, int id);

    // verify aruco marker detection
    void detectMarker(const cv::Mat img, vector<vector<cv::Point2f>>& markers_corners, vector<int>& markers_ids);

    // config camera param before estimate marker pose and draw marker
    void configCamAndPrintedMarker(cv::Mat camera_mat, cv::Mat dist_coefs, double side_length, bool use_fisheye = false);

    // estimate marker pose
    void estimateMarkerPose(const cv::Mat img, vector<cv::Vec3d>& rvecs, vector<cv::Vec3d>& tvecs);
    void estimateMarkerPose(const cv::Mat img, vector<cv::Vec3d>& rvecs, vector<cv::Vec3d>& tvecs, 
        vector<vector<cv::Point2f>>& markers_corners, vector<int>& markers_ids);

    // draw marker and axis on image (debug)
    void drawMarkerAxis(cv::Mat& img);

private:
    bool use_fisheye_{false}; // whether use fisheye model
    cv::Mat camera_mat_; // camera intrinsics
    cv::Mat dist_coefs_; // camera distort coefficients
    double side_length_{0.0}; // actual side length of printed marker
    cv::Ptr<cv::aruco::Dictionary> dictionary_ptr_; // aruco marker dictory
};