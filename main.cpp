#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <jpeglib.h>
#include <chrono>

namespace fs = std::filesystem;

// Definir o caminho do arquivo de imagem aqui (não o diretório)
const std::string image_path =
    "C:/Users/kayss/OneDrive/Documentos/6 periodo/Programacao Paralela/src/DataSetGrey Serial/Imagens/IMAGEM.jpg";

// Função para converter uma imagem para tons de cinza
void convertToGrayscale(std::vector<std::vector<std::vector<unsigned char>>>& image)
{
    int rows = image.size();
    int cols = image[0].size();
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            unsigned char blue = image[i][j][0];
            unsigned char green = image[i][j][1];
            unsigned char red = image[i][j][2];

            unsigned char gray = static_cast<unsigned char>(red * 0.298 + green * 0.587 + blue * 0.114);

            // Atribuir o valor ao pixel (tonalidade de cinza)
            image[i][j][0] = gray;
            image[i][j][1] = gray;
            image[i][j][2] = gray;
        }
    }
}

// Função para ler uma imagem JPEG
bool readJPEG(const std::string& filename, std::vector<std::vector<std::vector<unsigned char>>>& image, int& rows,
              int& cols)
{
    FILE* infile = fopen(filename.c_str(), "rb");
    if (!infile)
    {
        std::cerr << "Could not open file: " << filename << std::endl;
        return false;
    }

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    rows = cinfo.output_height;
    cols = cinfo.output_width;
    int channels = cinfo.output_components;

    if (channels != 3)
    {
        std::cerr << "Unsupported number of channels: " << channels << " in file: " << filename << std::endl;
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return false;
    }

    image.resize(rows, std::vector<std::vector<unsigned char>>(cols, std::vector<unsigned char>(3)));
    unsigned char* row_pointer = new unsigned char[cols * channels];
    while (cinfo.output_scanline < cinfo.output_height)
    {
        int row = cinfo.output_scanline;
        jpeg_read_scanlines(&cinfo, &row_pointer, 1);
        for (int col = 0; col < cols; ++col)
        {
            image[row][col][0] = row_pointer[col * channels]; // Blue
            image[row][col][1] = row_pointer[col * channels + 1]; // Green
            image[row][col][2] = row_pointer[col * channels + 2]; // Red
        }
    }

    delete[] row_pointer;
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return true;
}

// Função para salvar uma imagem JPEG
void saveJPEG(const std::string& filename, const std::vector<std::vector<std::vector<unsigned char>>>& image, int rows,
              int cols)
{
    FILE* outfile = fopen(filename.c_str(), "wb");
    if (!outfile)
    {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return;
    }

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = cols;
    cinfo.image_height = rows;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 85, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    unsigned char* row_pointer = new unsigned char[cols * 3];
    while (cinfo.next_scanline < cinfo.image_height)
    {
        int row = cinfo.next_scanline;
        for (int col = 0; col < cols; ++col)
        {
            row_pointer[col * 3] = image[row][col][0]; // Blue
            row_pointer[col * 3 + 1] = image[row][col][1]; // Green
            row_pointer[col * 3 + 2] = image[row][col][2]; // Red
        }
        jpeg_write_scanlines(&cinfo, &row_pointer, 1);
    }

    delete[] row_pointer;
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(outfile);
}

int main()
{
    auto start_time = std::chrono::high_resolution_clock::now();

    // Ler a imagem original
    int rows, cols;
    std::vector<std::vector<std::vector<unsigned char>>> image;
    if (!readJPEG(image_path, image, rows, cols))
    {
        return 1;
    }

    if (rows < 500 || cols < 500)
    {
        std::cerr << "Imagem menor que 500x500: " << image_path << std::endl;
        return 1;
    }

    // Definir os intervalos de diretórios
    std::vector<int> quantities = {1, 10, 100, 1000};
    for (int quantity : quantities)
    {
        // Marcar o tempo para cada quantidade
        auto start_interval_time = std::chrono::high_resolution_clock::now();

        // Criar diretório de saída
        std::string output_directory =
            "C:/Users/kayss/OneDrive/Documentos/6 periodo/Programacao Paralela/src/DataSetGrey Serial/Imagens/out_" +
            std::to_string(quantity);
        try
        {
            fs::create_directories(output_directory);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error creating output directory: " << e.what() << std::endl;
            return 1;
        }

        // Processar e salvar as imagens
        for (int i = 0; i < quantity; ++i)
        {
            std::vector<std::vector<std::vector<unsigned char>>> image_copy = image; // Criar uma cópia da imagem
            convertToGrayscale(image_copy); // Converter para tons de cinza

            std::string output_path = output_directory + "/IMAGEM_" + std::to_string(i + 1) + "_converted.jpg";
            saveJPEG(output_path, image_copy, rows, cols);
        }

        auto end_interval_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> interval_elapsed_time = end_interval_time - start_interval_time;
        std::cout << "Tempo para " << quantity << " imagens: " << interval_elapsed_time.count() << " segundos" <<
            std::endl;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    std::cout << "Tempo total: " << elapsed_time.count() << " segundos" << std::endl;

    return 0;
}
