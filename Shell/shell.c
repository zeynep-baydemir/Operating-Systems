#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define maxCom 100
#define maxNum 10

//function to print username >>> and take input from user with getline function.
char* getInput(){
	char *username = getenv("USER");
	size_t n = 50;
	char *buf = malloc(sizeof(char)*n);
	printf( "%s%s", username, " >>> " );
	getline(&buf, &n, stdin);
	return buf;
}

//function to parse spaces.
void parseSpace(char* notParsed, char **parsed){
	size_t n = 50;
	notParsed = strsep(&notParsed,"\n"); 
    for(int i = 0; i<maxNum; i++){
		parsed[i] = strsep(&notParsed," ");
		if (parsed[i] == NULL)
			break;
	}
}
//function to execute commands with fork and execvp functions.
//listdir, mycomputername, whatsmyip, printfile file1 > file2, and hellotext.
int executeCommands (char** parsed){
	char* command = parsed[0];
	pid_t pid;
	pid = fork(); //creates child process
	if(pid<0)
		return 1;
	if(pid==0){
		if(strcmp(command,"listdir")==0){
			execvp("ls", parsed); // ls command
		}
		else if(strcmp(command,"mycomputername")==0){
			execvp("hostname", parsed); // hostname command
		}
		else if(strcmp(command,"whatsmyip")==0){
			parsed[1] = "-i";
			execvp("hostname", parsed); //hostname -I command
		}
		else if(strcmp(command,"printfile")==0){
			parsed[2] = parsed[3]; // deletes > symbol.
			parsed[3] = NULL;
			execvp("cp",parsed); // cp command
			
		}	
		else if(strcmp(command,"hellotext")==0){
			execvp("gedit",parsed); // gedit command
		}
		
	}
	else{
		wait(NULL); // waits for child process.
		return 0;
	}

}


int main(int argc, char *argv[]) {
	size_t n = 50;
	size_t maxhistory = 16;
	char *command = malloc(sizeof(char)*n);
	char **parsed = malloc(sizeof(char*)*n);
	char **history = malloc(sizeof(char*)*maxhistory); // creates history array for dididothat command
	int historyPos = 0;
	while(1){
		command = getInput(); // gets input from user
		char* copyCom = malloc(sizeof(char)*n); // copies command to put it in history 
		strcpy(copyCom,command);
		char* noQuote=malloc(sizeof(char)*n); // for deleting quotes from command for dididothat command
		strcpy(noQuote,command);
		history[historyPos] = strsep(&copyCom,"\n"); // adds command to history without newline
		historyPos++;
		if(historyPos > 15){ // for keeping 15 commands in history
			historyPos = 0;
		}
		
		parseSpace(command, parsed); // parses command
		char* sep = strsep(&noQuote, "\""); //deletes quotes
		sep = strsep(&noQuote,"\"");
		if (strcmp(command,"exit")==0){ // exits process
			break;
		}else if(strcmp(command,"dididothat")==0){
			for(int i = 0 ;i<15;i++){
				if(history[i] == NULL){ // if nothing is in history
					printf("No\n");
					break;
				}
				if(strcmp(sep,history[i]) == 0){ // checks whether command is in history
					printf("%s","Yes\n");
					break;
				}else {
					if(i==14) // end of history and no match
					printf("%s","No\n");
				}
			}
		}
		else if(strcmp(command,"printfile")==0){ // if command is printfile
			if(parsed[2]==NULL ){ // if command is printfile file
				FILE *file;
				size_t n = 150;
				char* line = malloc(sizeof(char)*n);;
				file = fopen(parsed[1], "r");
				int i = 0;
				char* str;
				char enter;
				while(getline(&line, &n, file)!=-1){  // while file has lines
					str = strtok(line, "\n");  //deletes newlines from line 
					if(strcmp(line,"\n")==0)   //if line is only newline
					str = "";
					printf("%s",str);
					enter = getc(stdin); // waits for enter
					i++;
				}
				fclose(file);
				free(line);
			}
			else{
				executeCommands(parsed); // if command is printfile file1 > file2
			}
		}			
		else {
			executeCommands(parsed); // if command is listdir | whatsmyip | mycomputername | hellotext
		}
	}
	return 0;
}
