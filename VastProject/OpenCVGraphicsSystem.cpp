#include "OpenCVGraphicsSystem.h"
#include "MiningSimulation.h"


#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <thread>

#include <windows.h>

#include "MiningSimulation.h"
#include "Truck.h"
#include "Mine.h"
#include "Station.h"

using namespace cv;
using namespace std;

OpenCVGraphicsSystem::OpenCVGraphicsSystem()
{
}

OpenCVGraphicsSystem::~OpenCVGraphicsSystem()
{
}


void OpenCVGraphicsSystem::Initialize(MiningSimulation* miningSimilation)
{
    mMiningSimulation = miningSimilation;

    // Fullscv::creen
    mDimX = GetSystemMetrics(SM_CXSCREEN);
    mDimY = GetSystemMetrics(SM_CYSCREEN);

    // Create sub Image for window
    mMainCanvas = cv::Mat::zeros(cv::Size(mDimX, mDimY), CV_8UC3);;//imread("Image.jpg");

    if (mMainCanvas.empty())
    {
        std::cerr << "Could not open or find the image" << std::endl;
        return;
    }

    mDrawThread = std::thread(&OpenCVGraphicsSystem::DrawThread, this);
   
    
}

void OpenCVGraphicsSystem::ComputeUpdateAreas()
{
    cv::Rect windowRect = cv::getWindowImageRect(mWindowName);

    // Timeline
  //  mDimX = windowRect.width;
  //  mDimY = windowRect.height;

    mTimelineBeginX =  (int)((float)windowRect.width * 0.1f);
    mTimelineEndX = (int)(windowRect.width - (int)((float)windowRect.width * 0.1f));

    mTimelineBeginY = (int)((float)windowRect.height * 0.1f);
    mTimelineEndY = mTimelineBeginY + (int)((float)windowRect.height * 0.1f);

    mTimelineWidth  = mTimelineEndX - mTimelineBeginX;
    mTimelineHeight = mTimelineEndY - mTimelineBeginY;

    mUpdateAreaBeginX = mTimelineBeginX;
    mUpdateAreaEndX   = mTimelineEndX;

    mUpdateAreaBeginY = mTimelineBeginY + mTimelineHeight;
    mUpdateAreaEndY = (int)((float)windowRect.height * 0.9f);

    mUpdateAreaWidth = mDimX;
    mUpdateAreaHeight = mDimY - mUpdateAreaBeginY;

}

void  OpenCVGraphicsSystem::DrawThread()
{
    int loopCount = 0;
    fakeX = 0;

    namedWindow(mWindowName, cv::WINDOW_FULLSCREEN);

    cv::moveWindow(mWindowName, 0, 0);

    ComputeUpdateAreas();

    DrawTimelineFrame();

    // Start the frame swapper
    cv::waitKey(1);

    // I think Atomic will protect this
    mKillDrawThread = false;

    auto startTime = chrono::high_resolution_clock::now();
    const int fps = 30; // Target frames per second
    const int interval = 1000000000 / fps; // Interval in nanoseconds

    while (!mKillDrawThread)
    {
        auto currentTime = chrono::high_resolution_clock::now();
        auto elapsedTime = chrono::duration_cast<chrono::nanoseconds>(currentTime - startTime).count();

        loopCount++;

        if (elapsedTime >= interval)
        {
            DrawSimulation();

          //  DrawTimelineFrame();
            startTime = chrono::high_resolution_clock::now();

            // std::this_thread::sleep_for(std::chrono::milliseconds((long long)1));

        //     cout << "lc " << loopCount << endl;

            loopCount = 0;
        }
    }
}



Scalar ColorFromTime(float time)
{
    float normalizedTime = time / SIM_MAX_MISSION_IN_SECONDS_F;

    return cv::Scalar((int)(255.0f* normalizedTime), (int)(255.0f * (1.0f-normalizedTime)), (int)(255.0f * normalizedTime));
}

void OpenCVGraphicsSystem::DrawSimulation()
{
    if (mMiningSimulation == nullptr)
        return;

    if (mKillDrawThread)
        return;

   
    std::vector<Mission*> activeMissions = mMiningSimulation->GetActiveMissions();

    if (activeMissions.size() < 1)
        return;
 
   
    //cv::Rect clearRect(0, mUpdateAreaBeginY, mUpdateAreaWidth, mUpdateAreaHeight);
    cv::Rect clearRect(0, 0, mDimX, mDimY);

    // Choose the color
    cv::Scalar color(0, 255, 255); // Blue color

    // Set the thickness to -1 for a filled rectangle
    int thickness = -1;

    // Draw the filled rectangle
    cv::rectangle(mMainCanvas, clearRect, color, thickness);

    bool doMissions = false;
  
    fakeY = 50;
    if (doMissions)
    {
        for (auto missionIter : activeMissions)
        {
            float elapsedTime = missionIter->GetElasedTime();

            Scalar color = ColorFromTime(elapsedTime);


            int xOffset = - (int)elapsedTime;

            // Draw Right to left
            cv::Point point(mUpdateAreaEndX-fakeX, mUpdateAreaBeginY+fakeY);

            // Draw a circle on mMainCanvas
            circle(mMainCanvas, point, 10, color, FILLED);

            fakeX += 2;
            fakeY += 50;


            DrawMissionText(missionIter, point);

        }
    }

  
    // Swap buffers for display
    imshow(mWindowName, mMainCanvas);

    waitKey(1);

}

void OpenCVGraphicsSystem::DrawTimelineFrame()
{

/*
  //  cout << "Window Begin X" << mTimelineBeginX << "Window End X " << mTimelineEndX << endl;

    // Define the start and end points of the line
    cv::Point startPoint(mTimelineBeginX, mTimelineBeginY);
    cv::Point endPoint(mTimelineEndX, mTimelineBeginY);

    // Define the color of the line (in BGR)
    cv::Scalar lineColor(255, 255, 255); // Blue color

    // Define the thickness of the line
    int thickness = 2;

    // Define the line type
    int lineType = cv::LINE_AA; // Anti-aliased line

    // Draw the line on the image
    cv::line(mMainCanvas, startPoint, endPoint, lineColor, thickness, lineType);

    imshow(mWindowName, mMainCanvas);
*/

    cv::Rect clearRect(0, mTimelineBeginY,mDimX, mTimelineHeight);

    // Choose the color
    cv::Scalar color(255, 255, 255); // White color

    // Set the thickness to -1 for a filled rectangle
    int thickness = 1;

    // Draw the filled rectangle
    cv::rectangle(mMainCanvas, clearRect, color, thickness);

    imshow(mWindowName, mMainCanvas);
}




void    OpenCVGraphicsSystem::DrawMissionText(Mission* missionIter, cv::Point &point)
{
    // Specify the text to display
    std::string text = "Mission ";


    // Define the rectangle
    cv::Rect rect2(point.x, point.y, 150, 150); // Top-left corner at (50,50), width 200, height 100
    cv::Rect rect(point.x, point.y, 50, 50); // Top-left corner at (50,50), width 200, height 100

    // Create a mask of the same size as the image, filled with zeros
    cv::Mat mask = cv::Mat::zeros(mMainCanvas.size(), CV_8U);

    // Set the rectangle in the mask to white (255)
    mask(rect) = 255;

    std::stringstream ss;
    ss << text << missionIter->GetID();

    ss << " Truck:" << missionIter->GetTruckID();
    ss << " From Station:" << missionIter->GetTruckID();
    ss << " To Mine:" << missionIter->GetStation()->GetID();
    text = ss.str();

    // Specify the font type
    int fontFace = cv::FONT_HERSHEY_PLAIN;

    // Specify the font scale
    double fontScale = 1;

    // Specify the color of the text
    cv::Scalar textColor(255, 255, 255); // White color

    // Specify the thickness of the lines used to draw a text
    int thickness = 2;

    // Put the text on the image
    cv::putText(mMainCanvas, text, point, fontFace, fontScale, textColor, thickness);


    // Choose the color
    cv::Scalar color(0, 255, 0); // White color

    // Set the thickness to -1 for a filled rectangle
    

    // Draw the filled rectangle
    cv::rectangle(mMainCanvas, rect2, color, -1);

    // img.copyTo(img, mas
    mMainCanvas.copyTo(mMainCanvas, mask);

}

void OpenCVGraphicsSystem::ShutDown()
{
    mKillDrawThread = true;
    cv::destroyAllWindows();
}

