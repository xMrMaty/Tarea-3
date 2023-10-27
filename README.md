**Descripción:**
  - En esta tarea, debemos desarrollar una aplicación capaz de indexar libros del proyecto Gutenberg. La aplicación debe permitir cargar documentos de texto, mostrar libros ordenados, realizar búsquedas por título, encontrar las palabras más frecuentes y relevantes, buscar libros por palabra y mostrar apariciones de palabras dentro del contexto de un libro específico, por lo tanto, el objetivo final es crear una herramienta eficaz para la gestión y exploración de una colección de libros digitales.

**Compilar y ejecutar:**
https://github.com/JOAQUINCASTRO/Tarea-3
cd gestor-de-tareas
gcc main.c funciones_tareas.c -o gestor
./gestor

-----------------------------------------------------------------   
**Funcionalidades:**

  **Funcionando correctamente:**
      - "menu": Esta funcion se encarga de mostrar pantalla el menu al usuario para que este trabaje con el.
      
      - "cargarDocumento": Esta funcion se encarga de cargar a la aplicacion una carpeta llena de documentos, los cuales contienen libros para que el usuario trabaje con ellos segun le apetezca.
      
      - "MostrarDocOrdenados" Esta funcion se encarga de mostrar por pantalla todos los documentos ordenados por titulos, tambien muestra por pantalla las id´s de los documentos asi como tambien la cantidad de palabras que contienen cada documentos y la cantidad de caracteres que tienen los documentos.
      
      - "buscarLibroXtitulo": Esta funcion se encarga de mostrar por pantalla todos los titulos de los libros que contengan la palabra que el usuario ingreso. Se debe ingresar la palabra en minuscula sin tilde.
      
      - "palabrasConMayorFrecuencia": Esta funcion se encarga de mostrar las 10 primeras palabras que mas se repitan en un libro, el cual depende del titulo que ingreso el usuario, tambien se muestra por pantalla la cantidad de veces que se repiten cada palabra.
      
      - "palabrasMasRelevantes": Esta funcion se encarga de mostrar por pantalla las 10 palabras mas relevantes del libro ingresado por el usuario, esto se hace aplicando una formula, la cual define la relevancia de las palabras.
      
      - "buscarXpalabra": Esta funcion se encarga de mostrar por pantalla los libros y las id´s de estos los cuales contengan la palabra ingresado por el usuario, los libros se muestran ordenados dependiendo de la relevancia que tenga la palabra ingresada por el usuario en el libro.
      
      -"mostrarPalabraContexto": Esta funcion se encarga de mostrar por pantalla el contexto de una palabra buscada, es decir, el usuario ingresa un titulo y una palabra a buscar y la funcion muestra por pantalla las 5 anteriores y 5 siguientes palabras de la palabra ingresada por el usuario, cada vez que se encuentre la palabra en el documento.
      
  **Detalles:**
      -En la funcion 7 no logramos que se mostrara por pantalla el contexto de la palabra buscada.Lo dejamos como comentada la funcion para no generar problemas.
      -En la funcion 1 no creamos una condicion para que el usuario ingrese solo un texto, en cambio cargamos todo el contenido.
      -En la funcion 5 no pudimos buscarlo por el titulo, asi que lo buscamos por el ID para que asi pueda ejecutarse la funcion.
      
------------------------------------------------------------------
**Aportes:** 

  **Javier Morales**
      - Hizo la funcion "cargarDocumentos"
      - Hizo la funcion "buscarLibroXtitulo"
      - Hizo la funcion "palabrasConMayorFrecuencia"
      - **Auto-evaluación**: 3 (Aporte excelente).

  **Matias Ruiz**
      - Ayudo con la logica general en toda la aplicacion.
      - Hizo la funcion "mostrarDocOrdenados"
      - Hizo la funcion "palabrasMasRelevantes"
      - **Auto-evaluación**: 3 (Aporte excelente).
      
  **Joaquin Castro**
      - Hizo la funcion "menu"
      - Hizo la funcion "buscarXpalabra"
      - Hizo la funcion "mostrarPalabraContexto"
      - **Auto-evaluación**: 3 (Aporte excelente).