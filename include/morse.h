/**
 * @file morse.h
 * @brief Schnittstelle für Morse-Encode und Decode
 */

#ifndef MORSE_H
#define MORSE_H


/**
 * @brief Encodiert einen Text in Morsecode.
 * 
 * @param input Eingabetext (UTF-8)
 * @param out Zielstream (stdout oder Datei)
 * @param slash_mode Ob der Slash-Worttrenner aktiviert ist
 */
void encode_text(const char* input, FILE* out, int slash_mode);

/**
 * @brief Decodiert Morsecode in Text.
 * 
 * @param input Eingabe-Morsecode (UTF-8)
 * @param out Zielstream (stdout oder Datei)
 */
void decode_morse(const char* input, FILE* out);

/**
 * @brief Gibt die Programmhilfe aus.
 */
void print_help();

/**
 * @brief Gibt die Autoreninformationen im JSON-Format aus.
 */
void print_programmer_info();

#endif // MORSE_H