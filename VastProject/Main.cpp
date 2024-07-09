#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostrem>




int main()
{
	//cv::Mat img = cv::imread("C:\Deve\opencv")
	cv::namedWindow("First CV app", 0, 45);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}