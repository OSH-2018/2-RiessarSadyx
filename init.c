#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
	int argc;
	char orgcmd[4096],cmd[4096];
	char *args[128];
	char user[256],host[256],cwd[256];
	
	while(1)
	{
		//strcpy(user,"root");
		//gethostname(host,256);
		//getcwd(cwd,256);
		//printf("%s@%s:%s# ",user,host,cwd);
		printf("# ");
		
		fflush(stdin);
		fgets(orgcmd,4096,stdin);
		for(int i=0;i<4096;i++)cmd[i]=0;
		for(int i=0,j=0;orgcmd[i];)
		{
			if(orgcmd[i]=='|')
			{
				cmd[j++]=' ';
				cmd[j++]=orgcmd[i++];
				cmd[j++]=' ';
			}
			else if(orgcmd[i]=='>' && orgcmd[i+1]=='>')
			{
				cmd[j++]=' ';
				cmd[j++]=orgcmd[i++];
				cmd[j++]=orgcmd[i++];
				cmd[j++]=' ';
			}
                        else if(orgcmd[i]=='>')
                        {
                                cmd[j++]=' ';
                                cmd[j++]=orgcmd[i++];
                                cmd[j++]=' ';
                        }
                        else if(orgcmd[i]=='<' && orgcmd[i+1]=='<')
                        {
                                cmd[j++]=' ';
                                cmd[j++]=orgcmd[i++];
                                cmd[j++]=orgcmd[i++];
                                cmd[j++]=' ';
                        }
                        else if(orgcmd[i]=='<')
                        {
                                cmd[j++]=' ';
                                cmd[j++]=orgcmd[i++];
                                cmd[j++]=' ';
                        }
			else cmd[j++]=orgcmd[i++];
		}
		//printf("%s\n",cmd);
		
		argc=0;
		for(int i=0;i<128;i++)args[i]=NULL;
		for(int l=0;cmd[l];)
		{
			for(;cmd[l] && isspace(cmd[l]);l++)cmd[l]='\0';
			if(cmd[l])args[argc++]=cmd+l;
			for(;cmd[l] && !isspace(cmd[l]);l++);
		}
		//for(int i=0;i<argc;i++)printf("%s\n",args[i]);
		
		if(!args[0])continue;
		if(!strcmp(args[0],"cd"))
		{
			if(args[1])chdir(args[1]);
			continue;
		}
		if(!strcmp(args[0],"pwd"))
		{
			printf("%s\n",cwd);
			continue;
		}
		if(!strcmp(args[0],"echo"))
		{
			for(int i=1;i<argc;i++)
			{
				if(i>1)printf(" ");
				if(args[i][0]=='$')printf("%s",getenv(args[i]+1));
				else printf("%s",args[i]);
			}
			printf("\n");
			continue;
		}
		if(!strcmp(args[0],"export"))
		{
			for(int i=1;i<argc;i++)
			{
				int j;
				for(j=0;args[i][j] && args[i][j]!='=';j++);
				if(args[i][j]=='=')
				{
					args[i][j]='\0';
					setenv(args[i],args[i]+j+1,1);
				}
				else break;
			}
			continue;
		}
		if(!strcmp(args[0],"exit"))return 0;
		
		pid_t pid=fork();
		if(pid==0)
		{
			execvp(args[0],args);
			return 255;
		}
		wait(NULL);
	}
	return 0;
}
