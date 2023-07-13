#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define MAX_BUFFER_SIZE 1024

void replaceSequence(FILE* inputFile, FILE* outputFile, const char* sourceSequence, const char* replacementSequence);

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: %s <input_file> <output_file> <source_sequence> <replacement_sequence>\n", argv[0]);
        return 1;
    }

    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    const char* sourceSequence = argv[3];
    const char* replacementSequence = argv[4];

    FILE* input = fopen(inputFile, "rb");
    if (!input) {
        printf("Failed to open input file: %s\n", inputFile);
        return 2;
    }

    FILE* output = fopen(outputFile, "wb");
    if (!output) {
        printf("Failed to open output file: %s\n", outputFile);
        fclose(input);
        return 2;
    }

    replaceSequence(input, output, sourceSequence, replacementSequence);

    fclose(input);
    fclose(output);

    printf("Replacement completed!\n");

    return 0;
}

void replaceSequence(FILE* inputFile, FILE* outputFile, const char* sourceSequence, const char* replacementSequence) {
    char buffer[MAX_BUFFER_SIZE];
    int bytesRead;
    int sourceLength = strlen(sourceSequence);
    int replacementLength = strlen(replacementSequence);

    while ((bytesRead = fread(buffer, 1, BLOCK_SIZE, inputFile)) > 0) {
        char* sourcePtr = buffer;
        char* endPtr = buffer + bytesRead;

        while ((sourcePtr = memchr(sourcePtr, sourceSequence[0], endPtr - sourcePtr))) {
            if (endPtr - sourcePtr >= sourceLength && memcmp(sourcePtr, sourceSequence, sourceLength) == 0) {
                fwrite(buffer, 1, sourcePtr - buffer, outputFile);
                fwrite(replacementSequence, 1, replacementLength, outputFile);
                sourcePtr += sourceLength;
                buffer[0] = '0';
            }
            else {
                break;
            }
        }

        fwrite(sourcePtr, 1, endPtr - sourcePtr, outputFile);
    }
}
