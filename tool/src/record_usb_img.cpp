#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main (int argc, char** argv)
{
    string img_dir;
    int cam_id = 0;
    if (argc < 2)
    {
        cout << "please input: ./node img_dir or ./node img_dir cam_id !!!" << endl;
        return 0;
    }
    if (argc == 3) cam_id = atoi(argv[2]);
    img_dir = argv[1];

    cv::namedWindow("usb_cam", cv::WINDOW_NORMAL);

    cv::VideoCapture vcap(cam_id);
    cv::Mat img;
    int img_num = 0;

    char key = ' ';
    while (key != 27)
    {
        vcap >> img;
        if (img.empty())
        {
            cout << "cant read image from usb cam !!!" << endl;
            return 0;
        }
        {
            string log = "press [c] to capture, press [esc] to exit, has capture " + to_string(img_num); 
            cv::Mat img_copy = img.clone();
            cv::putText(img_copy, log, cv::Point(10.0, img_copy.rows - 10), 1, 1, cv::Scalar(0, 0, 255));
            cv::imshow("usb_cam", img_copy);
        }

        key = ' ';
        key = cv::waitKey(10);
        if (key == 'c')
        {
            string img_file = img_dir + to_string(img_num) + ".jpg";
            cv::imwrite(img_file, img);
            img_num++;
        }
    }

    return 0;
}