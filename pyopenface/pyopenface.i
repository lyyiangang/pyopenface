%module pyopenface
%include "std_vector.i"
%include "std_string.i"

%{
    #define SWIG_FILE_WITH_INIT
    #include "LandmarkDetectorParameters.h"
    #include "WOpenFace.h"
    #include <opencv2/core/mat.hpp>
%}

// Instantiate templates used by example
namespace std {
       %template(IntVector) vector<int>;
       %template(DoubleVector) vector<double>;
       %template(FloatVector) vector<float>;
       %template(StringVector) vector<string>;
}

// numpy array need init, https://mit-crpg.github.io/OpenMOC/devguide/swig.html
//https://docs.scipy.org/doc/numpy/reference/swig.interface-file.html
%include "numpy.i"
%init %{
  import_array();
%}

//typemap for ext_DetectLandmarksInVideo
%apply (int DIM1, int DIM2, int DIM3, unsigned char* IN_ARRAY3) {(int height, int width, int nChannels, uchar* rgb_img)};
%apply (int DIM1, int DIM2, unsigned char* IN_ARRAY2) {(int gheight, int gwidth, uchar* gray_img)};

%rename (DetectLandmarksInVideo) ext_DetectLandmarksInVideo ;
%inline %{
    bool ext_DetectLandmarksInVideo(int height, int width, int nChannels, uchar* rgb_img,
                        int gheight, int gwidth, uchar* gray_img,
                        WCLNF& clnf_model, LandmarkDetector::FaceModelParameters& params) 
    {
        if (height != gheight || width != gwidth) 
            PyErr_Format(PyExc_ValueError, "Arrays of shape is not equal.(%d, %d) vs (%d, %d)", height, width, gheight, gwidth);
        return DetectLandmarksInVideo(height, width, nChannels, rgb_img, gray_img, clnf_model, params);
    }
%}


//typemap for ext_print_numpy
%apply(int DIM1, int DIM2, int DIM3, unsigned char* IN_ARRAY3) {(int height, int width, int nChannels, uchar* rgb_img)};
%apply(int DIM1, int DIM2, unsigned char* IN_ARRAY2){(int gheight, int gwidth, uchar* gray_img)}


%include "../OpenFace/lib/local/LandmarkDetector/include/LandmarkDetectorParameters.h"
%include "WOpenFace.h"

%rename (print_numpy) ext_print_numpy ;
%inline %{
    void ext_print_numpy(int height, int width, int nChannels, uchar* rgb_img,
                        int gheight, int gwidth, uchar* gray_img) 
    {
     //       if (len1 != len2) {
       // PyErr_Format(PyExc_ValueError, "Arrays of lengths (%d,%d) given", len1, len2);
        //return 0.0;
    //}
        assert(gwidth == width && height == gheight);
        print_numpy(height, width, nChannels, rgb_img, gray_img);
    }
%}

%apply (float ARGOUT_ARRAY2[ANY][ANY]) {(float landmarks_3d[68][3])};
void CalcShape3D(WCLNF& clnf_model,float landmarks_3d[68][3]);
%{
 void CalcShape3D(WCLNF& clnf_model, float landmarks_3d[68][3])
  {
    cv::Mat_<float> tmp_3d_landmarks;
    clnf_model.face_model->pdm.CalcShape3D(tmp_3d_landmarks, clnf_model.face_model->params_local);
    tmp_3d_landmarks = tmp_3d_landmarks.reshape(1, 3).t();
    assert(tmp_3d_landmarks.total() == 68 * 3);
    std::copy(tmp_3d_landmarks.begin(), tmp_3d_landmarks.end(), &(landmarks_3d[0][0]));
        
 }
%}
