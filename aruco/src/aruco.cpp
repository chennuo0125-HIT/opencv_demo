#include "aruco.h"

Aruco::Aruco(string dict_name)
{
    cv::aruco::PREDEFINED_DICTIONARY_NAME name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_6X6_250;
    if (dict_name != "4X4_50" && dict_name != "4X4_100" && dict_name != "4X4_100" && dict_name != "4X4_1000"
        && dict_name != "5X5_50" && dict_name != "5X5_100" && dict_name != "5X5_250" && dict_name != "5X5_1000"
        && dict_name != "6X6_50" && dict_name != "6X6_100" && dict_name != "6X6_250" && dict_name != "6X6_1000"
        && dict_name != "7X7_50" && dict_name != "7X7_100" && dict_name != "7X7_250" && dict_name != "7X7_1000")
    if ("4X4_50" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50;
    else if ("4X4_100" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_100;
    else if ("4X4_100" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_250;
    else if ("4X4_1000" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_1000;
    else if ("5X5_50" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_5X5_50;
    else if ("5X5_100" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_5X5_100;
    else if ("5X5_250" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_5X5_250;
    else if ("5X5_1000" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_5X5_1000;
    else if ("6X6_50" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_6X6_50;
    else if ("6X6_100" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_6X6_100;
    else if ("6X6_250" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_6X6_250;
    else if ("6X6_1000" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_6X6_1000;
    else if ("7X7_50" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_7X7_50;
    else if ("7X7_100" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_7X7_100;
    else if ("7X7_250" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_7X7_250;
    else if ("7X7_1000" == dict_name) name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_7X7_1000;
    else cout << "input incorrect dictory name, use default 6X6_250 !" << endl;


    // initialize aruco marker dictionary
    dictionary_ptr_ = cv::aruco::getPredefinedDictionary(name);
}

Aruco::~Aruco(){}

void Aruco::generateMarker(cv::Mat& img, int id)
{
    int img_pixel_size = 500;
    if (!img.empty()) img_pixel_size = max(img.rows, img.cols);

    cv::aruco::drawMarker(dictionary_ptr_, id, img_pixel_size, img, 1);
}

void Aruco::detectMarker(const cv::Mat img, vector<vector<cv::Point2f>>& markers_corners, vector<int>& markers_ids)
{
    cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();
    vector<vector<cv::Point2f>> rejected_candidates;
    cv::aruco::detectMarkers(img, dictionary_ptr_, markers_corners, markers_ids, parameters, rejected_candidates);
}

void Aruco::configCamAndPrintedMarker(cv::Mat camera_mat, cv::Mat dist_coefs, double side_length)
{
    camera_mat_ = camera_mat;
    dist_coefs_ = dist_coefs;
    side_length_ = side_length;
}

void Aruco::estimateMarkerPose(const cv::Mat img, vector<cv::Vec3d>& rvecs, vector<cv::Vec3d>& tvecs, 
    vector<vector<cv::Point2f>>& markers_corners, vector<int>& markers_ids)
{
    if (camera_mat_.empty() || dist_coefs_.empty() || 0 == side_length_)
    {
        cout << "please config camera intrinsics and printed marker side length !!!" << endl;
        rvecs.clear();
        tvecs.clear();
        return;
    }

    detectMarker(img, markers_corners, markers_ids);
    cv::aruco::estimatePoseSingleMarkers(markers_corners, side_length_, camera_mat_, dist_coefs_, rvecs, tvecs);
}

void Aruco::estimateMarkerPose(const cv::Mat img, vector<cv::Vec3d>& rvecs, vector<cv::Vec3d>& tvecs)
{
    vector<vector<cv::Point2f>> markers_corners;
    vector<int> markers_ids;
    estimateMarkerPose(img, rvecs, tvecs, markers_corners, markers_ids);
}

void Aruco::drawMarkerAxis(cv::Mat& img)
{
    vector<vector<cv::Point2f>> markers_corners;
    vector<int> markers_ids;
    vector<cv::Vec3d> rvecs;
    vector<cv::Vec3d> tvecs;
    estimateMarkerPose(img, rvecs, tvecs, markers_corners, markers_ids);

    if (markers_corners.size() != markers_ids.size() || markers_corners.size() != rvecs.size() || markers_corners.size() != tvecs.size())
    {
        cout << "markers_corners size " << markers_corners.size() << ", markers_ids size " << markers_ids.size() 
            << ", rvecs size " << rvecs.size() << ", tvecs size " << tvecs.size() << " arent equal !!!" <<endl;
        return;
    }
    
    cv::aruco::drawDetectedMarkers(img, markers_corners, markers_ids);
    for (int i = 0; i < markers_ids.size(); i++)
        cv::aruco::drawAxis(img, camera_mat_, dist_coefs_, rvecs[i], tvecs[i], 0.1);
}