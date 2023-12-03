#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>
#include <chrono>

using namespace cv;

using namespace std;

using namespace chrono;

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

int main(int argc, char *argv[])
{

    // Verificar los parametros

    if (argc != 4)
    {
        cerr << "Faltan parametros a ingresar" << endl;
        cerr << "Se deben ingresar: <imagen_a_color> <imagen_escala_grises> <num_hebras>" << endl;

        return -1;
    }

    const string inputFileName = argv[1];

    const string outputFileName = argv[2];

    const int numThreads = stoi(argv[3]); // se convierte a entero

    // Carga la imagen a color desde el archivo especificado
    Mat image = imread(inputFileName, IMREAD_COLOR);

    cout << "Cargando Imagen..." << endl;

    if (image.empty())
    {
        cerr << "Error al cargar la imagen" << endl;

        return -1;
    }

    // Crear una nueva imagen en escala de grises.
    Mat grayscaleImage(image.rows, image.cols, CV_8UC1);

    cout << "Filas (altura): " << image.rows << " Columnas (ancho): " << image.cols << endl;

    // Vector para almacenar las hebras
    vector<thread> threads;

    // Dividir la imagen en partes y asignar cada parte a una hebra
    int rowsPerThread = image.rows / numThreads;

    int startRow = 0;

    int endRow = rowsPerThread;

    // Inicia el temporizador
    auto start = high_resolution_clock::now();

    cout << "Iniciando conversión..." << endl;

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

    cout << "Fin de la conversión..." << endl;

    // Detener el temporizador
    auto stop = high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start);

    cout << "Tiempo total transcurrido en segundos: " << duration.count() << endl;

    // Mostrar la imagen en escala de grises.
    imshow("Escala de grises", grayscaleImage);

    waitKey(0);

    // Para guargar la nueva imagen generada
    imwrite(outputFileName, grayscaleImage);

    return 0;
}