/**
detectHarrisCorners.cpp
Purpose: Detects corners using Harris corner detection algorithm and performs non-maximum suppression. 
 
@author Urvi Desai
@version 1.1 04/01/2018
*/


#include "detectharriscorners.h"
#include "computeedge.h"

/**
Performs non-maximum suppression on 2d corner points detected by Harris corner detector algorithm.

@param harrisValues - Input matrix of Harris operator values.
harrisCorners - vector of 2d corner points with maximum corner strength.
*/
void DetectHarrisCorners::selectStrongest( const std::vector<cv::Point> &cornerEstimates, const cv::Mat &harrisValue ) {

  int N = cornerEstimates.size();
  int winSize = 20;
  
  for ( int i=0; i<N; i++ ) {
    
    cv::Point currentC = cornerEstimates[i];
    float currentH = harrisValue.at<float>( currentC.y, currentC.x );
    float maxH = currentH;
    cv::Point maxPt = currentC;

    for( int m=-winSize/2; m<winSize/2; m++ ) {
      for( int n=-winSize/2; n<winSize/2; n++ ) {
 
        cv::Point currentPt = cv::Point ( currentC.x+n,currentC.y+m );
        float tempH = harrisValue.at<float>( currentPt.y, currentPt.x );
        if ( tempH > maxH ) {
          maxH = tempH;
          maxPt = currentPt;
        }          

      }
    }

    if (std::find( harrisCorners.begin(), harrisCorners.end(), maxPt) == harrisCorners.end() ) {
      harrisCorners.push_back( maxPt );
    }   
  }
}

/**
Detects harris corners with unweighted window.

@param img - Input image matrix.
@return vector of 2d corner points.
*/
void DetectHarrisCorners::findCorners ( const int harrisRadius, const cv::Mat &img ) {

  // create kernel 
  cv::Mat kernel(1,2, CV_32FC1);
  kernel.at<float>(0,0) = -1;
  kernel.at<float>(0,1) = 1;

  cv::Mat unWeightedKernel(harrisRadius, harrisRadius, CV_32FC1, 1);

  /// x, y derivatives
  cv::Mat imgGray, imgGradX, imgGradY, imgGradXX, imgGradYY, imgGradXY;
  cv::cvtColor( img, imgGray, CV_BGR2GRAY ); 
  cv::filter2D( imgGray, imgGradX, CV_32F, kernel, cv::Point(-1,-1), 0 );
  cv::filter2D( imgGray, imgGradY, CV_32F, kernel.t(), cv::Point(-1,-1), 0 );

  imgGradXX = imgGradX.mul( imgGradX );
  imgGradYY = imgGradY.mul( imgGradY );
  imgGradXY = imgGradX.mul( imgGradY );

  cv::filter2D( imgGradXX, imgGradXX, CV_32F, unWeightedKernel, cv::Point(-1,-1), 0 );
  cv::filter2D( imgGradYY, imgGradYY, CV_32F, unWeightedKernel, cv::Point(-1,-1), 0 );
  cv::filter2D( imgGradXY, imgGradXY, CV_32F, unWeightedKernel, cv::Point(-1,-1), 0 );

  const int height = img.rows;
  const int width = img.cols;
  cv::Mat harrisValue(height, width, CV_32F);

  for( int j = 0; j < height; j++ ){
    for( int i = 0; i < width; i++ ) {
    
      cv::Mat M(2,2,CV_32F);                         // Harris matrix
      M.at<float>(0,0) = imgGradXX.at<float>( j,i );
      M.at<float>(1,0) = imgGradXY.at<float>( j,i );
      M.at<float>(0,1) = imgGradXY.at<float>( j,i );
      M.at<float>(1,1) = imgGradYY.at<float>( j,i );
 
      int k = 0.05;
      float deterH = cv::determinant( M );
      float traceH = M.at<float>(0,0) + M.at<float>(1,1);      
      float cornerStrength = deterH - k*traceH;      // R = Det(M) - k*trace(M)
      harrisValue.at<float>(j,i) = cornerStrength;  
    }
  }
  
  double minVal, maxVal; 
  cv::Point minLoc, maxLoc; 
  cv::minMaxLoc( harrisValue, &minVal, &maxVal, &minLoc, &maxLoc );

  // Store corners
    
  float thresh = 0.35*maxVal; // set threshold to select corner points
  int rowCount = 0;
  for( int j = 0; j < height ; j++ ){
   for( int i = 0; i < width; i++ ) {
     if( harrisValue.at<float>(j,i) > thresh ) {
       cornerEstimates.push_back(cv::Point ( i, j ));
       rowCount++;
      }
    }
  }  
  
  selectStrongest( cornerEstimates, harrisValue );   // Non-maximum suppression
  
  imgGray.release();
  imgGradX.release();
  imgGradY.release();
  imgGradXX.release();
  imgGradYY.release();
  imgGradXY.release();
  
}

