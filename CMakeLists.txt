# Definir a versão mínima do CMake
cmake_minimum_required(VERSION 3.29)

# Definir o nome do projeto
project(DataSetGrey_Serial)

# Definir o padrão C++20
set(CMAKE_CXX_STANDARD 20)

# Defina o caminho para o vcpkg caso você ainda não tenha integrado o vcpkg ao CLion (se já fez isso no CLion, pode pular esta linha)
set(CMAKE_TOOLCHAIN_FILE "C:/Users/kayss/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Adicionar o arquivo fonte principal
add_executable(DataSetGrey_Serial main.cpp)

# Linkando as bibliotecas necessárias
# Se você usou o vcpkg, a libjpeg será automaticamente vinculada
find_package(JPEG REQUIRED)
target_link_libraries(DataSetGrey_Serial PRIVATE JPEG::JPEG)

# Se você estiver usando bibliotecas externas adicionais, pode continuar configurando o CMake aqui