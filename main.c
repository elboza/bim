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
#include "main.h"

struct m_action{
	int file;
	int shell;
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
	printf("-o '<file>'     --out '<file>'               output file\n");
	//printf("-x <file>       --execute <file>             execute file script\n");
	printf("-v              --version                    prints bim version number\n");
	exit(1);
}
void usage_b()
{
	printf("bim-%s (c) Fernando Iazeolla \n",VERSION);
	printf("for help type: bim --help\n");
}
void parse_args(int argc,char **argv,struct m_action *action)
{
	char *s1;
	int c;
	action->shell=0;
	action->file=0;
	while (1)
	{
		static struct option long_options[] =
		{
			{"out",required_argument,0,'o'},
			{"interactive",no_argument,0,'i'},
			{"shell",no_argument,0,'i'},
			{"help",no_argument,0,'h'},
			{"version",no_argument,0,'v'},
			{0,0,0,0,}
			
		};
		int option_index = 0;
		c = getopt_long (argc, argv, "vhio:",long_options, &option_index);
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
			case 'o':
				
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
	parse_args(argc,argv,&action);
	
	
	//if(action.shell)
	//{
	//	shell();
	//}
	
	usage_b();
	shell();
	
	printf("Bye.\n");
	return 0;
}
