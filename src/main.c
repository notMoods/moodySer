//steps to use the program
//1. Launch the program with a path-to-file
//2. The program outputs a URL link
//3. Devices connected on the same local network can
//   go to the URL and click a download button
//4. Once the Download button is clicked, the file
//   is downloaded to the device


#include <stdio.h>
#include "../headers/helpers.h"

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
	

	return 0;

}
