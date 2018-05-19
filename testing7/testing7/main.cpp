#include<opencv2\opencv.hpp>
#include<iostream>
#include<string>

using namespace cv;
using namespace std;

int main()
{
	//load image
	Mat original = imread("dilatedImage.jpg", CV_LOAD_IMAGE_UNCHANGED);

	//thresholding
	threshold(original, original, 100, 255, THRESH_BINARY);

	//inverting
	//bitwise_not(original, original);

	//connected components
	Mat labels, stats, centroids;
	int nLabels = connectedComponentsWithStats(original, labels, stats, centroids, 8, CV_32S);
	cout << "Number of connected components found : " << nLabels;

	//cropping connected components
	for (int i = 1; i < nLabels; i++) //iterating over total number of connected components found
	{
		int x = stats.at<int>(i, CC_STAT_LEFT);
		int y = stats.at<int>(i, CC_STAT_TOP);
		int width = stats.at<int>(i, CC_STAT_WIDTH);
		int height = stats.at<int>(i, CC_STAT_HEIGHT);
		int area = stats.at<int>(i, CC_STAT_AREA);

		//check area, to exclude small particle and long lines
		if (area > 225 && area < 10000)
		{
			//actual cropping
			Rect cropped = Rect(x, y, width, height);
			Mat cropped_img = original(cropped);

			//cropping to 28*28
			resize(cropped_img, cropped_img, Size(28, 28), 0, 0, CV_INTER_AREA);

			//writing file
			string filename = "aropped_" + to_string(i) + ".jpg";
			imwrite(filename, cropped_img); cout << "end";
		}
	}

	waitKey(0);
	return 0;
}