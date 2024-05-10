#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_01 "s21_grep: %s: No such file or directory\n"

typedef struct {
    int e_flag;
    int i_flag;
    int v_flag;
    int c_flag;
    int l_flag;
    int n_flag;
    int h_flag;
    int s_flag;
    int f_flag;
    int o_flag;
    int file_count;
    int line_number;
    int match_count;
} grep_options;

void run_grep(int argc, char **argv);
int parse_options(int argc, char **argv, grep_options *config, char **pattern);
void process_file(char *file_path, grep_options config, char *pattern);
void print_match_data(char *line, grep_options *options, char *pattern, char *file_name);
void set_config_f(grep_options *config, char **pattern, char *optarg);
void set_config_e(grep_options *config, char **pattern, char *optarg);
void print_config_o(regex_t regex, char *line, grep_options config);
void print_aux_data(grep_options config, char *path);
void *increase_string_length(void *str, int size);
int create_pattern(char **str, char *optarg);
void setup_config(grep_options *config, int argc);
void add_pattern(char **str, char *optarg);

#endif  // SRC_GREP_S21_GREP_H_