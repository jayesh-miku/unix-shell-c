#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>
#include<sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 100

void sigchld_handler(int sig){
	while(waitpid(-1, NULL, WNOHANG)> 0);
}

int main(){
	char line[MAX_LINE];
	char *args[MAX_ARGS];
	signal(SIGINT, SIG_IGN);
	signal(SIGCHLD, sigchld_handler);
	while(1){
		int background = 0;
		printf("myshell> ");
		fflush(stdout);

		if(fgets(line, sizeof(line), stdin)== NULL){
			printf("\n");
			break;
		}

		line[strcspn(line, "\n")] = 0;

		if(strcmp(line, "exit")== 0)
		{
			break;
		}

		int len = strlen(line);

		while (len > 0 && line[len-1] == ' ') {
    		line[len-1] = '\0';
    		len--;
		}

		if (len > 0 && line[len-1] == '&') {
    		background = 1;
    		line[len-1] = '\0';
		}
		while (len > 0 && line[len-1] == ' ') {
    		line[len-1] = '\0';
    		len--;
			}



		char *pipe_pos = strchr(line, '|');

		if(pipe_pos!=NULL)
		{
			*pipe_pos = '\0';
			char *cmd1 = line;
			char *cmd2 = pipe_pos +1;
			
			while(*cmd2 ==' ')cmd2++;
			char *args1[100];
			int i = 0;
			char *t = strtok(cmd1, " ");
			while(t){
			args1[i++] = t;
			t = strtok(NULL, " ");

			}
			args1[i] = NULL;

			char *args2[100];
			i = 0;
			t = strtok(cmd2, " ");
			while(t){
				args2[i++] =t;
				t = strtok(NULL, " ");
			}
			args2[i] = NULL;
			int fd[2];
			pipe(fd);

			pid_t p1 = fork();
			if(p1 ==0){
				signal(SIGINT, SIG_DFL);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				execvp(args1[0], args1);
				perror("exec1");
				exit(1);
			}
			pid_t p2 = fork();
			if(p2 ==0){
				signal(SIGINT, SIG_DFL);
				dup2(fd[0], STDIN_FILENO);
				close(fd[1]);
				close(fd[0]);
				execvp(args2[0], args2);
				perror("exec2");
				exit(1);
			}
			close(fd[0]);
			close(fd[1]);
			if(!background)
			{
			wait(NULL);
			wait(NULL);
			}
			continue;
		}


		if(strncmp(line,"cd ",3)==0)
		{
			char *path = line + 3;
			if(chdir(path)!=0)
			{
				perror("cd failed");
			}
			continue;
		}

		if(strcmp(line,"cd")==0)
		{
			chdir(getenv("HOME"));
			continue;
		}



		pid_t pid = fork();

		if(pid < 0){
			perror("fork failed");
			continue;
		}

		if(pid == 0)
		{
		signal(SIGINT, SIG_DFL);
		char *out = strchr(line, '>');
		if(out)
		{
			*out = '\0';
			char* filename = out+1;
			while(*filename == ' '){filename++;}
			int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC , 0644);
			if(fd<0)
			{
				perror("opening the file failed");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}

		char * in = strchr(line, '<');
		if(in)
		{
			*in = '\0';
			char *filename = in+1;
			while(*filename==' '){filename++;}
			int fd = open(filename, O_RDONLY);
			if(fd<0){
				perror("cant open the file");
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}

		int i = 0;
		char *token = strtok(line, " ");
		while(token != NULL){
			args[i++] = token;
			token = strtok(NULL, " ");
		}
		args[i] = NULL;

			execvp(args[0], args);
			perror("exec failed");
			exit(1);
		}else{
			if(!background){
			wait(NULL);
			}
		}
		}
		return 0;
}
