import time
import numpy as np
import cv2
import os
from dist import pyopenface

TEST_VIDEO = '../OpenFace/samples/2015-10-15-15-14.avi'
def get_default_intrisic_mat(frame_width, frame_height):
    """ set default camera intrisic parameres, see SequenceCapture::SetCameraIntrinsics for more details.
    Return:
        fx, fy, cx, cy.
    """
    fx = 500.0 * frame_width / 640.0 
    fy = 500.0 * frame_height / 480.0
    mean_fxy = (fx + fy) / 2
    return mean_fxy, mean_fxy, frame_width / 2, frame_height / 2

assert os.path.exists('./model'), 'make sure model dirctory position'
face_param = pyopenface.FaceModelParameters()
clnf = pyopenface.WCLNF(face_param.model_location)
cap = cv2.VideoCapture(TEST_VIDEO)
while True:
    ret, img = cap.read()
    gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    rgb_img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    start_t = time.time()
    success = pyopenface.DetectLandmarksInVideo(rgb_img, gray_img, clnf, face_param)
    print('success:{}, cost time:{}'.format(success, time.time() - start_t))
    fx, fy, cx, cy = get_default_intrisic_mat(rgb_img.shape[1], rgb_img.shape[0])
    tx, ty, tz, rx, ry, rz = pyopenface.GetPose(clnf, fx, fy, cx, cy)
    print('{},{},{},{},{},{}'.format(tx, ty, tz, rx, ry, rz))
    cv2.putText(img, 'headpose:[{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f}]'.format(tx, ty, tz, rx, ry, rz), (0, 30), cv2.FONT_HERSHEY_COMPLEX, 1.0, (0, 255, 0))
    landmarks = np.reshape(clnf.detected_landmarks(), (2, -1)).T.astype(np.int32)
    for pt in landmarks:
        cv2.circle(img, tuple(pt), 2, (0, 0, 255))
    print('3dshape:{}'.format(pyopenface.CalcShape3D(clnf)))
    cv2.imshow('img', img)
    cv2.waitKey(1)
