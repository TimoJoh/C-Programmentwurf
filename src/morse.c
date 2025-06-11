/**
 * @file morse.c
 * @brief Implementierung der Funktionen für das Morsecode-Tool.
 *
 * Dieses Modul enthält die Implementierungen zum Encodieren und Decodieren von
 * Text <-> Morsecode. Zusätzlich werden die Hilfefunktion und die
 * Autoreninformationen bereitgestellt.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "morse.h"

/**
 * @brief Struktur zur Zuordnung von Zeichen zu Morsecode.
 */
typedef struct {
    char character;           /**< Klartext-Zeichen */
    const char* morse;        /**< Entsprechender Morsecode */
} MorseCode;

/**
 * @brief Morsecode-Tabelle für unterstützte Zeichen.
 *
 * Basierend auf DARC-Tabelle: https://dk0ru.github.io/downloads/m21-morse-code.pdf
 */
static const MorseCode morse_table[] = {
    // Buchstaben
    {'A', ".-"},    {'B', "-..."},  {'C', "-.-."},
    {'D', "-.."},   {'E', "."},     {'F', "..-."},
    {'G', "--."},   {'H', "...."},  {'I', ".."},
    {'J', ".---"},  {'K', "-.-"},   {'L', ".-.."},
    {'M', "--"},    {'N', "-."},    {'O', "---"},
    {'P', ".--."},  {'Q', "--.-"},  {'R', ".-."},
    {'S', "..."},   {'T', "-"},     {'U', "..-"},
    {'V', "...-"},  {'W', ".--"},   {'X', "-..-"},
    {'Y', "-.--"},  {'Z', "--.."},
    // Ziffern
    {'0', "-----"}, {'1', ".----"}, {'2', "..---"},
    {'3', "...--"}, {'4', "....-"}, {'5', "....."},
    {'6', "-...."}, {'7', "--..."}, {'8', "---.."},
    {'9', "----."},
    // Sonderzeichen
    {'.', ".-.-.-"},{',', "--..--"},{':', "---..."},
    {';', "-.-.-."},{'?', "..--.."},
    {'=', "-...-"},{'-', "-....-"},{'_', "..--.-"},
    {'(', "-.--."}, {')', "-.--.-"}, {'+', ".-.-."},
    {'/', "-..-."}, {'@', ".--.-."}
};

/**
 * @brief Encodiert einen Text in Morsecode.
 *
 * Jeder Buchstabe wird durch einen SP (SPACE) getrennt.
 * Wörter werden durch drei SPACES oder alternativ ' / ' getrennt.
 * Unbekannte Zeichen werden als '*' dargestellt.
 *
 * @param input Eingabetext (UTF-8).
 * @param out Zielstream (stdout oder Datei).
 * @param slash_mode Aktiviert optional den Slash-Worttrenner (--slash-wordspacer).
 */
void encode_text(const char* input, FILE* out, int slash_mode) {
    for (const char* p = input; *p; ++p) {
        if (*p == '\n' || *p == '\r') {
            continue; 
        }
        if (*p == ' ') {
            fputs(slash_mode ? " / " : "   ", out);
            continue;
        }
        char upper = toupper((unsigned char)*p);
        int found = 0;
        for (size_t i = 0; i < sizeof(morse_table) / sizeof(MorseCode); ++i) {
            if (morse_table[i].character == upper) {
                fprintf(out, "%s ", morse_table[i].morse);
                found = 1;
                break;
            }
        }
        if (!found) fputs("* ", out);    
    }
    fputc('\n', out);
}

/**
 * @brief Dekodiert Morsecode in Klartext.
 *
 * Morsezeichen werden durch SPACES getrennt.
 * Wörter werden durch drei SPACES getrennt.
 * Unbekannte Morsezeichen werden als '*' dargestellt.
 *
 * @param input Eingabemorsecode (ASCII).
 * @param out Zielstream (stdout oder Datei).
 */
void decode_morse(const char* input, FILE* out) {
    char buffer[16];
    size_t buf_index = 0;
    while (*input) {
        if (*input == ' ' || *input == '\n' || *input == '\r') {
            buffer[buf_index] = '\0';
            if (buf_index > 0) {
                int found = 0;
                for (size_t i = 0; i < sizeof(morse_table) / sizeof(MorseCode); ++i) {
                    if (strcmp(morse_table[i].morse, buffer) == 0) {
                        fputc(morse_table[i].character, out);
                        found = 1;
                        break;
                    }
                }
                if (!found) fputc('*', out);
                buf_index = 0;
            } else if (*(input + 1) == ' ' && *(input + 2) == ' ') {
                fputc(' ', out);
                input += 2;
            }
        } else {
            buffer[buf_index++] = *input;
        }
        ++input;
    }
    fputc('\n', out);
}

/**
 * @brief Gibt die Programmhilfe auf stdout aus.
 *
 * Zeigt alle unterstützten Kommandozeilenparameter und deren Verwendung an.
 */
void print_help() {
    puts("Benutzung: ./morse [OPTION]... [DATEI]...\n"
         "  -e, --encode            Text in Morsecode umwandeln\n"
         "  -d, --decode            Morsecode in Text umwandeln\n"
         "  -o, --out DATEI         Ausgabe in DATEI schreiben\n"
         "  --slash-wordspacer      Worttrenner als ' / ' statt dreifach SPACE\n"
         "  -h, --help              Diese Hilfe anzeigen\n"
         "  --programmer-info       JSON-Ausgabe mit Autorinformationen");
}

/**
 * @brief Gibt die Autoreninformationen im JSON-Format auf stdout aus.
 *
 * Wird durch den Kommandozeilenparameter --programmer-info aktiviert.
 */
void print_programmer_info() {
    puts("{\n"
         "  \"firstname\": \"Timo\",\n"
         "  \"surname\": \"Johannsen\",\n"
         "  \"branch_of_study\": \"TIK\",\n"
         "  \"contact\": \"johannsen.timo-it24@it.dhbw-ravensburg.de\"\n"
         "}");
}
