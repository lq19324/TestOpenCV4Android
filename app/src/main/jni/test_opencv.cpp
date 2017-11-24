#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

using namespace cv;

template<typename Ty>
void printMatrix(const Mat_<Ty> &m) {
    printf("matrix:\n");
    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            printf("%d ", (int)(m(i, j)));
        }
        printf("\n");
    }
}

int add_matrix() {
    Mat_<int> a(3, 3);
    Mat_<int> b(3, 3);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            a(i, j) = i*3 + j;
            b(i, j) = i*3 + j + 1;
        }
    }

    Mat_<int> c = a + b;

    printMatrix(a);
    printMatrix(b);

    printf("matrix c:\n");
    printMatrix(c);

    Mat R = Mat(2, 2, CV_8UC4);
    int data[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 10, 11, 12, 13, 14, 15};
    return 0;
}


//int main() {
//    add_matrix();
//    return 0;
//}