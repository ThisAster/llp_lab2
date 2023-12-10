#include "../include/parser.h"

void print_error() {
    printf("incorrect\n");
    exit(EXIT_FAILURE);
}

void check_path(char *req, int *path_length) {
    int bracket = 0;

    for (int i = 0; i < *path_length; i++) {
        if (req[i] == '[') bracket++;
        if (req[i] == ']') bracket--;
    }

    if (bracket != 0) {
        print_error();
    }
}

void remove_char(char **req, int *path_length) {
    (*path_length)--;
    (*req)++;
}

int check_on_del_char(char symbol) {
    if (symbol == '=') return 1;
    if (symbol == '!') return 1;
    if (symbol == '>') return 1;
    if (symbol == '<') return 1;

    return 0;
}

char *read_condition(char **req, int *path_length) {

    char *cond = calloc(4, sizeof(char));

    int i;
    for (i = 0; i < *path_length; i++) {
        if (((*req)[i] >= 'a' && (*req)[i] <= 'z') || ((*req)[i] >= '1' && (*req)[i] <= '9')) {
            break;
        }
        cond[i] = (*req)[i];
    }

    for (int j = 0; j < i; j++) {
        remove_char(req, path_length);
    }

    return cond;
}

char *read_left(char **req, int *path_length) {

    if (*req[0] == '[') remove_char(req, path_length);
    char *word = calloc(MAX_STRING_SIZE, sizeof(char));

    int i;
    for (i = 0; i < *path_length; i++) {
        if (check_on_del_char((*req)[i])) {
            break;
        }
        word[i] = (*req)[i];
    }

    for (int j = 0; j < i; j++) {
        remove_char(req, path_length);
    }

    return word;
}

char *read_word(char **req, int *path_length) {

    if (*req[0] == '[') remove_char(req, path_length);
    char *word = calloc(MAX_STRING_SIZE, sizeof(char));

    int i;
    for (i = 0; i < *path_length; i++) {
        if ((*req)[i] == '/' || (*req)[i] == ']' || (*req)[i] == '[') {
            if ((*req)[i] == ']' || (*req)[i] == '[') i--;
            break;
        }
        word[i] = (*req)[i];
    }
    i++;

    for (int j = 0; j < i; j++) {
        remove_char(req, path_length);
    }

    return word;
}

void read_attributes(char **req, int *path_length, struct attribute *attribute) {

    if ((*req)[0] == '[') {

        char *left = read_left(req, path_length);
        char *cond = read_condition(req, path_length);
        char *right = read_word(req, path_length);

        attribute->left = left;
        attribute->condition = cond;
        attribute->right = right;
    }

    if ((*req)[0] == ']') {
        remove_char(req, path_length);
        if (*path_length > 1) {
            struct attribute *new_attribute = malloc(sizeof(struct attribute));
            read_attributes(req, path_length, new_attribute);
            attribute->next_attribute = new_attribute;
        }
    } else if ((*req)[0] == '|' || (*req)[0] == '&') {
        char *combined_condition = read_word(req, path_length);
        attribute->combined_condition = combined_condition;
        read_attributes(req, path_length, attribute);
    }
}

enum parser_status parse_request(char *req, struct request *request) {

    int path_length = strlen(req);

    check_path(req, &path_length);

    char *operation = read_word(&req, &path_length);
    request->operation = operation;

    if (!(
            strcmp("add", operation) == 0 ||
            strcmp("find", operation) == 0 ||
            strcmp("remove", operation) == 0 ||
            strcmp("update", operation) == 0
    ))
        print_error();

    char *id = read_word(&req, &path_length);
    request->parent_id = id;

    if (req[0] == '[') {
        struct attribute *attribute = malloc(sizeof(struct attribute));
        request->attributes = attribute;
        read_attributes(&req, &path_length, attribute);
    } else if (req[0] == '*') request->star = "*";

    return PARSE_OK;
}
