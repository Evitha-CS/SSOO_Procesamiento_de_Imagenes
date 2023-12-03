#include <iostream>

#include <opencv2/opencv.hpp>

using namespace cv;

using namespace std;

int main()
{

    // Leer la imagen desde el archivo.

    Mat image = imread("imagen_a_color.jpg", IMREAD_COLOR);

    if (image.empty())
    {

        cerr << "Error al cargar la imagen" << endl;

        return -1;
    }

    // Crear una nueva imagen en escala de grises.

    Mat grayscaleImage(image.rows, image.cols, CV_8UC1);

    // Se convierte la imagen a escala de grises utilizando el método de luminosidad.

    // Gray=0.299×Red+0.587×Green+0.114×Blue

    for (int r = 0; r < image.rows; r++)
    {

        for (int c = 0; c < image.cols; c++)
        {

            Point3_<uchar> *p = image.ptr<Point3_<uchar>>(r, c);

            // Calcular el valor de luminosidad.

            uchar gray = static_cast<uchar>(0.299 * p->z + 0.587 * p->y + 0.114 * p->x);

            // Asignar el valor de luminosidad a la nueva imagen en escala de grises.

            grayscaleImage.at<uchar>(r, c) = gray;

            // Imprimir el valor de luminosidad.

            printf("(%d) ", gray);
        }
    }

    // Una vez terminada la transformacion se muestra la imagen.

    imshow("Escala de grises", grayscaleImage);

    waitKey(0);

    // Para guargar la nueva imagen generada

    imwrite("imagen_escala_grises.jpg", grayscaleImage);

    return 0;
}