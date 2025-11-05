#define _CRT_SECURE_NO_WARNINGS
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_LINE_LEN 1024
#define TARGET_WIDTH 80


static char *format_line(const char *line) {
    if (!line) return NULL;
    
    int cnt = 0;
    size_t len = strlen(line);
    size_t start = 0;
    while (start < len && isspace((unsigned char)line[start])) start++;
    
    size_t end = len;
    while (end > start && isspace((unsigned char)line[end - 1])) end--;
    
    if (start >= end) {
        char *result = malloc(2);
        if (!result) return NULL;
        strcpy(result, "\n");
        return result;
    }
    
    if (end - start <= TARGET_WIDTH) {
        char *result = malloc(end - start + 2);
        if (!result) return NULL;
        strncpy(result, line + start, end - start);
        result[end - start] = '\n';
        result[end - start + 1] = '\0';
        return result;
    }

    char *result = (char *)calloc((len * 2) + 1, sizeof(char));
    if (!result) return NULL;

    size_t pos = 0;
    start = 0;

    while (start < len) {
        while (start < len && isspace((unsigned char)line[start])) start++;
        if (start >= len) break;

        size_t chunk_len = (len - start > TARGET_WIDTH) ? TARGET_WIDTH : len - start;
        size_t segment_end = start + chunk_len;

        if (segment_end < len && !isspace((unsigned char)line[segment_end]) && 
            !isspace((unsigned char)line[segment_end - 1])) {
            while (segment_end > start && !isspace((unsigned char)line[segment_end - 1])) {
                segment_end--;
                cnt++;
            }
            if (segment_end == start) {
                if (cnt >= 80) {return "___ERROR_LARGE_WORD___";}
                else {segment_end = start + chunk_len;}
            }
        }

        size_t segment_len = segment_end - start;
        char segment[MAX_LINE_LEN];
        strncpy(segment, line + start, segment_len);
        segment[segment_len] = '\0';

        size_t seg_len = strlen(segment);
        while (seg_len > 0 && isspace((unsigned char)segment[seg_len - 1])) {
            segment[seg_len - 1] = '\0';
            seg_len--;
        }

        char *ptr = segment;
        while (*ptr && isspace((unsigned char)*ptr)) ptr++;

        size_t segment_trimmed_len = strlen(ptr);
        
        if (segment_trimmed_len < TARGET_WIDTH && strchr(ptr, ' ') != NULL) {
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
                        for (size_t j = 0; j < space_between + ((size_t)i < extra ? 1 : 0); j++) {
                            result[pos++] = ' ';
                        }
                    }
                }
                
                while (pos > 0 && isspace((unsigned char)result[pos - 1])) {
                    pos--;
                }
                
                result[pos++] = '\n';
            }
        } else {
            pos += sprintf(result + pos, "%s\n", ptr);
        }

        start = segment_end;
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
    if (input_path == NULL || output_path == NULL) {
        return ERROR_NULL_POINTER;
    }

    FILE *in = fopen(input_path, "r");
    if (!in) return ERROR_FILE_OPEN;

    FILE *out = fopen(output_path, "w");
    if (!out) {
        fclose(in);
        return ERROR_FILE_OPEN;
    }

    char buffer[MAX_LINE_LEN];
    while (fgets(buffer, sizeof(buffer), in)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
        
        while (len > 0 && isspace((unsigned char)buffer[len - 1])) {
            buffer[len - 1] = '\0';
            len--;
        }

        char *formatted = format_line(buffer);
        if (!formatted) {
            fclose(in);
            fclose(out);
            return ERROR_MEMORY_ALLOCATION;
        }
        else if (strcmp(formatted, "___ERROR_LARGE_WORD___") == 0) {
            fclose(in);
            fclose(out);
            return ERROR_LARGE_WORD;
        }

        StatusCode write_status = write_line_to_file(out, formatted);
        free(formatted);
        
        if (write_status != SUCCESS) {
            fclose(in);
            fclose(out);
            return write_status;
        }
    }

    fclose(in);
    fclose(out);
    
    return SUCCESS;
}