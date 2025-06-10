#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "morse.h"

int main(int argc, char *argv[]) {
    int encode = 1, decode = 0, slash_mode = 0;
    char *outfile = NULL;

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
                break;
            case 'd':
                encode = 0;
                decode = 1;
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

    if (encode && decode) {
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

    // Eingabe lesen
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
