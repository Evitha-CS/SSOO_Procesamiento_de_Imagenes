#include <iostream>
#include <opencv2/opencv.hpp>
#include <omp.h>

using namespace cv;
using namespace std;

int main()
{
    // Cargar la imagen a color desde un archivo
    Mat image = imread("imagen_a_color.jpg", IMREAD_COLOR);

    // Verificar si la imagen se ha cargado correctamente
    if (image.empty())
    {
        cerr << "Error al cargar la imagen" << endl;
        return -1;
    }

    // Crear una nueva imagen en escala de grises con el mismo tamaño que la imagen original
    Mat grayscaleImage(image.rows, image.cols, CV_8UC1);

    // Utilizar OpenMP para paralelizar el bucle que recorre cada píxel de la imagen a color
    #pragma omp parallel for
    for (int r = 0; r < image.rows; r++)
    {
        for (int c = 0; c < image.cols; c++)
        {
            // Obtener un puntero al píxel en la posición (r, c) de la imagen a color
            Point3_<uchar> *p = image.ptr<Point3_<uchar>>(r, c);

            // Calcular el valor en escala de grises utilizando la fórmula estándar
            uchar gray = static_cast<uchar>(0.299 * p->z + 0.587 * p->y + 0.114 * p->x);
            
            // Utilizar una región crítica para garantizar operaciones seguras al modificar la imagen en escala de grises
            #pragma omp critical
            grayscaleImage.at<uchar>(r, c) = gray;

            // Utilizar otra región crítica para imprimir el valor de gris
            #pragma omp critical
            printf("(%d) ", gray);
        }
    }

    // Mostrar la imagen en escala de grises
    imshow("Escala de grises", grayscaleImage);
    waitKey(0);

    // Guardar la imagen en escala de grises como un nuevo archivo
    imwrite("imagen_escala_grises.jpg", grayscaleImage);

    return 0;
}

