#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "morse.h"

// Morsecode Tabelle (verkürzt)
typedef struct {
    char character;
    const char* morse;
} MorseCode;

static const MorseCode morse_table[] = {
    {'A', ".-"},    {'B', "-..."},  {'C', "-.-."},
    {'D', "-.."},   {'E', "."},     {'F', "..-."},
    {'G', "--."},   {'H', "...."},  {'I', ".."},
    {'J', ".---"},  {'K', "-.-"},   {'L', ".-.."},
    {'M', "--"},    {'N', "-."},    {'O', "---"},
    {'P', ".--."},  {'Q', "--.-"},  {'R', ".-."},
    {'S', "..."},   {'T', "-"},     {'U', "..-"},
    {'V', "...-"},  {'W', ".--"},   {'X', "-..-"},
    {'Y', "-.--"},  {'Z', "--.."},
    {'0', "-----"}, {'1', ".----"}, {'2', "..---"},
    {'3', "...--"}, {'4', "....-"}, {'5', "....."},
    {'6', "-...."}, {'7', "--..."}, {'8', "---.."},
    {'9', "----."},
    {'.', ".-.-.-"},{',', "--..--"},{':', "---..."},
    {';', "-.-.-."},{'?', "..--.."},
    {'=', "-...-"},{'-', "-....-"},{'+', ".-.-."},
    {'_', "..--.-"},{'(', "-.--."}, {')', "-.--.-"},
    {'/', "-..-."}, {'@', ".--.-."}
};

void encode_text(const char* input, FILE* out, int slash_mode) {
    for (const char* p = input; *p; ++p) {
        if (*p == ' ' || *p == '\n') {
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

void print_help() {
    puts("Benutzung: morse [OPTION]... [DATEI]...\n"
         "  -e, --encode            Text in Morsecode umwandeln\n"
         "  -d, --decode            Morsecode in Text umwandeln\n"
         "  -o, --out DATEI         Ausgabe in DATEI schreiben\n"
         "  --slash-wordspacer      Worttrenner als ' / ' statt dreifach SPACE\n"
         "  -h, --help              Diese Hilfe anzeigen\n"
         "  --programmer-info       JSON-Ausgabe mit Autorinformationen");
}

void print_programmer_info() {
    puts("{\n"
         "  \"surname\": \"Johannsen\",\n"
         "  \"name\": \"Timo\",\n"
         "  \"branch_of_study\": \"TIK\",\n"
         "  \"contact\": \"johannsen.timo-it24@it.dhbw-ravensburg.de\"\n"
         "}");
}
