#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000

void reverse_lines(FILE *input_file, FILE *output_file) {
    char *lines[MAX_LINES];
    char buffer[MAX_LINE_LENGTH];
    int line_count = 0;

    // Leer las líneas del archivo de entrada
    while (fgets(buffer, sizeof(buffer), input_file)) {
        // Verificar si el archivo excede el máximo permitido de líneas
        if (line_count >= MAX_LINES) {
            fprintf(stderr, "Error: Se excedió el número máximo de líneas.\n");
            exit(1);
        }

        // Eliminar el salto de línea al final de cada línea
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        // Asignar memoria para la línea y copiarla
        lines[line_count] = malloc(len + 1);
        if (lines[line_count] == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        strcpy(lines[line_count], buffer);
        line_count++;
    }

    // Escribir las líneas en orden inverso
    for (int i = line_count - 1; i >= 0; i--) {
        fprintf(output_file, "%s\n", lines[i]);
        free(lines[i]); // Liberar memoria
    }
}

int main(int argc, char *argv[]) {
    FILE *input_file = stdin;
    FILE *output_file = stdout; // Por defecto, la salida es la consola

    // Verificar el número de argumentos
    if (argc != 1 && argc != 3) {
        fprintf(stderr, "usage: reverse [input [output]]\n");
        exit(1);
    }

    // Abrir el archivo de entrada
    if (argc == 3 && strcmp(argv[1], argv[2]) == 0) {
        fprintf(stderr, "El archivo de entrada y salida deben diferir\n");
        exit(1);
    }
    if (argc >= 2) {
        input_file = fopen(argv[1], "r");
        if (input_file == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    // Abrir el archivo de salida
    if (argc == 3) {
        output_file = fopen(argv[2], "w");
        if (output_file == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            if (input_file != stdin) {
                fclose(input_file);
            }
            exit(1);
        }
    }

    // Leer, invertir y escribir las líneas
    reverse_lines(input_file, output_file);

    // Cerrar archivos
    if (input_file != stdin) {
        fclose(input_file);
    }
    if (output_file != stdout) {
        fclose(output_file);
    }

    return 0;
}
