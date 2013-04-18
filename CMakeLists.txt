project(xonix)

add_executable(xonix src/enemigo.cpp src/poligono.cpp src/timer.cpp src/personaje.cpp  main.cpp src/xonix.cpp)

install(TARGETS xonix RUNTIME DESTINATION bin)

#add_definitions(sdl-config --cflags --libs)
set(INSTALL_DIR_INCLUDE ${CMAKE_INSTALL_PREFIX}/include)
set(INSTALL_DIR_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_EXE_LINKER_FLAGS "-lSDL -lGL -lSDL_image -lSDL_ttf -zmuldefs")
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(resources)
