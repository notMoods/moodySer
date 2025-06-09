//steps to use the program
//1. Launch the program with a path-to-file
//2. The program outputs a URL link
//3. Devices connected on the same local network can
//   go to the URL and click a download button
//4. Once the Download button is clicked, the file
//   is downloaded to the device


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <errno.h>
#include "../headers/helpers.h"
#include "../headers/input.h"

#define PORT 8080
#define BUFFER_SIZE 4096

static int check_for_halt(){
	int ch = getchar();

	if (ch != EOF) {
		ch = tolower(ch);

		if(ch == 'q') {
			printf("Shutting down...\n");
			return 1;
		}
	}

	return 0;

}
int main(int argc, char *argv[]){
	struct Program_Instructions instructions;

	if (argc != 3){
		print_error("Only Two Arguments: [arg-type] [content] are accepted!");

		print_help_screen();

		return 1;
	}

	if(! tryParseArguments(argv[1], argv[2], &instructions) ) {
		print_error("Invalid Arguments Given!");

		print_help_screen();

		return 1;
	}

	//at this point we have a valid program instruction
	
	const char* html_string = prepare_html_file(&instructions);

	int server_fd, client_socket;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	char buffer[BUFFER_SIZE];

	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	bind(server_fd, (struct sockaddr*) &address, sizeof(address));
	listen(server_fd, 10);

	if(set_non_blocking_socket(server_fd) == 0){
		printf("Server in blocking mode\n");
	}else {
		printf("Server in non-blocking mode\n");
	}

	enable_non_blocking_input();
	
	printf("\n\n");
	print_local_ip();
	printf("\n");
	printf("Server running on port %d. Press Q to quit.\n", PORT);

	char download_buffer[80];

	while(1) {
		client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
		int bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);

		if(bytes_read <= 0) {
			if (check_for_halt() != 0) break;
			close(client_socket);
			usleep(10000);
			continue;
		}

		buffer[bytes_read] = '\0';

		if(strncmp(buffer, "GET / ", 6) == 0) {
			serve_file(client_socket, html_string, RAW_TEXT);
		} else {
			sprintf(download_buffer, "GET /%s", instructions.content);

			if(strncmp(buffer, download_buffer, strlen(download_buffer)) == 0){
				serve_file(client_socket, instructions.content, PATH);
			} else {
				send_404(client_socket);
			}
		}

		close(client_socket);

		if (check_for_halt() != 0) break; 

		usleep(10000);
	}

	
	close(server_fd);
	free((void*)html_string);
	disable_non_blocking_input();
	return 0;

}
