#include "../include/parser.h"
#include "../include/view.h"

int main(int argc, char **argv) {

    printf("enter your request\n");

    char req[MAX_REQUEST_SIZE];
    fgets(req, MAX_REQUEST_SIZE, stdin);

    struct request *request = malloc(sizeof(struct request));
    enum parser_status status = parse_request(req, request);

    if (status == PARSE_OK) get_request_view(request);

    free(request);

    return 0;
}