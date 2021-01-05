#include <iostream>
#include "aruco.h"

using namespace std;

int main (int argc, char** argv)
{
    if (argc < 3)
    {
        cout << "please run: ./node dict_name id0 id1 id2 ...... !!!" << endl;
        return 0;
    }

    Aruco aruco_obj(argv[1]); // use dictionary name to initialize aruco object

    cv::Mat img(250, 250, CV_8UC1);
    for (int i = 2; i < argc; i++)
    {
        aruco_obj.generateMarker(img, atoi(argv[i]));
        
        string id = argv[i];
        string img_name = "marker" + id + ".jpg";
        cv::imwrite(img_name, img);
    }

    return 0;
}