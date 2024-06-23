#include <GSPCore/GSPCore.h>
#include <GSPCore/Markup/GMarkupManager.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>

GMarkupParseState parseState = PARSE_STATE_READY;

void ParseCharacter(uint32_t codepoint);

void GMarkupManager_Load(const char* file) {
    FILE* fileHandle = fopen(file, "r");

    if (fileHandle == NULL) {
        GLog(WARNING, "Failed to open file \'%s\'", file);
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fileHandle)) != -1) {

        size_t idx = 0;

        while (idx < read) {
            // Determine UTF-8 character length
            int char_len = 1;
            unsigned char first_byte = line[idx];
            if ((first_byte & 0x80) == 0) {
                char_len = 1;
            } else if ((first_byte & 0xE0) == 0xC0) {
                char_len = 2;
            } else if ((first_byte & 0xF0) == 0xE0) {
                char_len = 3;
            } else if ((first_byte & 0xF8) == 0xF0) {
                char_len = 4;
            } else {
                // Invalid UTF-8 sequence, skip this character
                idx++;
                continue;
            }

            // Convert UTF-8 bytes to uint32_t code point
            uint32_t codepoint = 0;
            switch (char_len) {
                case 1:
                    codepoint = first_byte;
                    break;
                case 2:
                    codepoint = (first_byte & 0x1F) << 6;
                    codepoint |= (line[idx + 1] & 0x3F);
                    break;
                case 3:
                    codepoint = (first_byte & 0x0F) << 12;
                    codepoint |= (line[idx + 1] & 0x3F) << 6;
                    codepoint |= (line[idx + 2] & 0x3F);
                    break;
                case 4:
                    codepoint = (first_byte & 0x07) << 18;
                    codepoint |= (line[idx + 1] & 0x3F) << 12;
                    codepoint |= (line[idx + 2] & 0x3F) << 6;
                    codepoint |= (line[idx + 3] & 0x3F);
                    break;
                default:
                    break;
            }

            // Call ParseCharacter with the UTF-8 code point
            ParseCharacter(codepoint);

            // Move to the next UTF-8 character
            idx += char_len;
        }
    }

    free(line);
    fclose(fileHandle);

    GLog(INFO, "Parsing complete for file \'%s\'", file);
}

void ParseCharacter(uint32_t codepoint) {

    if (codepoint <= 0x7F) {
        printf("1 byte\n");
        printf("Parse UTF-8 codepoint: U+%04X \'%c\'\n", codepoint, codepoint);
    } else if (codepoint <= 0x7FF) {
        printf("2 byte\n");
        printf("Parse UTF-8 codepoint: U+%04X '%c%c'\n", codepoint,
               (char)(0xC0 | (codepoint >> 6)),                         // First byte
               (char)(0x80 | (codepoint & 0x3F)));                      // Second byte
    } else if (codepoint <= 0xFFFF) {
        printf("3 byte\n");
        printf("Parse UTF-8 codepoint: U+%04X '%c%c%c'\n", codepoint,
               (char)(0xE0 | (codepoint >> 12)),                        // First byte
               (char)(0x80 | ((codepoint >> 6) & 0x3F)),                // Second byte
               (char)(0x80 | (codepoint & 0x3F)));                      // Third byte
    } else if (codepoint <= 0x10FFFF) {
        printf("4 byte\n");
        printf("Parse UTF-8 codepoint: U+%04X '%c%c%c%c'\n", codepoint,
               (char)(0xF0 | (codepoint >> 18)),                        // First byte
               (char)(0x80 | ((codepoint >> 12) & 0x3F)),               // Second byte
               (char)(0x80 | ((codepoint >> 6) & 0x3F)),                // Third byte
               (char)(0x80 | (codepoint & 0x3F)));                      // Fourth byte
    } else {
        printf("invalid UTF8\n");
    }

    return;
    
    

    if (isspace(codepoint)) {
        printf("Whitespace character: U+%04X\n", codepoint);
    } else if (iscntrl(codepoint)) {
        printf("Control character: U+%04X\n", codepoint);
    } else if (isalpha(codepoint)) {
        printf("Valid alpha character: U+%04X\n", codepoint);
    } else if (isdigit(codepoint)) {
        printf("Valid digit character: U+%04X\n", codepoint);
    } else if (ispunct(codepoint)) {
        printf("Valid punct character: U+%04X\n", codepoint);
    } else {
        printf("Other character: U+%04X\n", codepoint);
    }
}

