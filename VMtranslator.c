#include "stdio.h"
#include "string.h"
#include "dirent.h"
#define ROW 350

#include "codewriter.h"

int main(int argc, char *argv[]){

	//works with multiple files
	if(argc == 2){
		char input[ROW], output[ROW];
		int counter = 0;
		DIR *dir;
		struct dirent *ptr;
		char directory[5]="./";
		FILE *f1, *f2;
		f2=fopen(argv[1], "w");
		
		//start of stack segment memory
		fprintf(f2, "%s", "@256\nD=A\n@SP\nM=D\n");
		
		//call of Sys.init function
		int_to_char(counter, output);
		strcpy(input, "Sys.init");
		call(input, "0", output);
		fprintf(f2, "%s", input);
		
		//iterates all files in current directory
		dir=opendir(directory);
		while ((ptr=readdir(dir)) != NULL) {
			
			//checks if the file has .vm extension and opens it
            		if(bool_vm(ptr->d_name)){
				f1=fopen(ptr->d_name, "r");
				
				//reads opened file row by row
				while(fgets(input, ROW, f1)!=NULL){
					if((*input!='\0')&&(*input!='/')&&(*input!='\n')&&(*input!='\r')&&(*input!='\t')){

						//parses vm-commands and translates into corresponding assembler code 
						parser(input, ptr->d_name, counter, argc);
						copy(input, output);
						fprintf(f2, "%s", output);
						counter++;
						}
					}
				fclose(f1);
    				}
			}
		closedir(dir);
		fclose(f2);
	}

	//works only with one file
	else if (argc == 3){
		char input[ROW], output[ROW];
		char namefile[ROW];
		int counter = 0;
		strcpy(namefile, argv[1]);
		FILE *f1, *f2;
		f1=fopen(argv[1], "r");
		f2=fopen(argv[2], "w");

		//start of stack segment memory
		fprintf(f2, "%s", "@256\nD=A\n@SP\nM=D\n");

		//reads opened file row by row
		while(fgets(input, ROW, f1)!=NULL){
			if((*input!='\0')&&(*input!='/')&&(*input!='\n')&&(*input!='\r')&&(*input!='\t')){

				//parses vm-commands and translates into corresponding assembler code
				parser(input, namefile, counter, argc);
				copy(input, output);
				fprintf(f2, "%s", output);
				counter++;
			}
		}

		fclose(f1);
		fclose(f2);
	}

	else{
		printf("Not enough input arguments!");
	}
	
	return 0;

}
