# Conversão serial de JPEG para tons de cinza

Experimento em C++ que converte pixels em tons de cinza na CPU. [main.cpp](main.cpp) usa libjpeg para ler a imagem, cria uma cópia para cada conversão e mede o tempo dos lotes de 1, 10, 100 e 1.000 arquivos. Cada lote usa a mesma imagem de origem.

## Como executar

Use CMake 3.29 ou superior, um compilador C++20 e libjpeg. A partir da raiz:

```bash
cmake -S . -B build
cmake --build build --config Release
```

Com vcpkg, configure o CMake com `-DCMAKE_TOOLCHAIN_FILE="CAMINHO_DO_VCPKG/scripts/buildsystems/vcpkg.cmake"` e disponibilize libjpeg para a arquitetura de compilação.

Execute o alvo `DataSetGrey_Serial` na raiz do repositório. Com Visual Studio, procure `build/Release/DataSetGrey_Serial.exe`. O programa usa `Imagens/IMAGEM.jpg` como entrada e `Imagens` como diretório de saída. Para usar outros caminhos, forneça o JPEG e a pasta de saída como primeiro e segundo argumentos. A entrada precisa ter três canais e dimensões mínimas de 500 × 500. O programa grava os JPEGs nas pastas `out_1`, `out_10`, `out_100` e `out_1000`. Os tempos por lote incluem o processamento e a gravação dos arquivos.
