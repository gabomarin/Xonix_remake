cmake_minimum_required(VERSION 3.10)

project(Xonix)

# Especificar el estándar de C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Buscar Boost
find_package(Boost REQUIRED)

# Buscar SDL y sus módulos
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)

# Buscar OpenGL
find_package(OpenGL REQUIRED)

# Incluir directorios
include_directories(${Boost_INCLUDE_DIRS} ${SDL_INCLUDE_DIRS} ${SDL_IMAGE_INCLUDE_DIRS} ${SDL_TTF_INCLUDE_DIRS})

# Agregar los archivos fuente
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

# Crear el ejecutable
add_executable(Xonix ${SOURCES} ${HEADERS})

# Enlazar bibliotecas
target_link_libraries(Xonix ${Boost_LIBRARIES} ${SDL_LIBRARIES} ${SDL_IMAGE_LIBRARIES} ${SDL_TTF_LIBRARIES} ${OPENGL_LIBRARIES})

install(TARGETS Xonix RUNTIME DESTINATION bin)

set(INSTALL_DIR_INCLUDE ${CMAKE_INSTALL_PREFIX}/include)
set(INSTALL_DIR_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(resources)