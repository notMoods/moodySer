#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>

#include "../headers/helpers.h"

#define PORT 8080


void print_error(const char* message){
	printf("\n\033[31mERROR\033[0m: %s\n", message);
}

static int string_compare(const char* first, const char* second){

	int ptr = 0;

	while(! (first[ptr] == '\0' && second[ptr] == '\0') ) {
		if(first[ptr] != second[ptr]){
			return 0;
		}

		ptr++;
	}

	return 1;
}

static int is_valid_filepath(const char* path){
	struct stat st;

	if(stat(path, &st) != 0) 
		return 0;

	if(!S_ISREG(st.st_mode))
		return 0;

	FILE* file = fopen(path, "r");
	if(!file)
		return 0;

	fclose(file);
	return 1;
}


void print_help_screen(){

	const char* first = "mser: How To Use:";
	const char* second = "mser [arg-type] [content]";

	const char* third = "[arg-type]:  raw-text  |  path";
	const char* fourth = "[content]:  \"{insert-text-without-brace}\" if raw-text\n\t\t    path/to/file if path is selected";

	printf("\t%s\n\n\t%s\n\n\t%s\n\n\t%s\n", first, second, third, fourth);
}


int tryParseArguments(const char* arg_type, const char* content, struct Program_Instructions* ins){
	if( string_compare(arg_type, "raw-text") ) {
		//at this point it's a raw text situation
		ins->arg_Type = 1;
		ins->content = content;

		return 1;
	}

	if( string_compare(arg_type, "path") && is_valid_filepath(content) ) {
		//at this point it is a valid file situation
		ins->arg_Type = 2;
		ins->content = content;

		return 1;
	}

	//at this point the args are invalid
	return 0;
}

const char* prepare_html_file(struct Program_Instructions* ins){
	const char* html_string = 
		"<!DOCTYPE html>\n"
		"<html lang=\"en\">\n"
		"<head>\n"
		"	<meta charset=\"UTF-8\">\n"
		"	<title>moodySer</title>\n"
		"</head>\n"
		"<body>\n    "
		"%s\n"
		"</body>\n"
		"</html>\n";

	char* content = (char*)malloc(strlen(ins->content) + 75);

	if(ins->arg_Type == 1) {
		sprintf(content, "<p>%s</p>", ins->content);
	}
	else {
	       //do something with download here to maybe make
	       //the path hidden in the html?, i dont r know
	       sprintf(content, "<a href=\"%s\" download=\"file\">\n	<button>Download File</button>\n    </a>", ins->content);
	}

	size_t total_length = strlen(html_string) + strlen(content) + 1;
	char* html_file = (char*)malloc(total_length);

	sprintf(html_file, html_string, content);

	free((void*)content);

	return html_file;
}

void print_local_ip() {
	struct ifaddrs *ifaddr, *ifa;

	char ip[INET_ADDRSTRLEN];

	if (getifaddrs(&ifaddr) == -1){
		perror("getifaddr");
		return;
	}

	printf("Go to these links on the device on the same network\n");

	for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){

		if(ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET)
			continue;

		void *addr_ptr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;

		inet_ntop(AF_INET, addr_ptr, ip, sizeof(ip));

		if(strcmp(ip, "127.0.0.1") != 0)
			printf("\thttp://%s:%d/\n", ip, PORT);

	}

	freeifaddrs(ifaddr);
}

void send_404(int client_socket){
	const char *header = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n";

	const char *body = "<html><body><h1>404 Not Found</h1></body></html>\n";

	write(client_socket, header, strlen(header));

	write(client_socket, body, strlen(header));
}

void serve_file(int client_socket, const char* path, const char* content_type){


}
