#pragma once
enum Arg_Type { Invalid = 0, Raw_Text = 1, Path = 2 };

struct Program_Instructions {
	enum Arg_Type arg_Type;
	const char * content;
};

void print_error(const char* error_message);

void print_help_screen();

//look into returning a BOOL type where 0 = FALSE and 1 = TRUE
int tryParseArguments(const char* arg_type, const char* content, struct Program_Instructions* ins);

void print_local_ip();

void send_404(int client_socket);

void serve_file(int client_socket, const char* path, const char* content_type);

const char* prepare_html_file(struct Program_Instructions* ins);
