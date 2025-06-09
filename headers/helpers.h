#pragma once
enum ARG_TYPE { INVALID = 0, RAW_TEXT = 1, PATH = 2 };

struct Program_Instructions {
	enum ARG_TYPE arg_Type;
	const char * content;
};

void print_error(const char* error_message);

void print_help_screen();

//look into returning a BOOL type where 0 = FALSE and 1 = TRUE
int tryParseArguments(const char* arg_type, const char* content, struct Program_Instructions* ins);

void print_local_ip();

void send_404(int client_socket);

void serve_file(int client_socket, const char* page_or_path, enum ARG_TYPE arg_type);

const char* prepare_html_file(struct Program_Instructions* ins);

int set_non_blocking_socket(int server_socket);
