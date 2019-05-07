#include <WOpenFace.h>
#include <opencv2/core/mat.hpp>
#include <memory>

WCLNF::WCLNF(const std::string& model_location)
{
    face_model.reset(new LandmarkDetector::CLNF(model_location));
}
bool WCLNF::loaded_successfully()
{
    return face_model->loaded_successfully;
}

std::vector<float> WCLNF::detected_landmarks()
{
    std::vector<float> landmarks;
    landmarks.assign(face_model->detected_landmarks.begin(), face_model->detected_landmarks.end());
    return landmarks;
}

float WCLNF::detection_certainty()
{
    return face_model->detection_certainty;
}

bool DetectLandmarksInVideo(int height, int width, int nChannels, uchar* rgb_image, uchar* grayscale_image, WCLNF& clnf_model, LandmarkDetector::FaceModelParameters& params)
{
        //Mat (int rows, int cols, int type, void *data, size_t step=AUTO_STEP)
    //        //bool detection_success = LandmarkDetector::DetectLandmarksInVideo(rgb_frame, face_model, det_parameters, gray_frame);
    assert(nChannels == 3);
    cv::Mat rgb_frame(height, width, CV_8UC3, rgb_image);
    cv::Mat gray_frame(height, width, CV_8UC1, grayscale_image);
    return LandmarkDetector::DetectLandmarksInVideo(rgb_frame, *(clnf_model.face_model), params, gray_frame);
}

std::vector<float> GetPose(WCLNF* clnf_model, float fx, float fy, float cx, float cy)
{
    cv::Vec6d pose_estimate = LandmarkDetector::GetPose(*(clnf_model->face_model), fx, fy, cx, cy);
    return std::vector<float> {pose_estimate[0], pose_estimate[1], pose_estimate[2], 
                                pose_estimate[3], pose_estimate[4], pose_estimate[5]};

}

void print_numpy(int height, int width, int nChannels, uchar* rgb_img, uchar* gray_img)
{
    std::cout<<"width:"<<width<<". height:"<<height<<". channels:"<<nChannels<<std::endl;
}

// void CalcShape3D(int npts, int dim_cor, float* landmarks_3d)//WCLNF& clnf_model, 
//  {
// //     if(npts != 68 || dim_cor != 3)
// //     {
// //         PyErr_Format(PyExc_ValueError, "Arrays of lengths (%d,%d) given", npts, dim_cor);
// //         return;
// //     }
// //     cv::Mat_<float> tmp_3d_landmarks;
// //     clnf_model.face_model->pdm.CalcShape3D(tmp_3d_landmarks, clnf_model.face_model->params_local);
// //     //tmp_3d_landmarks = tmp_3d_landmarks.reshape(1, 3).t();
// //     std::copy(tmp_3d_landmarks.begin(), tmp_3d_landmarks.end(), landmarks_3d);
// }
