#ifndef MORSE_H
#define MORSE_H

void encode_text(const char* input, FILE* out, int slash_mode);
void decode_morse(const char* input, FILE* out);
void print_help();
void print_programmer_info();

#endif // MORSE_H