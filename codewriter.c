#include "stdio.h"
#include "string.h"
#define ROW 350
#define N 6

#include "codewriter.h"

//counts number of words in the string
int words(char *str){
	int word=0, flag=0;
	while((*str!='\0')&&(*str!='/')&&(*str!='\n')&&(*str!='\r')&&(*str!='\t')){
		if(*str==' '){
			flag=1;
		}
		else{
			flag=0;
		}
		str++;

		if(((*str>=48 && *str<=57)||(*str>=65 && *str<=90)||(*str>=97 && *str<=122)) && flag==1){
			word=word+1;
		}
	}
	return word+1;

}

//converts integer into string
void int_to_char(int val, char *str){
	int n[N], i=0;
	do{
		n[i]=val%10;
		val=val/10;
		i++;
	}while(val!=0);


	for(int j=i-1; j>=0; j--){
		*str=n[j]+48;
		str++;
	}
	*str='\0';
}


//transaltes push command with given memory segment and value
void push(char *segment, char *value){
	char stack[ROW];
	strcpy(stack, "@SP\nA=M\nM=D\n@SP\nM=M+1\n");

	if(strcmp(segment, "constant")==0){
		strcpy(segment, "@");
		strcat(segment, value);
		strcat(segment, "\nD=A\n");
		strcat(segment, stack);
	}

	else if(strcmp(segment, "argument")==0){
		strcpy(segment, "@ARG\nD=M\n@");
		strcat(segment, value);
		strcat(segment, "\nA=D+A\nD=M\n");
		strcat(segment, stack);
	}

	else if(strcmp(segment, "local")==0){
		strcpy(segment, "@LCL\nD=M\n@");
		strcat(segment, value);
		strcat(segment, "\nA=D+A\nD=M\n");
		strcat(segment, stack);
	}

	else if(strcmp(segment, "static")==0){
		strcpy(segment, ".");
		strcat(segment, value);
		strcat(segment, "\nD=M\n@");
		strcat(segment, value);
		strcat(segment, "\nA=D+A\nD=M\n");
		strcat(segment, stack);
	}
	else if(strcmp(segment, "this")==0){
		strcpy(segment, "@THIS\nD=M\n@");
		strcat(segment, value);
		strcat(segment, "\nA=D+A\nD=M\n");
		strcat(segment, stack);
	}

	else if(strcmp(segment, "that")==0){
		strcpy(segment, "@THAT\nD=M\n@");
		strcat(segment, value);
		strcat(segment, "\nA=D+A\nD=M\n");
		strcat(segment, stack);
	}

	else if(strcmp(segment, "temp")==0){
		strcpy(segment, "@R5\nD=A\n@");
		strcat(segment, value);
		strcat(segment, "\nA=D+A\nD=M\n");
		strcat(segment, stack);
	}

	else if(strcmp(segment, "pointer")==0){
		strcpy(segment, "@THIS\nD=A\n@");
		strcat(segment, value);
		strcat(segment, "\nA=D+A\nD=M\n");
		strcat(segment, stack);
	}


}

////transaltes pop command with given memory segment and value
void pop(char *segment, char *value){
	char stack[ROW];
	strcpy(stack, "\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n");

	if(strcmp(segment, "argument")==0){
		strcpy(segment, "@ARG\nD=M\n@");
		strcat(segment, value);
		strcat(segment, stack);
	}

	else if(strcmp(segment, "local")==0){
		strcpy(segment, "@LCL\nD=M\n@");
		strcat(segment, value);
		strcat(segment, stack);
	}

	else if(strcmp(segment, "static")==0){
		strcpy(segment, ".");
		strcat(segment, value);
		strcat(segment, "\nD=M\n@");
		strcat(segment, value);
		strcat(segment, stack);
	}
	else if(strcmp(segment, "this") == 0){
		strcpy(segment, "@THIS\nD=M\n@");
		strcat(segment, value);
		strcat(segment, stack);
	}

	else if(strcmp(segment, "that") == 0){
		strcpy(segment, "@THAT\nD=M\n@");
		strcat(segment, value);
		strcat(segment, stack);
	}

	else if(strcmp(segment, "temp") == 0){
		strcpy(segment, "@R5\nD=A\n@");
		strcat(segment, value);
		strcat(segment, stack);
	}

	else if(strcmp(segment, "pointer") == 0){
		strcpy(segment, "@THIS\nD=A\n@");
		strcat(segment, value);
		strcat(segment, stack);
	}

}


//translates branching command (if-goto, goto, label)
void branching(char *command, char *label){
	if(strcmp(command, "if-goto")==0){
		strcpy(command, "@SP\nAM=M-1\nD=M\n@");
		strcat(command, label);
		strcat(command, "\nD;JNE\n");
	}

	else if(strcmp(command, "goto")==0){
		strcpy(command, "@");
		strcat(command, label);
		strcat(command, "\n0;JMP\n");
	}

	else if(strcmp(command, "label")==0){
		strcpy(command, "(");
		strcat(command, label);
		strcat(command, ")\n");
	}

}

//translates arithmetic and logic operations 
void math(char *operator, char *str){

	if (strcmp(operator, "add")==0){
		strcpy(operator, "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=M+D\n@SP\nM=M+1\n");
	}

	else if(strcmp(operator, "sub")==0){
		strcpy(operator, "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=M-D\n@SP\nM=M+1\n");
	}

	else if(strcmp(operator, "neg")==0){
		strcpy(operator, "@SP\nAM=M-1\nM=-M\n@SP\nM=M+1\n");
	}

	else if(strcmp(operator, "eq")==0){
		strcpy(operator, "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=D-M\n@EQ");
		strcat(operator, str);
		strcat(operator, "\nD;JEQ\n@SP\nA=M\nM=0\n@NEQ");
		strcat(operator, str);
		strcat(operator, "\n0;JMP\n(EQ");
		strcat(operator, str);
		strcat(operator, ")\n@SP\nA=M\nM=-1\n(NEQ");
		strcat(operator, str);
		strcat(operator, ")\n@SP\nM=M+1\n");
	}

	else if(strcmp(operator, "lt")==0){
		strcpy(operator, "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=D-M\n@NLT");
		strcat(operator, str);
		strcat(operator, "\nD;JLE\n@SP\nA=M\nM=-1\n@LT");
		strcat(operator, str);
		strcat(operator, "\n0;JMP\n(NLT");
		strcat(operator, str);
		strcat(operator, ")\n@SP\nA=M\nM=0\n(LT");
		strcat(operator, str);
		strcat(operator, ")\n@SP\nM=M+1\n");
	}

	else if(strcmp(operator, "gt")==0){
		strcpy(operator, "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=D-M\n@NGT");
		strcat(operator, str);
		strcat(operator, "\nD;JGE\n@SP\nA=M\nM=-1\n@GT");
		strcat(operator, str);
		strcat(operator, "\n0;JMP\n(NGT");
		strcat(operator, str);
		strcat(operator, ")\n@SP\nA=M\nM=0\n(GT");
		strcat(operator, str);
		strcat(operator, ")\n@SP\nM=M+1\n");
	}

	else if(strcmp(operator, "and")==0){
		strcpy(operator, "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=D&M\n@SP\nM=M+1\n");
	}

	else if(strcmp(operator, "or")==0){
		strcpy(operator, "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=M|D\n@SP\nM=M+1\n");
	}

	else if(strcmp(operator, "not")==0){
		strcpy(operator, "@SP\nAM=M-1\nM=!M\n@SP\nM=M+1\n");
	}

}


//translates call of function
void call(char *str1, char *str2, char *str3){
	char aux[ROW];

	strcpy(aux, "@RETURN_");
	strcat(aux, str1);
	strcat(aux, str3);
	strcat(aux, "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@SP\nD=M\n@5\nD=D-A\n@");
	strcat(aux, str2);
	strcat(aux, "\nD=D-A\n@ARG\nM=D\n@SP\nD=M\n@LCL\nM=D\n@");
	strcat(aux, str1);
	strcat(aux, "\n0;JMP\n(RETURN_");
	strcat(aux, str1);
	strcat(aux, str3);
	strcat(aux, ")\n");
	strcpy(str1, aux);

}


//translates function
void function(char *str1, char *str2, char *str3){
	char aux[ROW];

	strcpy(aux, "(");
	strcat(aux, str1);
	strcat(aux, ")\n");

	if(strcmp(str2, "0")!=0){
		strcat(aux, "@");
		strcat(aux, str2);
		strcat(aux, "\nD=A\n(PUSHARG");
		strcat(aux, str3);
		strcat(aux, ")\n@SP\nA=M\nM=0\n@SP\nM=M+1\nD=D-1\n@PUSHARG");
		strcat(aux, str3);
		strcat(aux, "\nD;JNE\n");
	}

	strcpy(str1, aux);

}


//translates return command
void return_value(char *str){

	strcpy(str, "@LCL\nD=M\n@R7\nM=D\n@5\nA=D-A\nD=M\n@R14\nM=D\n@ARG\nD=M\n@0\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n@ARG\nD=M\n@SP\nM=D+1\n@R7\nD=M\n@1\nA=D-A\nD=M\n@THAT\nM=D\n@R7\nD=M\n@2\nA=D-A\nD=M\n@THIS\nM=D\n@R7\nD=M\n@3\nA=D-A\nD=M\n@ARG\nM=D\n@R7\nD=M\n@4\nA=D-A\nD=M\n@LCL\nM=D\n@R14\nA=M\n0;JMP\n");

}


//parses vm-commands and translates into corresponding assembler code
void parser(char *str1, char *str2, int count, int argc){

	char aux[ROW];
	strcpy(aux, str1);

	//vm-command has 3 words
	if(words(str1)==3){
		char aux1[ROW];
		char aux2[ROW];
		char aux3[ROW];
		char name[ROW];
		char counter[ROW];

		parse_elem(str1, aux, aux1);
		strcpy(str1, aux);
		parse_elem(str1, aux, aux2);
		strcpy(str1, aux);
		parse_last_elem(str1, aux3);
		parse_name(str2, name);
		
		//translates push/pop into static segment memory while working with multiple files 
		if((argc == 2)&&(strcmp(aux2, "static")==0)){
			if(strcmp(aux1, "push")==0){
				push_static_mult(name, aux3, str1);
			} 

			else if(strcmp(aux1, "pop")==0){
				pop_static_mult(name, aux3, str1);
			}
		}

		else{
			if(strcmp(aux1, "push")==0){
				push(aux2, aux3);
				if(aux2[0]=='.'){
					strcpy(str1, "@");
					strcat(str1, name);
					strcat(str1, aux2);
				}
				else{
					strcpy(str1, aux2);
				}
			}
			else if(strcmp(aux1, "pop")==0){
				pop(aux2, aux3);
				if(aux2[0]=='.'){
					strcpy(str1, "@");
					strcat(str1, name);
					strcat(str1, aux2);
				}
				else{
					strcpy(str1, aux2);
				}
			}
			else if(strcmp(aux1, "call")==0){
				int_to_char(count, counter);
				call(aux2, aux3, counter);
				strcpy(str1, aux2);
			}
			else if(strcmp(aux1, "function")==0){
				int_to_char(count, counter);
				function(aux2, aux3, counter);
				strcpy(str1, aux2);
			}
		}
	}

	//vm-command has 2 words
	else if(words(str1)==2){
		char aux1[ROW];
		char aux2[ROW];

		parse_elem(str1, aux, aux1);
		strcpy(str1, aux);
		parse_last_elem(str1, aux2);

		branching(aux1, aux2);
		strcpy(str1, aux1);
	}

	//vm-command has 1 word
	else if(words(str1)==1){
		char aux1[ROW];
		char counter[ROW];

		parse_last_elem(aux, aux1);

		if(strcmp(aux1, "return")==0){
			return_value(aux1);
			strcpy(str1, aux1);
		}

		else{
			int_to_char(count, counter);
			math(aux1, counter);
			strcpy(str1, aux1);
		}
	}
}

//parses element from vm-command 
void parse_elem(char *str1, char *str2, char *str3){
	while(*str1!=' '){
		*str3=*str1;
		str1++;
		str3++;
	}
	*str3='\0';
	str1++;
	while((*str1!='\0')&&(*str1!='/')&&(*str1!='\n')&&(*str1!='\r')&&(*str1!='\t')){
		*str2=*str1;
		str1++;
		str2++;
	}
	*str2='\0';
}

//parses last element from vm-command
void parse_last_elem(char *str1, char *str2){
	while((*str1!='\0')&&(*str1!='/')&&(*str1!='\n')&&(*str1!='\r')&&(*str1!='\t')&&(*str1!=' ')){
		*str2=*str1;
		str1++;
		str2++;
	}
	*str2='\0';
}

//parses name of file
void parse_name(char *str1, char *str2){
	while(*str1!='.'){
		*str2=*str1;
		str1++;
		str2++;
	}
	*str2='\0';
}

//translates push command into static segment memory while working with multiple files 
void push_static_mult(char *str1, char *str2, char *str3){
	strcpy(str3, "@");
	strcat(str3, str1);
	strcat(str3, ".");
	strcat(str3, str2);
	strcat(str3, "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
}

////translates pop command into static segment memory while working with multiple files 
void pop_static_mult(char *str1, char *str2, char *str3){
	strcpy(str3, "@");
	strcat(str3, str1);
	strcat(str3, ".");
	strcat(str3, str2);
	strcat(str3, "\nD=A\n@R13\nM=D\n@SP\nM=M-1\n@SP\nA=M\nD=M\n@R13\nA=M\nM=D\n");
}

//copes from one string into another
void copy(char *str1, char *str2){
	while(*str1!='@' && *str1!='('){
		str1++;
	}
	while(*str1!='\0'){
		*str2=*str1;
		str1++;
		str2++;
	}
	*str2='\0';
}

//checks if the file has .vm extension
int bool_vm(char *str){
	char check[3];
	strcpy(check, "vm");
	int i=0, ext=1;
	while(*str!='.'){
		str++;
	}
	str++;
	while(*str!='\0'){
		if(*str!=check[i]){
			ext=0;
		}
		str++;
		i++;
	}
	return ext;
}
