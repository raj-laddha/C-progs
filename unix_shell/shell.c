#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LINE 80

struct hist_node {
	int cmd_num;
	char cmd[MAX_LINE];
};


struct hist_node history[10];
int front = -1;
int rear = -1;

void push_cmd(int num, char cmd[]) {
	int idx;
	if(front == -1) {
		idx = 0;
		front = 0;
		rear = 0;
	}
	
	else if((rear + 1) % 10 == front) {
		front = (front + 1) % 10;
		rear = (rear + 1) % 10;
		idx = rear;
	}
	
	else {
		rear++;
		idx = rear;
	}
	
	history[rear].cmd_num = num;
	strcpy(history[rear].cmd, cmd);
}

int search_cnum(int num, char cmd[]) {
	if(front == -1)
		return 0;
		
	int i = front;
	do {
		if(history[i].cmd_num == num) {
			strcpy(cmd, history[i].cmd);
			return 1;
		}
			
		i = (i + 1) % 10;
	} while( i != (rear + 1) % 10);
	
	return -1;
}

void history_cmd(int argc, char *args[]) {	
	int i;
	int valid = 1;
	if(argc == 1) {
		if(front == -1) {
			printf("No commands in history\n");
			return;
		}
		
		i = rear;
		while(i != front) {
			printf("%d %s\n", history[i].cmd_num, history[i].cmd);
			if(i == 0)
				i = 9;
			else
				i--;
		}
		printf("%d %s\n", history[i].cmd_num, history[i].cmd);		
	}
	
	else if(argc == 2) {
		if(args[1][0] == '!') {
			if(args[1][1] == '!') {
			
				if(front == -1) {
					printf("No commands in history\n");
					return;
				}
				
				printf("%s\n", history[rear].cmd);
			}
			
			else {
				for(i = 1; args[1][i] != '\0'; ++i)
					if(args[1][i] < '0' || args[1][i] > '9')
						valid = 0;
						
				if(valid) {
					char c, cmd[MAX_LINE];
					int num;
					sscanf(args[1], "%c%d", &c, &num);
					int status = search_cnum(num, cmd);
					
					if(status == 0) {
						printf("No commands in history\n");
					}
					
					else if(status == -1) {
						printf("No such command in history\n");
					}
					
					else {
						printf("%s\n", cmd);
					}
				}
			}
		}
		
		else 
			valid = 0;
	}
	
	else
		valid = 0;
	
	if(!valid)
		printf("Error: invalid command\n");	
}

int get_cmd(char *args[], char cmd[]) {
	char ch;
	char word[30];
	int argc = 0, i = 0, c = 0;
	while((ch = getchar()) == '\t' || ch == ' ');
	
	if(ch == '\n')
		return 0;
	
	word[i++] = ch;
 	cmd[c++] = ch;	
 	
	while((ch = getchar()) != '\n') {
	
		if(ch == ' ') {
			word[i] = '\0';
			args[argc] = (char *) malloc(sizeof(char) * strlen(word) + 1);
			strcpy(args[argc], word);
			i = 0;
			argc++;		
		}
		
		else {
			word[i++] = ch;
		}
		
		cmd[c++] = ch;
	}
	
	cmd[c] = '\0';
	word[i] = '\0';
	args[argc] = (char *) malloc(sizeof(char) * strlen(word) + 1);
	strcpy(args[argc], word);
	return argc + 1;
	
}

int main(void) {
	char *args[MAX_LINE/2 + 1];
	char cmd[MAX_LINE];
	int should_run = 1;
	int should_wait;
	int argc;
	int cmd_count = 0;
	pid_t pid;
	
	while(should_run) {
		should_wait = 1;
		printf("osh> ");
		fflush(stdout);
		
		argc = get_cmd(args, cmd);
		if(argc == 1 && strcmp(args[0], "exit") == 0) {
			should_run = 0;
			continue;
		}
		
		if(strcmp(args[argc - 1], "&") == 0) {
			free(args[argc - 1]);
			argc--;
			args[argc] = NULL;
			should_wait = 0;
		}
		
		else {
			args[argc] = NULL;
		}
		
		pid = fork();
		
		if(pid < 0) {
			perror("Error executing");
			continue;
		}
		
		else if(pid > 0) {
			int status;
			if(should_wait) {
				wait(&status);
				if(WIFEXITED(status) && !WEXITSTATUS(status)) {
					cmd_count++;
					push_cmd(cmd_count, cmd);
				}
			}
			
		}
		
		else {
			if(strcmp(args[0], "history") == 0) {
				history_cmd(argc, args);
				return 1;
			}
			
			if(execvp(args[0], args) < 0) {
				perror("Error");
				return 1;
			}
		}
		
	}
	
	return 0;
}
