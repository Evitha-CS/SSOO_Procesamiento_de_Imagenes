#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>

using namespace cv;

using namespace std;

// Función para convertir una región de la imagen a escala de grises

void convertToGrayscale(Mat &image, Mat &grayscaleImage, int startRow, int endRow)
{

    for (int r = startRow; r < endRow; r++)
    {
        for (int c = 0; c < image.cols; c++)
        {
            Point3_<uchar> *p = image.ptr<Point3_<uchar>>(r, c);
            uchar gray = static_cast<uchar>(0.299 * p->z + 0.587 * p->y + 0.114 * p->x);
            grayscaleImage.at<uchar>(r, c) = gray;
        }
    }
}

int main()
{

    // Leer la imagen desde el archivo.

    Mat image = imread("imagen_a_color.jpg", IMREAD_COLOR);

    if (image.empty())
    {
        cerr << "Error al cargar la imagen" << endl;
        return -1;
    }

    Mat grayscaleImage(image.rows, image.cols, CV_8UC1); // Crear una nueva imagen en escala de grises.

    const int numThreads = 4; // Número de hebras

    vector<thread> threads; // Vector para almacenar las hebras

    // Dividir la imagen en partes y asignar cada parte a una hebra

    int rowsPerThread = image.rows / numThreads;

    int startRow = 0;

    int endRow = rowsPerThread;

    for (int i = 0; i < numThreads; i++)
    {
        threads.emplace_back(convertToGrayscale, ref(image), ref(grayscaleImage), startRow, endRow);
        startRow = endRow;
        endRow = (i == numThreads - 2) ? image.rows : endRow + rowsPerThread;
    }

    // Esperar a que todas las hebras terminen

    for (auto &t : threads)
    {
        t.join();
    }

    // Mostrar la imagen en escala de grises.

    imshow("Escala de grises", grayscaleImage);
    waitKey(0);

    // Guardar la nueva imagen en escala de grises con otro nombre y en formato JPG.

    imwrite("imagen_escala_grises_thread.jpg", grayscaleImage);

    return 0;
}