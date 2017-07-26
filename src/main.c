/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include "repl.h"
#include "ast.h"
#include "env.h"
#include "main.h"

struct m_action{
	int file;
	int shell;
	int exec;
	int stdin;

};
void log_d(char *s){
	if(!s) return;
	if(!LOG_DEBUG) return;
	printf("%s\n",s);
}
void usage()
{
	printf("bim v%s\n",VERSION);
	printf("\nUSAGE: ");
	printf("bim [options] [file]\n");
	printf("valid options:\n");
	printf("-i              --shell --interactive        interactive (shell mode)\n");
	printf("-h              --help                       show this help\n");
	printf("-f <file>       --file <file>                execute file\n");
	printf("-e '<cmd>'      --exec '<cmd>'               execute cmd commands\n");
	printf("-s              --stdin                      execute from stdin (pipe)\n");
	printf("-v              --version                    prints bim version number\n");
	exit(1);
}
void usage_b()
{
	printf("bim-%s (c) Fernando Iazeolla \n",VERSION);
	printf("for help type: bim --help\n");
}
void parse_args(int argc,char **argv,struct m_action *action,char **buff,char *filename)
{
	//char *s1;
	int c,len;
	action->shell=0;
	action->file=0;
	action->exec=0;
	action->stdin=0;
	while (1)
	{
		static struct option long_options[] =
		{
			{"file",required_argument,0,'f'},
			{"interactive",no_argument,0,'i'},
			{"shell",no_argument,0,'i'},
			{"stdin",no_argument,0,'s'},
			{"exec",required_argument,0,'e'},
			{"help",no_argument,0,'h'},
			{"version",no_argument,0,'v'},
			{0,0,0,0,}
			
		};
		int option_index = 0;
		c = getopt_long (argc, argv, "svhif:e:",long_options, &option_index);
		if (c == -1) break;
		switch(c)
		{
			case 'i':
				action->shell=1;
				break;
			case 'v':
				usage_b();
				exit(1);
				break;
			case 'f':
				action->file=1;
				strncpy(filename,optarg,FILENAME_LEN);
				break;
			case 's':
				action->stdin=1;
				*buff=(char*)malloc(MAX_CMD_LEN);
				if(!*buff) {fprintf(stderr,"error allocating mem..."); exit(1);}
				len=read(STDIN_FILENO,*buff,MAX_CMD_LEN);
				freopen("/dev/tty","rb",stdin); //reset stdin
				break;
			case 'e':
				action->exec=1;
				*buff=(char*)malloc(MAX_CMD_LEN);
				if(!*buff) {fprintf(stderr,"error allocating mem..."); exit(1);}
				strncpy(*buff,optarg,MAX_CMD_LEN);
				break;
			case 'h':
			case '?':
				usage();
				exit(1);
				break;
			default:
				usage_b();
				exit(1);
				break;
		}
		
	}
}
int main(int argc,char **argv)
{
	struct m_action action;
	char filename[FILENAME_LEN];
	char *buff=NULL;
	init_env();
	set_debug_var(DEBUG);
	parse_args(argc,argv,&action,&buff,&filename[0]);
	
	if(action.file){
		printf("file...%s\n",filename);
		run_script(filename);
		exit(0);
	}
	if(action.exec){
		printf("exec...%s\n",buff);
		run_exec(buff);
		free(buff);
		exit(0);
	}
	if(action.stdin){
		printf("stdin...\n");
		run_exec(buff);
		free(buff);
		exit(0);
	}
	//if(action.shell)
	//{
		usage_b();
		shell();
		printf("Bye.\n");
	//}
	//usage_b();
	return 0;
}
