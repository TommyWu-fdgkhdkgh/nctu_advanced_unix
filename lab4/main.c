#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *help_str = "- break {instruction-address}: add a break point\n" \
		"- cont: continue execution\n" \
		"- delete {break-point-id}: remove a break point\n" \
		"- disasm addr: disassemble instructions in a file or a memory region\n" \
		"- dump addr [length]: dump memory content" \
		"- exit: terminate the debugger\n" \
		"- get reg: get a single value from a register\n" \
		"- getregs: show registers\n" \
		"- help: show this message\n" \
		"- list: list break points\n" \
		"- load {path/to/a/program}: load a program\n" \
		"- run: run the program\n" \
		"- vmmap: show memory layout\n" \
		"- set reg val: get a single value to a register\n" \
		"- si: step into instruction\n" \
		"- start: start the program and stop at the first instruction\n";

#define MAXARGS 10

void parse_argv(char *buf_ptr, char *argv[], int *argc){

	char *tail_ptr = buf_ptr;
	char *head_ptr = buf_ptr;

	int state = 0;
	while(1){

		//處理連續很多空白的情況
		if(tail_ptr!=buf_ptr && (*(tail_ptr-1))==0 && (*tail_ptr)==' '){
			*tail_ptr=0;
			head_ptr=tail_ptr+1;
			tail_ptr++;
			continue;
		}

		//把一個個參數丟進argv
		if((*tail_ptr)=='\n' || (*tail_ptr)==' '){
			argv[(*argc)] = head_ptr;
			*argc = (*argc)+1;
			head_ptr = tail_ptr+1;
		}else if((*tail_ptr)==0){
			if((*(tail_ptr-1))==0){
				break;	
			}

			argv[(*argc)] = head_ptr;
			*argc = (*argc)+1;
			break;
		}

		//把一個個token中間的'\n'用0x00切斷
		if((*tail_ptr)=='\n' || (*tail_ptr)==' '){
			*tail_ptr = 0;
		}else if((*tail_ptr)==0){
			break;
		}
		tail_ptr++;
	}

}


int main(){


	char buf[1000];
	char *argv[MAXARGS];
	int argc;
	
	//main loop
	while(1){
		argc = 0;

		printf("sdb>");
			
		int buf_i = 0; 
		char c;
		while(1){
			c = getchar();
			buf[buf_i] = c;
			if(buf_i==800 || buf[buf_i]=='\n'){
				if(buf_i==800){
					buf[buf_i]='\n';
				}
				break;
			}
			buf_i++;
		}
		while(1){
			if(buf[buf_i]=='\n'){
				buf[buf_i] = 0;
				break;
			}
			buf_i++;
		}

		char *buf_ptr = buf;

		parse_argv(buf_ptr, argv, &argc);	
		
		
		/*
		printf("argc:%d\n", argc);
		for(int i=0;i<argc;i++){
			printf("%d : %s\n", i, argv[i]);
		}
		*/

		if(strcmp(argv[0], "exit")==0){
			return 0;
		}else if(strcmp(argv[0], "q")==0){
			return 0;
		}else if(strcmp(argv[0], "help")==0){
			printf("%s", help_str);
		}else{
			printf("unknown command!\n");
		}
	}

	return 0;
}
