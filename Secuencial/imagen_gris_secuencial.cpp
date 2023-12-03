#include <iostream>

#include <opencv2/opencv.hpp>

#include <chrono>

using namespace cv;

using namespace std;

using namespace chrono;

int main(int argc, char *argv[])
{

    // Verificar los parametros

    if (argc != 3)

    {
        cerr << "Uso: " << argv[0] << " <imagen_a_color> <imagen_escala_grises>" << endl;

        return -1;
    }

    const string inputFileName = argv[1];

    const string outputFileName = argv[2];

    // Carga la imagen a color desde el archivo especificado

    Mat image = imread(inputFileName, IMREAD_COLOR);

    cerr << "Cargando Imagen..." << endl;

    if (image.empty())
    {
        cerr << "Error al cargar la imagen" << endl;
        return -1;
    }

    // Crear una nueva imagen en escala de grises.

    Mat grayscaleImage(image.rows, image.cols, CV_8UC1);

    cout << "Filas (altura): " << image.rows << " Columnas (ancho): " << image.cols << endl;

    // Se convierte la imagen a escala de grises utilizando el método de luminosidad.

    // Gray=0.299×Red+0.587×Green+0.114×Blue

    // Inicia el temporizador

    auto start = high_resolution_clock::now();

    cerr << "Iniciando conversión..." << endl;

    for (int r = 0; r < image.rows; r++)
    {

        for (int c = 0; c < image.cols; c++)
        {
            Point3_<uchar> *p = image.ptr<Point3_<uchar>>(r, c);

            // Calcular el valor de luminosidad.

            uchar gray = static_cast<uchar>(0.299 * p->z + 0.587 * p->y + 0.114 * p->x);

            // Asignar el valor de luminosidad a la nueva imagen en escala de grises.

            grayscaleImage.at<uchar>(r, c) = gray;
        }
    }

    cerr << "Fin de la conversión..." << endl;

    // Detener el temporizador

    auto stop = high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start);

    cout << "Tiempo total transcurrido en segundos: " << duration.count() << endl;

    // Una vez terminada la transformacion se muestra la imagen.

    imshow("Escala de grises", grayscaleImage);

    resizeWindow("Escala de grises", 800, 600);

    waitKey(0);

    // Para guargar la nueva imagen generada

    imwrite(outputFileName, grayscaleImage);

    return 0;
}
