#define _CRT_SECURE_NO_WARNINGS
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_LINE_LEN 1024
#define TARGET_WIDTH 80

static StatusCode validate_string(const char* str) {
    if (str == NULL) {
        return ERROR_NULL_POINTER;
    }
    return SUCCESS;
}

static FILE *open_file(const char *path, const char *mode) {
    if (path == NULL || mode == NULL) {
        return NULL;
    }
    FILE *f = fopen(path, mode);
    return f;
}

static char *format_line(const char *line) {
    if (!line) return NULL;

    size_t len = strlen(line);
    char *result = (char *)calloc((len * 2) + 1, sizeof(char));
    if (!result) return NULL;

    size_t start = 0;
    size_t pos = 0;

    while (start < len) {
        size_t chunk_len = (len - start > TARGET_WIDTH) ? TARGET_WIDTH : len - start;
        size_t end = start + chunk_len;

        if (end < len && !isspace((unsigned char)line[end]) && !isspace((unsigned char)line[end - 1])) {
            while (end > start && !isspace((unsigned char)line[end - 1])) {
                end--;
            }
            if (end == start) end = start + chunk_len;
        }

        size_t segment_len = end - start;
        char segment[MAX_LINE_LEN];
        strncpy(segment, line + start, segment_len);
        segment[segment_len] = '\0';

        char *ptr = segment;
        while (*ptr && isspace((unsigned char)*ptr)) ptr++;

        size_t slen = strlen(ptr);
        if (slen < TARGET_WIDTH) {
            char *words[256];
            int word_count = 0;
            char *token = strtok(ptr, " ");
            while (token && word_count < 256) {
                words[word_count++] = token;
                token = strtok(NULL, " ");
            }

            if (word_count == 1) {
                pos += sprintf(result + pos, "%s\n", words[0]);
            } else {
                size_t total_word_len = 0;
                for (int i = 0; i < word_count; i++) {
                    total_word_len += strlen(words[i]);
                }
                size_t total_spaces = TARGET_WIDTH - total_word_len;
                size_t space_between = total_spaces / (word_count - 1);
                size_t extra = total_spaces % (word_count - 1);

                for (int i = 0; i < word_count; i++) {
                    pos += sprintf(result + pos, "%s", words[i]);
                    if (i < word_count - 1) {
                        for (size_t j = 0; j < space_between + ((size_t)i < extra ? 1 : 0); j++)
                            result[pos++] = ' ';
                    }
                }
                result[pos++] = '\n';
            }
        } else {
            pos += sprintf(result + pos, "%s\n", ptr);
        }

        start = end;
        while (isspace((unsigned char)line[start])) start++;
    }

    result[pos] = '\0';
    return result;
}

static StatusCode write_line_to_file(FILE *out, const char *formatted_line) {
    if (!out || !formatted_line) return ERROR_NULL_POINTER;
    if (fputs(formatted_line, out) == EOF) return ERROR_FILE_WRITE;
    return SUCCESS;
}

StatusCode process_file(const char *input_path, const char *output_path) {
    StatusCode status = validate_string(input_path);
    if (status != SUCCESS) {
        return status;
    }
    
    status = validate_string(output_path);
    if (status != SUCCESS) {
        return status;
    }

    if (strcmp(input_path, output_path) == 0) {
        return ERROR_INVALID_INPUT;
    }

    FILE *in = open_file(input_path, "r");
    if (!in) return ERROR_FILE_OPEN;

    FILE *out = open_file(output_path, "w");
    if (!out) {
        fclose(in);
        return ERROR_FILE_OPEN;
    }

    char buffer[MAX_LINE_LEN];
    while (fgets(buffer, sizeof(buffer), in)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';

        char *formatted = format_line(buffer);
        if (!formatted) {
            fclose(in);
            fclose(out);
            return ERROR_MEMORY_ALLOCATION;
        }

        StatusCode write_status = write_line_to_file(out, formatted);
        free(formatted);
        
        if (write_status != SUCCESS) {
            fclose(in);
            fclose(out);
            return write_status;
        }
    }

    if (fclose(in) != 0) {
        fclose(out);
        return ERROR_FILE_READ;
    }

    if (fclose(out) != 0) {
        return ERROR_FILE_WRITE;
    }

    return SUCCESS;
}