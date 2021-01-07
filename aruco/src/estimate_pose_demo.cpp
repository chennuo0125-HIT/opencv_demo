#include "aruco.h"
#include <opencv2/core.hpp>

#define VIEW_ONLINE false

using namespace std;

int main (int argc, char** argv)
{
    if (argc != 4)
    {
        cout << "please run: ./node dict_name param.yaml img !!!" << endl;
        return 0;
    }

    // use dictionary name to initialize aruco object
    Aruco aruco(argv[1]); 

    // read local config param
    bool use_fisheye;
    double side_length;
    cv::Mat cam_mat, dist_coef;
    string instrinsics_file = argv[2];
    cv::FileStorage fs(instrinsics_file, cv::FileStorage::READ);
    if (false == fs.isOpened())
    {
        cout << "cant open file " << instrinsics_file << "!!!"<<endl;
        return 0;
    }
    fs["camera_matrix"] >> cam_mat;
    fs["dist_coefs"] >> dist_coef;
    fs["printed_marker_side_length"] >> side_length;
    fs["use_fisheye"] >> use_fisheye;
    
    // config aruco for estimate pose
    aruco.configCamAndPrintedMarker(cam_mat, dist_coef, side_length, use_fisheye);
#if VIEW_ONLINE
    cv::VideoCapture vcap(0);
    cv::Mat image;
    cv::namedWindow("result", CV_WINDOW_NORMAL);
    while (cv::waitKey(100) != 27)
    {
        vcap >> image;
        if (image.empty()) break;

        // estimate pose
        vector<cv::Vec3d> rvecs, tvecs;
        aruco.estimateMarkerPose(image, rvecs, tvecs);
        assert(rvecs.size() == tvecs.size());
        // for (int i = 0; i < rvecs.size(); i++)
        // {
        //     cout << "marker " << i << " : rvec->[" << rvecs[i][0] << "," << rvecs[i][1] << "," << rvecs[i][2] 
        //         << "], tvec->[" << tvecs[i][0] << "," << tvecs[i][1] << "," << tvecs[i][2] << "]" << endl;
        // }
        // cout << endl;

        // view result image
        cv::Mat copy_img = image.clone();
        aruco.drawMarkerAxis(copy_img);

        cv::imshow("result", copy_img);
    }
#else
    // read detecting image
    cv::Mat image = cv::imread(argv[3]).clone();
    if (image.empty())
    {
        cout << "image is empty !!!" << endl;
        return 0;
    }

    // estimate pose
    vector<cv::Vec3d> rvecs, tvecs;
    aruco.estimateMarkerPose(image, rvecs, tvecs);
    assert(rvecs.size() == tvecs.size());
    for (int i = 0; i < rvecs.size(); i++)
    {
        cout << "marker " << i << " : rvec->[" << rvecs[i][0] << "," << rvecs[i][1] << "," << rvecs[i][2] 
            << "], tvec->[" << tvecs[i][0] << "," << tvecs[i][1] << "," << tvecs[i][2] << "]" << endl;
    }

    // view result image
    cv::Mat copy_img = image.clone();
    aruco.drawMarkerAxis(copy_img);
    cv::imwrite("result.jpg", copy_img);
#endif

    return 0;
}