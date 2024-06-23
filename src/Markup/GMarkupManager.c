#include <GSPCore/GSPCore.h>
#include <GSPCore/Markup/GMarkupManager.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <string.h>

GMarkupParseState parseState = PARSE_STATE_READY;

GMarkupNode* rootNode = NULL;
GMarkupNode* activeNode = NULL; // deepest node that is incomplete


char classBuffer[128] = {0};

int CodepointWidth(uint32_t codepoint);
int ParseCharacter(uint32_t codepoint);
void PrintCodepoint(uint32_t codepoint);
void AppendCodepoint(char* destination, uint32_t codepoint);

void PrintStructure();

void AddNode(GMarkupNode* node);

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
            if (ParseCharacter(codepoint) < 0) {
                free(line);
                fclose(fileHandle);

                GLog(WARNING, "Parsing aborted for file \'%s\'", file);
                return;
            }

            // Move to the next UTF-8 character
            idx += char_len;
        }
    }

    free(line);
    fclose(fileHandle);


    

    GLog(INFO, "Parsing complete for file \'%s\'", file);

    PrintStructure();
}

int ParseCharacter(uint32_t codepoint) {
    /*
    if (codepoint <= 0x7F) {
        printf("Parse UTF-8 codepoint: U+%04X \'%c\'\n", codepoint, codepoint);
    } else if (codepoint <= 0x7FF) {
        printf("Parse UTF-8 codepoint: U+%04X '%c%c'\n", codepoint,
               (char)(0xC0 | (codepoint >> 6)),                         // First byte
               (char)(0x80 | (codepoint & 0x3F)));                      // Second byte
    } else if (codepoint <= 0xFFFF) {
        printf("Parse UTF-8 codepoint: U+%04X '%c%c%c'\n", codepoint,
               (char)(0xE0 | (codepoint >> 12)),                        // First byte
               (char)(0x80 | ((codepoint >> 6) & 0x3F)),                // Second byte
               (char)(0x80 | (codepoint & 0x3F)));                      // Third byte
    } else if (codepoint <= 0x10FFFF) {
        printf("Parse UTF-8 codepoint: U+%04X '%c%c%c%c'\n", codepoint,
               (char)(0xF0 | (codepoint >> 18)),                        // First byte
               (char)(0x80 | ((codepoint >> 12) & 0x3F)),               // Second byte
               (char)(0x80 | ((codepoint >> 6) & 0x3F)),                // Third byte
               (char)(0x80 | (codepoint & 0x3F)));                      // Fourth byte
    } else {
        printf("invalid UTF8\n");
    }*/

    
    switch (parseState) {
        case PARSE_STATE_READY:
            if (codepoint == '<') {
                //printf("OPENING TAG\n");
                classBuffer[0] = '\0';
                parseState = PARSE_STATE_OPENING_TAG;
            } else if (!isspace(codepoint)) {
                GLog(WARNING, "Unexpected character in READY:");
                printf("\t");
                PrintCodepoint(codepoint);
                printf("\n");
                return -1;
            }
            break;
        case PARSE_STATE_OPENING_TAG:
            if (CodepointWidth(codepoint) > 1 || isalpha(codepoint) || isdigit(codepoint)) {
                AppendCodepoint(classBuffer, codepoint);
                parseState = PARSE_STATE_CLASS;
            } else if (codepoint == '/') {
                classBuffer[0] = '\0';
                parseState = PARSE_STATE_START_CLOSE;
            } else {
                GLog(WARNING, "Unexpected character in OPENING TAG:");
                printf("\t");
                PrintCodepoint(codepoint);
                printf("\n");
                return -1;
            }
            break;
        case PARSE_STATE_CLASS:
            if (CodepointWidth(codepoint) > 1 || isalpha(codepoint) || isdigit(codepoint)) {
                AppendCodepoint(classBuffer, codepoint);
            } else if (isspace(codepoint)) {
                //printf("END CLASS\n");
                parseState = PARSE_STATE_IN_TAG;
                //printf("CLASS WAS %s\n", classBuffer);
            } else if (codepoint == '/') {
                //printf("end close\n");
                //printf("CLASS WAS %s\n", classBuffer);
                parseState = PARSE_STATE_END_CLOSE;
            } else if (codepoint == '>') {
                //printf("END TAG\n");
                //printf("CLASS WAS %s\n", classBuffer);

                GMarkupNode* node = calloc(1, sizeof(GMarkupNode));
                if (node == NULL) {
                    printf("node allocation failed\n");
                    return -1;
                }

                strcpy(node->class, classBuffer);
                node->isComplete = false;

                AddNode(node);
                parseState = PARSE_STATE_READY;

            } else {
                GLog(WARNING, "Unexpected character in CLASS:");
                printf("\t");
                PrintCodepoint(codepoint);
                printf("\n");
                return -1;
            }
            break;
        case PARSE_STATE_END_CLOSE:
            if (codepoint == '>') {

                GMarkupNode* node = calloc(1, sizeof(GMarkupNode));
                if (node == NULL) {
                    printf("node allocation failed\n");
                    return -1;
                }

                strcpy(node->class, classBuffer);
                node->isComplete = true;

                AddNode(node);
                parseState = PARSE_STATE_READY;

            } else {
                GLog(WARNING, "Unexpected character in END CLOSE:");
                printf("\t");
                PrintCodepoint(codepoint);
                printf("\n");
                return -1;
            }
            break;
        case PARSE_STATE_START_CLOSE:
            if (CodepointWidth(codepoint) > 1 || isalpha(codepoint) || isdigit(codepoint)) {
                AppendCodepoint(classBuffer, codepoint);
                parseState = PARSE_STATE_CLOSE_CLASS;
            } else {
                GLog(WARNING, "Unexpected character in START CLOSE:");
                printf("\t");
                PrintCodepoint(codepoint);
                printf("\n");
                return -1;
            }
            break;
        case PARSE_STATE_CLOSE_CLASS: 
            if (CodepointWidth(codepoint) > 1 || isalpha(codepoint) || isdigit(codepoint)) {
                AppendCodepoint(classBuffer, codepoint);
            } else if (codepoint == '>') {
                if (activeNode != NULL && strcmp(activeNode->class, classBuffer) == 0) {
                    printf("VALID CLOSING TAG\n");
                    activeNode->isComplete = true;
                    activeNode = activeNode->parent;
                    parseState = PARSE_STATE_READY;
                } else {
                    printf("INVALID CLOSING TAG CLASS %s\n", classBuffer);
                }
            } else {
                GLog(WARNING, "Unexpected character in CLOSE CLASS:");
                printf("\t");
                PrintCodepoint(codepoint);
                printf("\n");
                return -1;
            }
    }

    /*
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
    }*/

    return 0;
}

int CodepointWidth(uint32_t codepoint) {
    if (codepoint <= 0x7F) {
        return 1;
    } else if (codepoint <= 0x7FF) {
        return 2;
    } else if (codepoint <= 0xFFFF) {
        return 3;
    } else if (codepoint <= 0x10FFFF) {
        return 4;
    } else {
        return -1;
    } 
}

void PrintCodepoint(uint32_t codepoint) {
    if (codepoint <= 0x7F) {
        printf("%c", codepoint);
    } else if (codepoint <= 0x7FF) {
        printf("%c%c",
               (char)(0xC0 | (codepoint >> 6)),                         // First byte
               (char)(0x80 | (codepoint & 0x3F)));                      // Second byte
    } else if (codepoint <= 0xFFFF) {
        printf("%c%c%c",
               (char)(0xE0 | (codepoint >> 12)),                        // First byte
               (char)(0x80 | ((codepoint >> 6) & 0x3F)),                // Second byte
               (char)(0x80 | (codepoint & 0x3F)));                      // Third byte
    } else if (codepoint <= 0x10FFFF) {
        printf("%c%c%c%c",
               (char)(0xF0 | (codepoint >> 18)),                        // First byte
               (char)(0x80 | ((codepoint >> 12) & 0x3F)),               // Second byte
               (char)(0x80 | ((codepoint >> 6) & 0x3F)),                // Third byte
               (char)(0x80 | (codepoint & 0x3F)));                      // Fourth byte
    }
}

void AppendCodepoint(char* destination, uint32_t codepoint) {
    
    // use sprintf to write codepoints to string.

    char localbuf[8] = {0};

    if (codepoint <= 0x7F) {
        sprintf(localbuf, "%c", codepoint);
    } else if (codepoint <= 0x7FF) {
        sprintf(localbuf, "%c%c",
               (char)(0xC0 | (codepoint >> 6)),                         // First byte
               (char)(0x80 | (codepoint & 0x3F)));                      // Second byte
    } else if (codepoint <= 0xFFFF) {
        sprintf(localbuf, "%c%c%c",
               (char)(0xE0 | (codepoint >> 12)),                        // First byte
               (char)(0x80 | ((codepoint >> 6) & 0x3F)),                // Second byte
               (char)(0x80 | (codepoint & 0x3F)));                      // Third byte
    } else if (codepoint <= 0x10FFFF) {
        sprintf(localbuf, "%c%c%c%c",
               (char)(0xF0 | (codepoint >> 18)),                        // First byte
               (char)(0x80 | ((codepoint >> 12) & 0x3F)),               // Second byte
               (char)(0x80 | ((codepoint >> 6) & 0x3F)),                // Third byte
               (char)(0x80 | (codepoint & 0x3F)));                      // Fourth byte
    }


    strcat(destination, localbuf);
}

void AddNode(GMarkupNode* node) {

    if (rootNode == NULL) {
        rootNode = node;
        node->parent = NULL;
        printf("MADE NODE WITH CLASS \'%s\' ROOT\n", node->class);

        if (!node->isComplete) {
            activeNode = node;
            printf("MADE NODE WITH CLASS \'%s\' ACTIVE\n", node->class);
        } else {
            activeNode = NULL;
        }
    } else if (activeNode == NULL) {
        GMarkupNode* lastTopLevel = rootNode;

        printf("ADDING NODE %s WITH ROOT %s\n", node->class, rootNode->class);

        while (lastTopLevel->sibling != NULL) {
            lastTopLevel = lastTopLevel->sibling;
        }

        printf("ADDED TOP LEVEL NODE WITH CLASS \'%s\'\n", node->class);

        printf("SET NODE AS SIBLING FOR %s", lastTopLevel->class);
        lastTopLevel->sibling = node;
        node->parent = NULL;

        if (!node->isComplete) {
            activeNode = node;
            printf("MADE NODE WITH CLASS \'%s\' ACTIVE\n", node->class);
        }

    } else if (activeNode != NULL) {

        printf("ADDING NODE %s WITH ROOT %s AND ACTIVE %s\n", node->class, rootNode->class, activeNode->class);

        if (activeNode->child == NULL) {
            node->parent = activeNode;
            activeNode->child = node;
        
            if (!node->isComplete) {
                activeNode = node;
                printf("MADE NODE WITH CLASS \'%s\' ACTIVE\n", node->class);
            }
            return;
        }

        GMarkupNode* lastChild = activeNode->child;

        while (lastChild->sibling != NULL) {
            lastChild = lastChild->sibling;
        }

        lastChild->sibling = node;
        node->parent = activeNode;

        if (!node->isComplete) {
            activeNode = node;
            printf("MADE NODE WITH CLASS \'%s\' ACTIVE\n", node->class);
        } else {
            printf("NODE WITH CLASS \'%s\' WAS COMPLETE\n", node->class);

        }
    }
}


void PrintStructure() {

    GMarkupNode* node = rootNode;
    int level = 0;
    while (node != NULL) {
        for (int i = 0; i < level; i++) {
            printf("\t");
        }
        
        printf("%s\n", node->class);

        if (node->child != NULL) {
            level++;
            //printf("PRINTING CHILD FOR %s\n", node->class);
            node = node->child;
        } else if (node->sibling != NULL) {
            //printf("PRINTING SIBLING FOR %s\n", node->class);
            node = node->sibling;
        } else {
            while (node != NULL) {
                node = node->parent;
                level--;
                if (node != NULL && node->sibling != NULL) {
                    node = node->sibling;
                    break;
                }
            }
        }

    }

}