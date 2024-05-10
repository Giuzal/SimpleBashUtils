#include "s21_grep.h"

int main(int argc, char **argv) {
    run_grep(argc, argv);
    return 0;
}

void run_grep(int argc, char **argv) {
    if (argc > 1) {
        char *temp = NULL;
        grep_options config = {0};

        for (int x = parse_options(argc, argv, &config, &temp); x < argc; x += 1)
            process_file(argv[x], config, temp);

        if (temp != NULL) free(temp);
    }
}

int parse_options(int argc, char **argv, grep_options *config, char **pattern) {
    for (int sym = 0; (sym = getopt(argc, argv, "e:ivclnhsf:o")) != (-1);) {
        switch (sym) {
            case 'i':
                config->i_flag = 1;
                break;
            case 'v':
                config->v_flag = 1;
                break;
            case 'c':
                config->c_flag = 1;
                break;
            case 'l':
                config->l_flag = 1;
                break;
            case 'n':
                config->n_flag = 1;
                break;
            case 'h':
                config->h_flag = 1;
                break;
            case 's':
                config->s_flag = 1;
                break;
            case 'o':
                config->o_flag = 1;
                break;
            case 'e':
                set_config_e(config, pattern, optarg);
                break;
            case 'f':
                set_config_f(config, pattern, optarg);
                break;
        }
    }
    if ((config->e_flag || config->f_flag) == 0) {
        create_pattern(pattern, argv[optind]);
        optind += 1;
    }
    setup_config(config, argc);
    return optind;
}

void process_file(char *file_path, grep_options config, char *pattern) {
    FILE *file = fopen(file_path, "r");
    if (file != NULL) {
        for (char symbol = '0'; (symbol = getc(file)) != EOF;) {
            char *line = calloc(256, 1);
            config.line_number += 1;
            int length = 0;

            for (line[length] = '\0'; symbol != EOF && symbol != '\n'; symbol = getc(file)) {
                line[length] = symbol;
                line[length += 1] = '\0';
                if (length % 255 == 0) line = increase_string_length(line, length + 256);
            }

            print_match_data(line, &config, pattern, file_path);
            free(line);
        }
        print_aux_data(config, file_path);
        fclose(file);
    } else if (config.s_flag == 0) {
        fprintf(stderr, ERROR_01, file_path);
    }
}

void set_config_f(grep_options *config, char **pattern, char *optarg) {
    FILE *file = fopen(optarg, "r");
    if (file != NULL) {
        for (char symbol = '0'; (symbol = getc(file)) != EOF;) {
            int length = 0;
            char *line = calloc(256, 1);

            for (line[length] = '\0'; symbol != EOF && symbol != '\n'; symbol = getc(file)) {
                line[length] = symbol;
                line[length += 1] = '\0';
                if (length % 255 == 0)
                    line = (char *)increase_string_length(line, length + 256);
            }

            if (!(config->e_flag || config->f_flag))
                config->f_flag = create_pattern(pattern, line);
            else
                add_pattern(pattern, line);
            free(line);
        }
        fclose(file);
    } else if (config->s_flag == 0) {
        fprintf(stderr, ERROR_01, optarg);
    }
}

void set_config_e(grep_options *config, char **pattern, char *optarg) {
    if (!(config->e_flag || config->f_flag))
        config->e_flag = create_pattern(pattern, optarg);
    else
        add_pattern(pattern, optarg);
}

void print_aux_data(grep_options config, char *path) {
    if (config.c_flag) {
        if (config.l_flag) {
            config.file_count > 1 ? printf("%s:1\n", path) : printf("1\n");
        } else {
            if (config.file_count > 1) printf("%s:", path);
            printf("%i\n", config.match_count);
        }
    }
    if (config.l_flag && config.match_count) printf("%s\n", path);
}

void print_match_data(char *line, grep_options *config, char *pattern, char *path) {
    regex_t regex;
    if (regcomp(&regex, pattern, config->i_flag ? REG_ICASE : REG_EXTENDED) == 0) {
        if (regexec(&regex, line, 0, NULL, 0) == config->v_flag) {
            config->match_count += 1;
            if ((config->c_flag || config->l_flag) == 0) {
                if (config->file_count > 1 && !(config->h_flag)) printf("%s:", path);
                if (config->n_flag) printf("%i:", config->line_number);
                if (!config->o_flag)
                    printf("%s\n", line);
                else
                    print_config_o(regex, line, *config);
            }
        }
        regfree(&regex);
    }
}

void print_config_o(regex_t regex, char *line, grep_options config) {
    while (regexec(&regex, line, 0, NULL, 0) == config.v_flag) {
        char *aux = (char *)calloc(strlen(line) + 1, 1);
        strcpy(aux, line);
        int end = strlen(line);
        while (regexec(&regex, aux, 0, NULL, 0) == config.v_flag) {
            end--;
            aux[strlen(aux) - 1] = 0;
        }
        aux[strlen(aux)] = line[strlen(aux)];
        int start = 0;
        while (regexec(&regex, aux, 0, NULL, 0) == config.v_flag && strlen(aux) > 0) {
            start++;
            int j = 0;
            while (aux[j] != 0) {
                aux[j] = aux[j + 1];
                j++;
            }
        }
        start--;
        int i = strlen(aux);
        while (i != 0) {
            aux[i] = aux[i - 1];
            i--;
        }
        aux[0] = line[start];
        printf("%s\n", aux);
        free(aux);
        i = start + 1;
        while (line[i] != 0) {
            line[i - start - 1] = line[i];
            i++;
        }
        line[i - start - 1] = 0;
    }
}

void setup_config(grep_options *config, int argc) {
    if (config->o_flag && (config->l_flag || config->v_flag || config->c_flag)) config->o_flag = 0;
    config->file_count = argc - optind;
}

int create_pattern(char **str, char *optarg) {
    *str = calloc(strlen(optarg) + 1, 1);
    if (*str) strcpy(*str, optarg);
    return *str ? 1 : 0;
}

void add_pattern(char **str, char *optarg) {
    *str = increase_string_length(*str, strlen(*str) + strlen(optarg) + 2);
    if (*str) strcat(strcat(*str, "|"), optarg);
}

void *increase_string_length(void *str, int size) {
    char *aux = realloc(str, size);
    return aux;
}
