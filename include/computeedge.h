#ifndef COMPUTE_EDGE
#define COMPUTE_EDGE

#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


class ComputeEdge {

  public:
    cv::Mat imgEdge;

    cv::Mat detectEdge( const cv::Mat &img );
    
};

#endif  // COMPUTE_EDGE
