#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define LSIZ 128 
#define RSIZ 10 
#include "argdigest.h"
#define print_success(fmt, args...) fprintf(stdout, "\033[01;32mSuccess:\033[0m "fmt, ##args); fflush(stdout);
#define print_warning(fmt, args...) fprintf(stdout, "\033[01;33mWarning:\033[0m "fmt, ##args); fflush(stdout);
#define print_failure(fmt, args...) fprintf(stderr, "\033[01;31mFailure:\033[0m "fmt, ##args); fflush(stderr);
#define print_manuals(fmt, args...) fprintf(stdout, "\033[01;34mManuals:\033[0m "fmt, ##args); fflush(stdout);
char *keyword = {"print"};

int main(int argc, char **argv)
{
    
    char c; 
    
    int counter = 1;

    char *output_file = "program.out"; // Default output file if "-o" isn't chosen

    FILE *fptr;

    int i = 0;

    int tot = 0;

    char line[RSIZ][LSIZ];

    ArgDigest args; // ArgDigest struct
    ArgDigestInit(&args, argc, argv, NULL, "A lexical analysis compiler example"); // Initialize args
    ArgDigestAddParam(&args, "Source", "-s", "--source", "Make a copy of the source file", ARG_SWITCH, false);
    ArgDigestAddParam(&args, "File", "-f", "--file", "The source file you want to compile", ARG_STR, true); // "-f" OR "--file" flag with the name "File": ArgDigestAddParam(structure=args, name="File", short_flag="-f", long_flag="--file", help="The source file you want to compile", data_type=string, required=true);
    ArgDigestAddParam(&args, "Output", "-o", "--output", "The output binary file", ARG_STR, false); // "-o" OR "--output" flag with the name "Output": ArgDigestAddParam(structure=args, name="Output", short_flag="-f", long_param="--file", help="The output binary file", data_type=string, required=false);
    ArgDigestInvokeDigestion(&args);

    ArgResult *arg_file = ArgDigestGetValue(&args, "File");
    ArgResult *arg_output = ArgDigestGetValue(&args, "Output");

    if (arg_file == NULL || arg_file->value == NULL)
    {
        return -1;
    }

    if (arg_output != NULL) // if "-o" or "--output" wasn't chosen, arg_output will be NULL
    {
        output_file = arg_output->value;
    }

    char *source_file = arg_file->value;

    print_success("File to compile -> %s\n", source_file);
    print_success("Output binary   -> %s\n\n", output_file);

    fptr = fopen(source_file, "r");
    if (fptr == NULL)
    {
        print_failure("Failed to open file [%s] - %s\n", source_file, strerror(errno));
        return -1;
    }

    fseek(fptr, 0, SEEK_END);
    long file_len = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    if (file_len < 0)
    {
        print_failure("Cannot get length of file contents [%s]\n", source_file);
        return -1;
    }

    char data[file_len];

    fread(data, file_len, 1, fptr);

    data[file_len] = '\0';

    fptr = fopen(source_file, "r");

    while(fgets(line[i], LSIZ, fptr)) 
	{
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
    tot = i;

    for(i = 0; i < tot; ++i)
    {
        if(!strncmp(line[i],"print",256)){
            printf("found print on line%d\n",counter++);}

        else{
            printf(" %s %d\n", line[i],counter++);}
    }


    fclose(fptr);

    print_success("Read file successfully [length: %04ld]\n", file_len);

    return 0;
}
