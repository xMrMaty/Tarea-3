#include "hashmap.h"
#include "heap.h"
#include "list.h"
#include "treemap.h"
#include "math.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
//////////////////////////////////////////////////////////////////////////////////
//ESTRUCTURAS
typedef struct Contexto {
  char palabrasAnteriores[100];
  char palabrasPosteriores[100];
} Contexto;

typedef struct Palabra {
  char palabra[100];               // La palabra en sí
  int contador;                   // Contador de ocurrencias
  double relevancia;              // Relevancia
  List *contextoPalabra;          // Contexto de la palabra (lista de Contexto)
  List *libros;                   // Lista de libros que contienen esta palabra
  char tituloLibro[256];
} Palabra;

typedef struct Libro {
  char id[256];                   // Identificador
  char titulo[256];               // Título del libro
  int contPalabras;               // Número de palabras del texto
  int contCaracteres;             // Número de caracteres del texto
  HashMap *mapa_contenido;        // Mapa de contenido (HashMap de Palabra)
  List *frec_palabra_contenido;
} Libro;
//////////////////////////////////////////////////////////////////////
//FUNCIONES VARIAS

void leerCadena101(char cadena[101]) {
  scanf("%101[^\n]s", cadena);
  getchar();
}
//--------------------------------------------------------------------
int lower_than_string(void* key1, void* key2){
  char* k1=(char*) key1;
  char* k2=(char*) key2;
  if(strcmp(k1,k2)<0) return 1;
  return 0;
}
//--------------------------------------------------------------------
char* next_word (FILE *f) {
  char x[1024];
  /* assumes no word exceeds length of 1023 */
  if (fscanf(f, " %1023s", x) == 1)
    return strdup(x);
  else
    return NULL;
}
//--------------------------------------------------------------------
void menu() {
  printf("\n\033[1;34m=================================\n");
  printf("         MENÚ BIBLIOTECA      \n");
  printf("=================================\n");
  printf("\033[0m\033[1;37m1 \033[0mCargar documentos.\n");
  printf("\033[0m\033[1;37m2 \033[0mMostrar documentos ordenados.\n");
  printf("\033[0m\033[1;37m3 \033[0mBuscar un libro por título.\n");
  printf("\033[0m\033[1;37m4 \033[0mPalabras con mayor frecuencia.\n");
  printf("\033[0m\033[1;37m5 \033[0mPalabras más relevantes.\n");
  printf("\033[0m\033[1;37m6 \033[0mBuscar por palabra.\n");
  printf("\033[0m\033[1;37m7 \033[0mMostrar palabra en su contexto.\n");
  printf("\033[0m\033[1;31m0 \033[0mSALIR\n");
  printf("=================================\n");
  printf("Ingrese una opción: ");
}

//////////////////////////////////////////////////////////////////////
//FUNCIONES MENÚ
/*
Esta funcion se encarga de limpiar las palabras de caracteres no deseados que dificulten el trabajo con 
ellas y ademas las transforma a minusculas.
*/
void limpiarYConvertirAMinusculas(char *palabra) {
  char palabraLimpia[1024];
  int j = 0;
  
  for (int i = 0; palabra[i] != '\0'; i++) {
    if (isalpha(palabra[i])) {
      palabraLimpia[j] = tolower(palabra[i]);
      j++;
    }
  }
  palabraLimpia[j] = '\0';
  strcpy(palabra, palabraLimpia);
}
/*
Esta funcion se encarga de abrir una carpeta de archivos para despues procesar cada archivo, almacena los titulos y el contenido de cada archivo en palabras.
*/
void cargarDocumentos(TreeMap *arbolLibros, TreeMap *arbolLibroID){
  
  getchar();
  char carpeta[1024];
  int numCaracteresLibro = 0;
  printf("\nIngrese la ruta de la carpeta que contiene los archivos: ");
  fgets(carpeta, sizeof(carpeta), stdin);
  strtok(carpeta, "\n");

  DIR *dir;
  struct dirent *entry;

  // Lista de palabras en inglés a omitir
  char *palabrasEnIngles[]={
  
  "a", "an", "the", "in", "on", "at", "to", "of", "and", "or", "with", "without",
  "i", "you", "he", "she", "it", "we", "they",
  "am", "is", "are", "was", "were",
  "have", "has", "had",
  "do", "does", "did",
  "will", "shall",
  "can", "could",
  "may", "might",
  "must",
  "am", "is", "are",
  "was", "were",
  "be", "being", "been",
  "have", "has", "had",
  "do", "does", "did",
  "will", "shall", "would", "should",
  "can", "could",
  "may", "might",
  "must",
  "if", "then", "else", "while", "for", "foreach", "continue", "break", "return", "function"  
  };
  
  if ((dir = opendir(carpeta)) != NULL){
    
    printf("Cargando documentos...\n");
    while ((entry = readdir(dir)) != NULL){
      if (entry->d_type == DT_REG){
        char nombreArchivo[1024];
        snprintf(nombreArchivo, sizeof(nombreArchivo), "%s/%s", carpeta, entry->d_name);
        Libro *libro = (Libro *)malloc(sizeof(Libro));
        strcpy(libro->id, entry->d_name);
        libro->mapa_contenido = createMap(1000); 
        libro->contPalabras = 0;
        numCaracteresLibro = 0; 
        FILE *archivo = fopen(nombreArchivo, "r");
        if (archivo){
          
          fgets(libro->titulo, sizeof(libro->titulo), archivo);
          strtok(libro->titulo, "\n");
          
          char *palabra;
          char *palabraAnterior = NULL;
          Palabra *nuevaPalabra = NULL;
          
          while ((palabra = next_word(archivo)) != NULL){
            
            libro->contPalabras++;
            numCaracteresLibro += strlen(palabra); 

            limpiarYConvertirAMinusculas(palabra);

            int esPalabraEnIngles = 0;
            for (int i = 0; i < sizeof(palabrasEnIngles) / sizeof(palabrasEnIngles[0]); i++){
              
              if (strcmp(palabrasEnIngles[i], palabra) == 0){
                
                esPalabraEnIngles = 1;
                break;
              }
            }

            if (!esPalabraEnIngles){
              
              Pair *result = searchMap(libro->mapa_contenido, palabra);
              if (result == NULL){
                
              
                Palabra *nuevaPalabra = (Palabra *)malloc(sizeof(Palabra));
              
                strcpy(nuevaPalabra->palabra, palabra);
                nuevaPalabra->contador = 1;
                nuevaPalabra->contextoPalabra = createList();
                nuevaPalabra->libros = createList();

                insertMap(libro->mapa_contenido, nuevaPalabra->palabra, nuevaPalabra);

                pushBack(nuevaPalabra->libros, libro);
                } else{
                
              
                Palabra *palabraExistente = (Palabra *)result->value;
                palabraExistente->contador++;
                }
              
              if (palabraAnterior != NULL){
                
              }
            }
            palabraAnterior = palabra;

            free(palabra); 
          }
          fclose(archivo);
        } else {
          
          printf("No se pudo abrir el archivo: %s\n", nombreArchivo);
          free(libro);
          continue;
        }
        insertTreeMap(arbolLibros, libro->titulo, libro);
        insertTreeMap(arbolLibroID, libro->id, libro);
      }
    }
        printf("Archivos guardados exitosamente.\n");
        closedir(dir);
  } else{
    
     printf("No se pudo abrir la carpeta: %s\n", carpeta);
  }
}

//--------------------------------------------------------------------
/*
Esta funcion se encarga de mostrar por pantalla los titulos de los todos los libros almacenados en la 
carpeta de archivos, junto con su id, el numero de palabras y el numero de caracteres que tiene cada
libro.
*/
void mostrarDocOrdenados(TreeMap *arbolLibros) {
  Pair *pair = firstTreeMap(arbolLibros);
  if (pair == NULL) {
    printf("No hay libros cargados en la biblioteca.\n");
    return;
  }
  printf("Documentos ordenados por título:\n");
  while (pair != NULL){
    Libro *libro = (Libro *)pair->value;
    printf("ID: %s\n", libro->id);
    printf("Título: %s\n", libro->titulo);
    printf("Cantidad de palabras: %d\n", libro->contPalabras);
    printf("Cantidad de caracteres: %d\n\n", libro->contCaracteres);
    pair = nextTreeMap(arbolLibros);
  }
}

//--------------------------------------------------------------------
/*
Esta funcion muestra por pantalla los titulos de los libros que contengan la palabra proporcionada por el
usuario.
*/
void buscarLibroXtitulo(TreeMap *arbolLibros) {
  getchar();
  printf("Ingrese palabras clave separadas por espacios y en minusculas: ");
  char entrada[1024];
  leerCadena101(entrada);
  
  char palabrasClave[100][256];
  int numPalabrasClave = 0;
  char *token = strtok(entrada, " ");
  while (token != NULL && numPalabrasClave < 100) {
    strcpy(palabrasClave[numPalabrasClave], token);
    numPalabrasClave++;
    token = strtok(NULL, " ");
  }
  
  Pair *pair = firstTreeMap(arbolLibros);
  int encontrados = 0;
  printf("Resultados de la búsqueda:\n");
  
  while (pair != NULL) {
    Libro *libro = (Libro *)pair->value;
    int todasLasPalabrasClaveEncontradas = 1;
    
    for (int i = 0; i < numPalabrasClave; i++) {
      char *lowerTitulo = strdup(libro->titulo);
      limpiarYConvertirAMinusculas(lowerTitulo);
      if (strstr(lowerTitulo, palabrasClave[i]) == NULL) {
        todasLasPalabrasClaveEncontradas = 0;
        break;
      }
    }

    if (todasLasPalabrasClaveEncontradas) {
      printf("Título: %s\n", libro->titulo);
      encontrados++;
    }
    pair = nextTreeMap(arbolLibros);
  }
  if (encontrados == 0) {
    printf("No se encontraron libros que coincidan con la búsqueda.\n");
  }
}

//--------------------------------------------------------------------
/*
Esta funcion se encarga de ordenar las palabras segun su frecuencia.
*/
int compararPorFrecuencia(const void *a, const void *b){
  const Palabra *palabraA = *(const Palabra **)a;
  const Palabra *palabraB = *(const Palabra **)b;
  return palabraB->contador - palabraA->contador; // Orden descendente
}
/*
Esta funcion se encarga especificamente de mostrar por pantalla las palabras y la frecuencia que tienen
en el libro con la id entregada por el usuario.
*/
void imprimirPalabrasContador(HashMap *mapaPalabras){
  // Crear un arreglo de punteros a Palabra para almacenar las palabras del mapa
  int numPalabras = size(mapaPalabras);
  Palabra **palabrasArray = (Palabra **)malloc(numPalabras * sizeof(Palabra *));

  Pair *pair = firstMap(mapaPalabras);
  int i = 0;
  while (pair != NULL){
    palabrasArray[i] = (Palabra *)pair->value;
    pair = nextMap(mapaPalabras);
    i++;
  }

  // Ordenar el arreglo de palabras por frecuencia
  qsort(palabrasArray, numPalabras, sizeof(Palabra *), compararPorFrecuencia);

  // Mostrar las primeras 10 palabras con mayor frecuencia
  int numPalabrasAMostrar = (numPalabras < 10) ? numPalabras : 10;
  for (i = 0; i < numPalabrasAMostrar; i++){
    printf("Palabra: %s, Frecuencia: %d\n", palabrasArray[i]->palabra, palabrasArray[i]->contador);
  }

  // Liberar la memoria del arreglo
  free(palabrasArray);
}
/*
Esta funcion se encarga de ordenar las palabras segun su frecuencia, dependiendo de la id del libro 
entregada por el usuario.
*/
void palabrasConMayorFrecuencia(TreeMap *arbolLibroID){
  getchar(); // Limpiar el búfer de entrada
  printf("Ingrese el ID del libro: ");
  char idBusqueda[256];
  leerCadena101(idBusqueda);
  
  Pair *pair = searchTreeMap(arbolLibroID, idBusqueda);
  if (pair != NULL){
    Libro *libroBuscado = (Libro *)pair->value;
    printf("Palabras con mayor frecuencia para el libro %s:\n", libroBuscado->titulo);
    imprimirPalabrasContador(libroBuscado->mapa_contenido);
  }else{
    printf("No se encontró ningún libro con el ID especificado.\n");
  }
}

//--------------------------------------------------------------------
/*
Esta funcion se encarga de ordenar y mostrar por pantalla las palabras segun su relevancia, esto se 
define mediante una formula que se aplica al libro que coincida con el titulo que nos brinda el usuario. 
*/
void palabrasMasRelevantes(TreeMap *arbolLibros){
  
  getchar();

  printf("Ingrese el título del libro: ");
  char tituloBusqueda[256];
  leerCadena101(tituloBusqueda);

  Pair *pair = searchTreeMap(arbolLibros, tituloBusqueda);

  if (pair == NULL){
    
    printf("No se encontró ningún libro con el título especificado.\n");
  } else{
    
    Libro *libro = (Libro *)pair->value;
    HashMap *mapaContenido = libro->mapa_contenido;

    int numDocumentos = sizeTreeMap(arbolLibros);
    Palabra palabrasRelevantes[10];
    Pair *pairPalabra = firstMap(mapaContenido);
        
    while (pairPalabra != NULL){
      Palabra *palabra = (Palabra *)pairPalabra->value;
      int numDocumentosConPalabra = sizeList(palabra->libros);
      double relevancia = ((double)palabra->contador / libro->contPalabras) * log((double)numDocumentos / numDocumentosConPalabra); 

      for (int i = 0; i < 10; i++){
        
        if (relevancia > palabrasRelevantes[i].relevancia){
          
          for (int j = 9; j > i; j--){
            
            strcpy(palabrasRelevantes[j].palabra, palabrasRelevantes[j - 1].palabra);
            palabrasRelevantes[j].relevancia = palabrasRelevantes[j - 1].relevancia;
          }
          strcpy(palabrasRelevantes[i].palabra, palabra->palabra);
          palabrasRelevantes[i].relevancia = relevancia;
          break;
        }
      }
      pairPalabra = nextMap(mapaContenido);
    }
    printf("Las 10 palabras más relevantes en el libro \"%s\" son:\n", libro->titulo);
    for (int i = 0; i < 10; i++){
      printf("%d. %s\n", i + 1, palabrasRelevantes[i].palabra);
    }
  }
}

//--------------------------------------------------------------------
/*

*/
void calcularNumDocumentos(TreeMap *arbolLibros, char *palabraBusqueda, int *numDocumentos, int *numDocumentosConPalabra) {
  *numDocumentos = 0;
  *numDocumentosConPalabra = 0;
  Pair *pair = firstTreeMap(arbolLibros);
  while (pair != NULL) {
    (*numDocumentos)++;
    Libro *libro = (Libro *)pair->value;
    HashMap *mapaContenido = libro->mapa_contenido;
    Pair *result = searchMap(mapaContenido, palabraBusqueda);
    if (result != NULL) {
      (*numDocumentosConPalabra)++;
    }
    pair = nextTreeMap(arbolLibros);
  }
}

void buscarXpalabra(TreeMap *arbolLibros) {
  getchar();

  printf("Ingrese la palabra que desea buscar: ");
  char palabraBusqueda[100];
  leerCadena101(palabraBusqueda);

  int numDocumentos = 0;
  int numDocumentosConPalabra = 0;
  calcularNumDocumentos(arbolLibros, palabraBusqueda, &numDocumentos, &numDocumentosConPalabra);


  Pair *pair = firstTreeMap(arbolLibros);
  int resultadosEncontrados = 0;
  printf("Libros que contienen la palabra \"%s\":\n", palabraBusqueda);


  Palabra palabrasEncontradas[100];
  int numPalabras = 0;

  while (pair != NULL) {
    Libro *libro = (Libro *)pair->value;
    HashMap *mapaContenido = libro->mapa_contenido;
    Pair *result = searchMap(mapaContenido, palabraBusqueda);
    
    if (result != NULL) {
      Palabra *palabra = (Palabra *)result->value;
      double relevancia = ((double)palabra->contador / libro->contPalabras) * log((double)numDocumentos / numDocumentosConPalabra);
      
      strcpy(palabrasEncontradas[numPalabras].palabra, palabra->palabra);
      strcpy(palabrasEncontradas[numPalabras].tituloLibro, libro->titulo);
      palabrasEncontradas[numPalabras].relevancia = relevancia;
      numPalabras++;
    }
    
    pair = nextTreeMap(arbolLibros);
  }
  
  // Ordena las palabras por relevancia en orden descendente
  for (int i = 0; i < numPalabras - 1; i++) {
    for (int j = 0; j < numPalabras - i - 1; j++) {
      if (palabrasEncontradas[j].relevancia < palabrasEncontradas[j + 1].relevancia) {
        Palabra temp = palabrasEncontradas[j];
        palabrasEncontradas[j] = palabrasEncontradas[j + 1];
        palabrasEncontradas[j + 1] = temp;
      }
    }
  }
  
  for (int i = 0; i < numPalabras; i++) {
    printf("Titulo: %s\nPalabra: %s\nRelevancia: %lf\n\n", palabrasEncontradas[i].tituloLibro, palabrasEncontradas[i].palabra, palabrasEncontradas[i].relevancia);
    resultadosEncontrados++;
  }
  
  if (resultadosEncontrados == 0) {
    printf("No se encontraron libros que contengan la palabra \"%s\".\n", palabraBusqueda);
  }
}

//--------------------------------------------------------------------
/*
void mostrarPalabraContexto(TreeMap *arbolLibros) {
    getchar();
    char tituloLibro[256];
    char palabraBuscada[100];
    printf("Ingrese el título del libro: ");
    leerCadena101(tituloLibro);
    printf("Ingrese la palabra que desea buscar en el contexto: ");
    leerCadena101(palabraBuscada);

    // Busca el libro en el árbol de libros
    Pair *result = searchTreeMap(arbolLibros, tituloLibro);

    if (result == NULL) {
        printf("El libro con el título \"%s\" no se encontró en la biblioteca.\n", tituloLibro);
        return;
    }

    Libro *libro = (Libro *)result->value;

    // Busca la palabra en el mapa_contenido del libro
    result = searchMap(libro->mapa_contenido, palabraBuscada);

    if (result == NULL) {
        printf("La palabra \"%s\" no se encontró en el libro \"%s\".\n", palabraBuscada, libro->titulo);
        return;
    }

    Palabra *palabra = (Palabra *)result->value;

    printf("Apariciones de la palabra \"%s\" en el libro \"%s\" con su contexto:\n", palabraBuscada, libro->titulo);

    // Abre el archivo del libro para buscar las apariciones
    FILE *archivo = fopen(libro->id, "r");

    if (archivo) {
        char linea[1024];
        int numeroLinea = 1;

        while (fgets(linea, sizeof(linea), archivo)) {
            char *token = strtok(linea, " ");
            int posicion = 0;

            while (token != NULL) {
                limpiarYConvertirAMinusculas(token);

                if (strcmp(token, palabraBuscada) == 0) {
                    printf("Aparición en la línea %d: ", numeroLinea);

                    // Muestra el contexto de la palabra
                    for (int i = posicion - 5; i <= posicion + 5; i++) {
                        if (i >= 0) {
                            printf("%s ", token);
                        }
                        token = strtok(NULL, " ");
                    }

                    printf("\n");
                }

                token = strtok(NULL, " ");
                posicion++;
            }

            numeroLinea++;
        }

        fclose(archivo);
    }
}
*/
//////////////////////////////////////////////////////////////////////
//MENÚ
int main() {
  TreeMap *arbolLibro = createTreeMap(lower_than_string);
  TreeMap *arbolLibroID = createTreeMap(lower_than_string);
  
  int opcion;
  while (true) {
    menu();
    scanf("%d", &opcion);
    switch (opcion) {
    case 1:
      cargarDocumentos(arbolLibro,arbolLibroID);
      break;
    case 2:
      mostrarDocOrdenados(arbolLibro);
      break;
    case 3:
      buscarLibroXtitulo(arbolLibro);
      break;
    case 4:
      palabrasConMayorFrecuencia(arbolLibroID);
      break;
    case 5:
      palabrasMasRelevantes(arbolLibro);
      break;
    case 6:
      buscarXpalabra(arbolLibro);
      break;
    case 7:
      //mostrarPalabraContexto(arbolLibroID);
      break;
    case 0:
      printf("\nSALISTE DEL MENÚ\n");
      return 0;
    default:
      printf("\nIngrese una opción válida\n");
      break;
    }
  }
  return 0;
}