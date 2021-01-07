#include "aruco.h"

Aruco::Aruco(string dict_name)
{
    cv::aruco::PREDEFINED_DICTIONARY_NAME name = cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_6X6_250;
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

void Aruco::configCamAndPrintedMarker(cv::Mat camera_mat, cv::Mat dist_coefs, double side_length, bool use_fisheye)
{
    camera_mat_ = camera_mat;
    dist_coefs_ = dist_coefs;
    side_length_ = side_length;
    use_fisheye_ = use_fisheye;
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

    if (use_fisheye_)
    {
        // enlarge image size 4 times for more undistorted points locate in border of image
        const int scale = 4; 
        cv::Mat rimg(scale * img.rows, scale * img.cols, img.type());
        cv::Size new_img_size(rimg.cols, rimg.rows);
        cv::Mat new_mat = camera_mat_.clone();
        new_mat.at<double>(0, 2) = double(new_img_size.width) / 2;
        new_mat.at<double>(1, 2) = double(new_img_size.height) / 2;

        // cv::Mat map1, map2;
        // cv::fisheye::initUndistortRectifyMap(camera_mat_, dist_coefs_, cv::Matx33d::eye(), new_mat, new_img_size, CV_16SC2, map1, map2);
        // cv::remap(img, rimg, map1, map2, CV_INTER_LINEAR);

        // undistort markers' corners
        vector<vector<cv::Point2f>> un_corners;
        for (int i = 0; i < markers_corners.size(); i++)
        {
            vector<cv::Point2f> un_corner;
            cv::fisheye::undistortPoints(markers_corners[i], un_corner, camera_mat_, dist_coefs_, cv::Matx33d::eye(), new_mat);
            
            bool out_border = false;
            for (const auto& p : un_corner)
            {
                if (p.x < 0 || p.y < 0)
                {
                    out_border = true;
                    break;
                }
            }
            if (!out_border) un_corners.push_back(move(un_corner));
        }

        cv::aruco::estimatePoseSingleMarkers(un_corners, side_length_, new_mat, cv::Mat::zeros(5, 1, CV_64F), rvecs, tvecs);
    }
    else
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