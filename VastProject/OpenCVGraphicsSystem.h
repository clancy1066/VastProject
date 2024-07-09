#pragma once

#include <string>
#include <opencv2/opencv.hpp>

// Forward decl
class Mission;
class Point;
class MiningSimulation;
class thread;

class OpenCVGraphicsSystem
{
public:
    OpenCVGraphicsSystem(); // Constructor
 
protected:
    ~OpenCVGraphicsSystem(); // Destructor

    void WaitFrame();

    cv::Mat     mMainCanvas;

    std::string      mWindowName = "Vast Simulation";

    MiningSimulation *mMiningSimulation;


    int     mDimX   = 2000;
    int     mDimY   = 1500;

    void    ComputeUpdateAreas();
    void    DrawThread();
    void    DrawSimulation();
    void    DrawTimelineFrame();
    void    DrawMissionText(Mission* missionIter, cv::Point &point);

    std::atomic<bool>   mKillDrawThread = false;
    std::thread  mDrawThread;

    // For drawing the time line
    int         mTimelineBeginX;
    int         mTimelineEndX;

    int         mTimelineBeginY;
    int         mTimelineEndY;

    int         mTimelineWidth;
    int         mTimelineHeight;

    // For drawing the simulation activity
    int         mUpdateAreaBeginX;
    int         mUpdateAreaEndX;

    int         mUpdateAreaBeginY;
    int         mUpdateAreaEndY;

    int         mUpdateAreaWidth;
    int         mUpdateAreaHeight;


    // For simulating mission status
int fakeX,fakeY=50;

    

public:
    void    Initialize(MiningSimulation* miningSimilation);

    void    StopUpdating() { mKillDrawThread = true; }

    void    ShutDown();

//    int drawX = 50;
//    int drawY = 50;
};
