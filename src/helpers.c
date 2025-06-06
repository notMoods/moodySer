#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include "../headers/helpers.h"


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

