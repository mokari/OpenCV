#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <functional>

using namespace cv;

void show_mat(const cv::Mat &image, std::string const &win_name);

cv::Mat &invert_mat(cv::Mat &mat);

cv::Mat& invert_mat_pointer(cv::Mat &mat);

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("usage: Opencv_Test <Image_Path> <Output_Path>\n");
        return -1;
    }

    Mat image;
    image = imread(argv[1], 1);

    if (!image.data) {
        printf("No image data (warning: OpenCV recognize files by extensions)\n");
        return -1;
    }

    show_mat(image, "Input");

    image = invert_mat_pointer(image);

    show_mat(image, "Output");

    imwrite(argv[2], image);

    return 0;
}

template< typename T>
void ForAllPixels(cv::Mat &mat, std::function <void (MatIterator_<T>&)> handler)
{
	auto it = mat.begin<T>();
	auto end = mat.end<T>();
	for (; it != end; ++it)
		handler(it);
}

cv::Mat& invert_mat(cv::Mat &mat) {
    // accept only char type matrices
    CV_Assert(mat.depth() == CV_8U);

    const int channels = mat.channels();
    switch (channels) {
        case 1: {
//            gray scale image
			ForAllPixels<uchar>(mat, [](auto& it) {*it = ~*it; });
			break;
        }
        case 3: {
//            RGB image
			ForAllPixels<Vec3b>(mat, [](auto& it) {
				(*it)[0] = ~(*it)[0];
				(*it)[1] = ~(*it)[1];
				(*it)[2] = ~(*it)[2]; });
        }
    }

    return mat;
}

cv::Mat& invert_mat_pointer(cv::Mat &mat) {
    int channels = mat.channels();

    int nRows = mat.rows;
    int nCols = mat.cols;

    int i, j;
    switch (channels) {
        case 1: {
//            gray scale image
            uchar *p;
            for (i = 0; i < nRows; ++i) {
                p = mat.ptr<uchar>(i);
                for (j = 0; j < nCols; ++j) {
                    p[j] = ~p[j];
                }
            }
        }
        case 3: {
//            RGB image
            Vec3b *p;
            for (i = 0; i < nRows; ++i) {
                p = mat.ptr<Vec3b>(i);
                for (j = 0; j < nCols; ++j) {
                    p[j][0] = ~p[j][0];
                    p[j][1] = ~p[j][1];
                    p[j][2] = ~p[j][2];
                }
            }
        }
    }

    return mat;
}

void show_mat(const cv::Mat &image, std::string const &win_name) {
    namedWindow(win_name, WINDOW_AUTOSIZE);
    imshow(win_name, image);
    waitKey(0);
}
