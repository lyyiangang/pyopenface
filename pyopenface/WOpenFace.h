#ifndef WOPENFACE_H
#define WOPENFACE_H

#include <vector>
#include <memory>
#include <LandmarkDetectorFunc.h>
#include <LandmarkDetectorModel.h>

class WCLNF
{
    public:
        WCLNF(const std::string& model_location);
        bool loaded_successfully();
        std::vector<float> detected_landmarks();
        float detection_certainty();
        std::shared_ptr<LandmarkDetector::CLNF> face_model;
};

bool DetectLandmarksInVideo(int height, int width, int nChannels, uchar* rgb_image, uchar* grayscale_image, WCLNF& clnf_model, LandmarkDetector::FaceModelParameters& params);

//see SequenceCapture::SetCameraIntrinsics for the defination of fx, fy, cx and cy, return [Tx, Ty, Tz, Eul_x, Eul_y, Eul_z]
std::vector<float> GetPose(WCLNF* clnf_model, float fx, float fy, float cx, float cy);

// void CalcShape3D(WCLNF* clnf_model, float* pts_3d, int n_pts);

void print_numpy(int height, int width, int nChannels, uchar* rgb_img, uchar* gray_img);

//void CalcShape3D( int npts, int dim_cor, float* landmarks_3d);//WCLNF& clnf_model,
#endif
