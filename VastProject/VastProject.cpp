// VastProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <conio.h>


#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

#include "MiningSimulation.h"
#include "OpenCVGraphicsSystem.h"

using namespace std;


static std::chrono::steady_clock::time_point startTime;
static std::chrono::steady_clock::time_point lastUpdateTime;


/*
void InitGraphics()
{

    // Load the cascade
    cv::CascadeClassifier eye_cascade;
    if (!eye_cascade.load("opencv/sources/data/haarcascades/haarcascade_eye.xml")) {
        std::cerr << "Failed to load eye cascade." << std::endl;
        return;
    }

    // Load the image
   // cv::Mat img = cv::imread("path_to_your_image.jpg");
    cv::Mat img = imread("Image.jpg");

    if (img.empty()) 
    {
        std::cerr << "Could not open or find the image" << std::endl;
        return;
    }


    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display

    // Show our image inside the created window
    cv::imshow("Display window", img);

    // Wait for any keystroke in the window
    cv::waitKey(0);

    cv::destroyAllWindows();


}

void InitGraphics2()
{

   // Mat img = imread("Image.jpg");
    cv::Mat img = cv::Mat::zeros(cv::Size(800, 600), CV_8UC3);

    if (img.empty())
    {
        std::cerr << "Could not open or find the image" << std::endl;
        return;
    }

    cv::circle(img, cv::Point(50, 50), 10, cv::Scalar(255, 255, 255), cv::FILLED);

    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display

    // Show our image inside the created window
    cv::imshow("Display window", img);

    cv::waitKey(1);


}
*/
void PrintTimeSinceStart(string label)
{
    auto now = std::chrono::steady_clock::now();

    // Calculate the difference between now and lastUpdateTime
    auto deltaTime = now - startTime;

    // Cast the difference to microseconds and normalize to seconds
    double deltaTimeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(deltaTime).count() / 1000000.0;

    long long idtInSeconds = 10000 * (long long)deltaTimeInSeconds;

    deltaTimeInSeconds = (double)(idtInSeconds) / 10000.0;

    // Use deltaTimeInSeconds for your calculations
    if (label.empty())
        label = " Print Time ";

    std::cout << std::endl << "######"  << label << "######" << std::endl;

    std::cout << "Time Interval: " << (float)deltaTimeInSeconds << " seconds\n";
}

void PrintDeltaTime(string label)
{
    auto now = std::chrono::steady_clock::now();

    // Calculate the difference between now and lastUpdateTime
    auto deltaTime = now - lastUpdateTime;

    // Cast the difference to microseconds and normalize to seconds
    double deltaTimeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(deltaTime).count() / 1000000.0;

    long long idtInSeconds = 10000 * (long long)deltaTimeInSeconds;

    deltaTimeInSeconds = (double)(idtInSeconds) / 10000.0;

    // Update lastUpdateTime for the next iteration
    lastUpdateTime = now;

    // Use deltaTimeInSeconds for your calculations
    if (label.empty())
        label = " Print Time ";

    std::cout << std::endl << "######" << label << "######" << std::endl;

    std::cout << "Time Interval: " << (float)deltaTimeInSeconds << " seconds\n";
}


// ****************************
// Entry point
// ****************************
    int main()
    {
        MiningSimulation* theSimulation = new MiningSimulation();

      //  OpenCVGraphicsSystem *graphicsSystem = new OpenCVGraphicsSystem();

      //  graphicsSystem->Initialize(theSimulation);
  
        bool complete = false;

        theSimulation->SetDeltaTime(SIM_EVAL_GRANULAROTY_SECONDS_F);

        // Reset this
        lastUpdateTime = startTime = std::chrono::steady_clock::now();
      
        PrintTimeSinceStart("Simulation Start: ");

        while (!complete)
        {
            complete = theSimulation->Execute();

            std::this_thread::sleep_for(std::chrono::microseconds(SIM_WAIT_FRAME_IN_MILLISECONDS));

          //  cout << "Hit any key..." << endl;
          //  char ch = _getch(); // Note: _getch() is used instead of getch() for compatibility with MinGW

        }
      //  PrintTimeSinceStart("Loop End: ");

        cout << "Simulation Complete" <<  " Collected Ore: " << theSimulation->GetCollectedOre() <<endl;

        cout << "Done.Hit any key..." << endl;

      //  graphicsSystem->StopUpdating();

        // Wait for any key press
      //  char ch = _getch(); // Note: _getch() is used instead of getch() for compatibility with MinGW

      //  graphicsSystem->ShutDown();
        
        return 0;
    }

