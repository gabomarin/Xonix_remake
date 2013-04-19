Xonix_remake
============

Remake of the arcade game xonix written in C++


####################################
#  +Ortiz Valdovinos Gabriel       #
#                                  #
####################################

################################## REQUISITOS ##################################################################
  +OpenGL
  +SDL
  +SDL_IMAGE
  +SDL_ttf
  -boostlibs 
  
  +Para una compilacion mas facil un sistema operativo linux con kdevelop instalado
  +Linux cmake
  +En windows tener CMAKE instalado  y saber compilar con cmakeslists (no probada la compilacion de esa forma)

################################################################################################################



  
################################## COMPILACION #################################################################

 +Si se esta utilizando KDevelop
    -Proyecto -> Abrir/Importar -> Seleccionar el archivo cmakelists.txt que esta dentro de la carpeta principal
      del proyecto.
    -Una vez abierto, darle al boton de construir y esperar a que compile
    -Para ejecutar seleccionar el lanzador primero: Ejecutar-> Configurar lanzamientos y seleccionar el ejecutable
      del proyecto actual.
      
 +Utilizando CMAKE en linux
    -Utilizando la terminal, navegar a la carpeta raiz del proyecto
    -Ejecutar el comando:  cmake ../Xonix -G "Unix Makefiles"
    -Despues este comando: make install (con permisos de administrador)
    -En la consola aparecera la ruta donde se guardo el ejecutable
    -Mover la carpeta "Resources" a la ruta del ejecutable ( en mi caso /usr/local/bin)
    - Probar el avance del juego
  
  +Windows
    -Aun no probado
    
################################################################################################################

################################## NOTAS ADICIONALES ###########################################################

+ Pasar la carpeta completa de "resources" dentro de la carpeta donde se encuentra el ejecutable, si no esta en
  el lugar indicado el programa no funcionara. En caso de haber usado kdevelop debe ponerse dentro de la carpeta
  llamada "build"
  
+ Si la libreria boost no se encuentra instalada entonces borrar la linea de los include  de la libreria boost
  ya que fue añadida automaticamente por kdevelop pero no se utiliza.
  
  
 -No se incluye el codigo fuente de xonix en allegro debido al limite de archivos del proymoodle, solo se deja
  una captura
  
 -Cualquier duda o problema contactarme al correo gabrielortiz_26@hotmail.com
################################################################################################################
