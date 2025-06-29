/**
 * @file main.c
 * @brief Hauptmodul für das Morsecode-Tool.
 *
 * Dieses Modul implementiert das Parsen der Kommandozeilenoptionen und steuert
 * das Encodieren und Decodieren von Morsecode gemäß den Programmrequirements (TI24).
 *
 * Unterstützt:
 * - Text in Morsecode encodieren (--encode / -e)
 * - Morsecode in Text decodieren (--decode / -d)
 * - Ausgabedatei (--out / -o)
 * - Slash-Wordspacer (--slash-wordspacer)
 * - Hilfetext (--help / -h)
 * - Autoreninfo (--programmer-info)
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "morse.h"

/**
 * @brief Hauptfunktion des Programms.
 *
 * Liest die Kommandozeilenparameter aus, verarbeitet die Eingabedaten (Datei oder stdin)
 * und ruft die Funktionen zum Encodieren oder Decodieren von Morsecode auf.
 *
 * @param argc Anzahl der Kommandozeilenargumente.
 * @param argv Array der Kommandozeilenargumente.
 * @return int Exit-Code (0 = Erfolg, 1 = Fehler).
 */
int main(int argc, char *argv[]) {
    int encode = 1, decode = 0, slash_mode = 0;
    int has_e = 0, has_d = 0;
    char *outfile = NULL;

    /** Definition der unterstützten langen Optionen. */
    static struct option long_options[] = {
        {"encode", no_argument, 0, 'e'},
        {"decode", no_argument, 0, 'd'},
        {"out", required_argument, 0, 'o'},
        {"help", no_argument, 0, 'h'},
        {"programmer-info", no_argument, 0, 0},
        {"slash-wordspacer", no_argument, 0, 1},
        {0, 0, 0, 0}
    };

    int option_index = 0, c;

    /** Verarbeitung der Kommandozeilenoptionen. */
    while ((c = getopt_long(argc, argv, "edo:h", long_options, &option_index)) != -1) {
        switch (c) {
            case 0:
                print_programmer_info();
                return 0;
            case 1:
                slash_mode = 1;
                break;
            case 'e':
                encode = 1;
                has_e = 1;
                break;
            case 'd':
                encode = 0;
                decode = 1;
                has_d = 1;
                break;
            case 'o':
                outfile = optarg;
                break;
            case 'h':
                print_help();
                return 0;
            default:
                fprintf(stderr, "Unbekannte Option\n");
                return 1;
        }
    }

    if (has_d && has_e) {
        fprintf(stderr, "Fehler: Encode und Decode gleichzeitig nicht erlaubt\n");
        return 1;
    }

    FILE *out = stdout;
    if (outfile) {
        out = fopen(outfile, "w");
        if (!out) {
            perror("Fehler beim Öffnen der Ausgabedatei");
            return 1;
        }
    }

    /** Eingabe lesen (Datei oder stdin). */
    char input[4096] = {0};
    if (optind < argc) {
        const char *input_filename = argv[optind];
        FILE *infile = fopen(argv[optind], "r");
        if (!infile) {
            perror("Fehler beim Öffnen der Eingabedatei");
            return 1;
        }
        fread(input, 1, sizeof(input)-1, infile);
        fclose(infile);
    } else {
        fgets(input, sizeof(input), stdin);
    }

    /** Verarbeitung der Eingabedaten. */
    if (decode) {
        decode_morse(input, out);
    } else {
        encode_text(input, out, slash_mode);
    }

    if (out != stdout) {
        fclose(out);
    }

    return 0;
}
