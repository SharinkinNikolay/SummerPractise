#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Функция для генерации случайного символа из заданного алфавита
char getRandomChar(char* alphabet, int length) {
    int index = rand() % length;
    return alphabet[index];
}

// Функция для генерации случайного пароля заданной длины
void generatePassword(char* alphabet, int alphabetLength, int passwordLength) {
    for (int i = 0; i < passwordLength; i++) {
        printf("%c", getRandomChar(alphabet, alphabetLength));
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    int minPasswordLength = 0;
    int maxPasswordLength = 0;
    int passwordLength = 0;
    int passwordCount = 1;
    char* alphabet = NULL;
    int alphabetLength = 0;
    char* characterSet = NULL;
    int characterSetLength = 0;

    // Обработка аргументов командной строки
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-m1") == 0) {
            minPasswordLength = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-m2") == 0) {
            maxPasswordLength = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-n") == 0) {
            passwordLength = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-c") == 0) {
            passwordCount = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-a") == 0) {
            alphabet = argv[i + 1];
            alphabetLength = strlen(alphabet);
        }
        else if (strcmp(argv[i], "-C") == 0) {
            characterSet = argv[i + 1];
            characterSetLength = strlen(characterSet);
        }
    }

    // Проверка корректности опций и аргументов
    if (minPasswordLength > maxPasswordLength) {
        printf("Ошибка: минимальная длина пароля больше максимальной.\n");
        return 1;
    }

    if (minPasswordLength > 0 && maxPasswordLength > 0 && passwordLength > 0) {
        printf("Ошибка: опции -m1, -m2 и -n несовместимы вместе.\n");
        return 1;
    }

    if ((alphabet != NULL && characterSet != NULL) || (alphabet == NULL && characterSet == NULL)) {
        printf("Ошибка: опции -a и -C несовместимы вместе.\n");
        return 1;
    }

    if (alphabet == NULL) {
        alphabet = "";
        alphabetLength = 0;

        if (characterSet != NULL) {
            // Формирование алфавита символов на основе опции -C
            for (int i = 0; i < characterSetLength; i++) {
                switch (characterSet[i]) {
                case 'a':
                    alphabet = strcat(alphabet, "abcdefghijklmnopqrstuvwxyz");
                    alphabetLength += 26;
                    break;
                case 'A':
                    alphabet = strcat(alphabet, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
                    alphabetLength += 26;
                    break;
                case 'D':
                    alphabet = strcat(alphabet, "0123456789");
                    alphabetLength += 10;
                    break;
                }
            }
        }
    }

    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Генерация паролей
    for (int i = 0; i < passwordCount; i++) {
        int currentPasswordLength = passwordLength;
        if (minPasswordLength > 0 && maxPasswordLength > 0) {
            currentPasswordLength = minPasswordLength + (rand() % (maxPasswordLength - minPasswordLength + 1));
        }

        generatePassword(alphabet, alphabetLength, currentPasswordLength);
    }

    return 0;
}
