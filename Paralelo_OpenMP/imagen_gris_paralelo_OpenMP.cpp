#include <iostream>
#include <opencv2/opencv.hpp>
#include <omp.h>
#include <chrono>

using namespace cv;
using namespace std;
using namespace chrono;

int main(int argc, char *argv[])
{
    // Verificar la cantidad correcta de argumentos
    if (argc != 4)
    {
        cerr << "Uso: " << argv[0] << " <imagen_a_color> <imagen_escala_grises> <num_hebras>" << endl;
        return -1;
    }

    // Obtener nombres de archivo y número de hebras desde la línea de comandos
    string nombreImagenColor = argv[1];
    string nombreImagenGrises = argv[2];
    int numHebras = atoi(argv[3]);

    cout << "Cargando imagen. . ." << endl;

    // Cargar la imagen a color desde un archivo
    Mat image = imread(nombreImagenColor, IMREAD_COLOR);

    // Verificar si la imagen se ha cargado correctamente
    if (image.empty())
    {
        cerr << "Error al cargar la imagen" << endl;
        return -1;
    }

    // Obtener las dimensiones de la imagen
    int height = image.rows;
    int width = image.cols;

    cout << "Filas (altura): " << height << " Columnas (ancho): " << width << endl;

    // Crear una nueva imagen en escala de grises con el mismo tamaño que la imagen original
    Mat grayscaleImage(height, width, CV_8UC1);

    cout << "Iniciando conversión..." << endl;

    // Medir el tiempo de ejecución usando la clase chrono
    auto start_time = high_resolution_clock::now();

    // Utilizar OpenMP para paralelizar el bucle que recorre cada píxel de la imagen a color
    #pragma omp parallel for num_threads(numHebras)
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            Point3_<uchar> *p = image.ptr<Point3_<uchar>>(r, c);
            uchar gray = static_cast<uchar>(0.299 * p->z + 0.587 * p->y + 0.114 * p->x);

            #pragma omp critical
            grayscaleImage.at<uchar>(r, c) = gray;
        }
    }

    // Medir el tiempo de ejecución final
    auto end_time = high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);

    cout << "Conversión finalizada..." << endl;
    cout << "Tiempo total transcurrido en segundos: " << duration.count() << endl;

    // Mostrar la imagen en escala de grises (descomentar si se desea)
    //imshow("Escala de grises", grayscaleImage);
    //waitKey(0);

    // Guardar la imagen en escala de grises como un nuevo archivo
    imwrite(nombreImagenGrises, grayscaleImage);

    return 0;
}


