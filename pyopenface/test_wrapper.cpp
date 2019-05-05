#include <WOpenFace.h>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
    cv::VideoCapture cap("/root/project/gaze-test-set/IR-closer-lyy-withglass.mp4");
	LandmarkDetector::FaceModelParameters det_parameters;
    WCLNF clnf(det_parameters.model_location);

    while(true)
    {
        cv::Mat bgr_img;
        cap.read(bgr_img);
        cv::Mat rgb_img, gray_img ;
        cv::cvtColor(bgr_img,rgb_img, cv::COLOR_BGR2RGB);
        cv::cvtColor(bgr_img, gray_img, cv::COLOR_BGR2GRAY);
        bool sucess = DetectLandmarksInVideo(rgb_img.rows, rgb_img.cols, 3, rgb_img.data, gray_img.data, clnf, det_parameters);
        
        std::vector<float> landmarks = clnf.detected_landmarks();
        std::cout<<"landmarks:";
        for(float val : landmarks)
            std::cout<<val<<" ";
        std::cout<<std::endl;
        exit(0);
    };
    return 0;
}