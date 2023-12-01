#include <iostream>
#include <opencv2/opencv.hpp>
#include <omp.h>

using namespace cv;
using namespace std;

int main()
{
    Mat image = imread("imagen_a_color.jpg", IMREAD_COLOR);

    if (image.empty())
    {
        cerr << "Error al cargar la imagen" << endl;
        return -1;
    }

    Mat grayscaleImage(image.rows, image.cols, CV_8UC1);

    #pragma omp parallel for
    for (int r = 0; r < image.rows; r++)
    {
        for (int c = 0; c < image.cols; c++)
        {
            Point3_<uchar> *p = image.ptr<Point3_<uchar>>(r, c);
            uchar gray = static_cast<uchar>(0.299 * p->z + 0.587 * p->y + 0.114 * p->x);
            
            #pragma omp critical
            grayscaleImage.at<uchar>(r, c) = gray;

            // Esta impresión puede dar lugar a resultados desordenados en la consola.
            // Si la precisión del orden es esencial, considere no imprimir en paralelo.
            #pragma omp critical
            printf("(%d) ", gray);
        }
    }

    imshow("Escala de grises", grayscaleImage);
    waitKey(0);

    imwrite("imagen_escala_grises.jpg", grayscaleImage);

    return 0;
}

