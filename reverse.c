/*INTEGRANTES:
    - Oswald Gutiérrez
    - Santiago Rivera
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define RAISE_ERROR(message)          \
    do {                             \
        fprintf(stderr, "%s\n", message); \
        exit(1);                      \
    } while (0)

#define capacidadInicial 2

void verificarNombreArchivos(const char *archivoEntrada, const char *archivoSalidad);
void abrirArchivos(const char *archivoEntrada, const char *archivoSalidad, FILE **archivoEntradaPtr, FILE **archivoSalidaPtr);
void leerLineas(FILE *archivoEntrada, char ***lineas, size_t *cantidadLineas, size_t *capacidadLineas);
void escribirLineasReversa(FILE *archivoSalidad, char **lineas, size_t cantidadLineas);
void limpiar(char **lineas, size_t cantidadLineas, FILE *archivoEntrada, FILE *archivoSalidad);

int main(int argc, char *argv[])
{
    FILE *archivoEntrada;
    FILE *archivoSalidad;
    char **lineas = NULL;
    size_t cantidadLineas = 0;
    size_t capacidadLineas = capacidadInicial;

    if (argc < 2 || argc > 3)
    {
        RAISE_ERROR("usage: reverse <input> <output>");
    }

    const char *archivoEntradaNombre = argv[1];
    const char *archivoSalidaNombre = (argc > 2) ? argv[2] : NULL;

    verificarNombreArchivos(archivoEntradaNombre, archivoSalidaNombre);
    abrirArchivos(archivoEntradaNombre, archivoSalidaNombre, &archivoEntrada, &archivoSalidad);

    lineas = (char **)malloc(capacidadLineas * sizeof(char *));
    if (lineas == NULL)
    {
        RAISE_ERROR("reverse: malloc failed");
    }

    leerLineas(archivoEntrada, &lineas, &cantidadLineas, &capacidadLineas);
    escribirLineasReversa(archivoSalidad, lineas, cantidadLineas);
    limpiar(lineas, cantidadLineas, archivoEntrada, archivoSalidad);

    return 0;
}

// Verifica que los archivos de entrada y salida sean diferentes
void verificarNombreArchivos(const char *archivoEntrada, const char *archivoSalidad)
{
    if (archivoSalidad != NULL && strcmp(archivoEntrada, archivoSalidad) == 0)
    {
        RAISE_ERROR("reverse: input and output file must differ");
    }

    if (archivoSalidad != NULL)
    {
        struct stat stat_entrada, stat_salida;
        if (stat(archivoEntrada, &stat_entrada) == 0 && stat(archivoSalidad, &stat_salida) == 0)
        {
            if (stat_entrada.st_ino == stat_salida.st_ino)
            {
                RAISE_ERROR("reverse: input and output file must differ");
            }
        }
    }
}

// Abre los archivos de entrada y salida
void abrirArchivos(const char *archivoEntrada, const char *archivoSalidad, FILE **archivoEntradaPtr, FILE **archivoSalidaPtr)
{
    if (archivoEntrada != NULL)
    {
        *archivoEntradaPtr = fopen(archivoEntrada, "r");
        if (*archivoEntradaPtr == NULL)
        {
            fprintf(stderr, "reverse: cannot open file '%s'\n", archivoEntrada);
            exit(1);
        }
    }
    else
    {
        *archivoEntradaPtr = stdin;
    }

    if (archivoSalidad != NULL)
    {
        *archivoSalidaPtr = fopen(archivoSalidad, "w");
        if (*archivoSalidaPtr == NULL)
        {
            fprintf(stderr, "reverse: cannot open file '%s'\n", archivoSalidad);
            exit(1);
        }
    }
    else
    {
        *archivoSalidaPtr = stdout;
    }
}

// Lee las líneas del archivo de entrada
void leerLineas(FILE *archivoEntrada, char ***lineas, size_t *cantidadLineas, size_t *capacidadLineas)
{
    size_t len = 0;
    while (getline(&(*lineas)[*cantidadLineas], &len, archivoEntrada) != -1)
    {
        (*cantidadLineas)++;

        if (*cantidadLineas == *capacidadLineas)
        {
            *capacidadLineas *= 2;
            *lineas = (char **)realloc(*lineas, *capacidadLineas * sizeof(char *));
            if (*lineas == NULL)
            {
                RAISE_ERROR("reverse: realloc failed");
            }
        }
        (*lineas)[*cantidadLineas] = NULL;
        len = 0;
    }
}

// Escribe las líneas en orden inverso en el archivo de salida
void escribirLineasReversa(FILE *archivoSalidad, char **lineas, size_t cantidadLineas)
{
    for (int i = cantidadLineas - 1; i >= 0; i--)
    {
        fprintf(archivoSalidad, "%s", lineas[i]);
    }
}

// Libera la memoria y cierra los archivos
void limpiar(char **lineas, size_t cantidadLineas, FILE *archivoEntrada, FILE *archivoSalidad)
{
    for (size_t i = 0; i < cantidadLineas; i++)
    {
        free(lineas[i]);
    }
    free(lineas);

    if (archivoEntrada != stdin)
        fclose(archivoEntrada);
    if (archivoSalidad != stdout)
        fclose(archivoSalidad);
}
