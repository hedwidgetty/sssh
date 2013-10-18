//Sarah Bass

//My shell...it's not great, but it does execute some basic commands.


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "linkedList.h"
#include "alias.h"
#define MOST 1000

int historyCmds(char** prePipe, Node* historyhead, int HISTORYCOUNT, int histSize);

int parsePipe(char * s, char **leftSide, char ** rightSide)
{

   int i=0, j=0, hasPipe=0;
   char cur;
   char first [MOST], second[MOST];
   cur=s[i];
   while(i<strlen(s)&&cur!='|')
   {
		       
       first[i]=cur;
       i++;
       cur=s[i];
   }
   if(cur=='|')
	hasPipe=1; 
   first[i]='\0';
   (*leftSide)=(char*)calloc(i+1, sizeof(char));
   strcpy(*leftSide, first);
   i++;
   if(hasPipe ==0)
	return -1;
   cur=s[i];
   while(i<strlen(s))
   {
		       
       second[j]=cur;
       j++;
       i++;
       cur=s[i];
   }
   second[j]='\0';
   (*rightSide)=(char*)calloc(j+1, sizeof(char));
   strcpy(*rightSide, second);
   

   return 1;

}// end parsePipe

void strip(char array[])
{
	int len = strlen(array);
	if(array[len - 1] =='\n')
	{
		array[len - 1] ='\0';
	}
}

int goAgain(char * s)
{
	char temp [100];
	strcpy(temp, s);
	
	if(strlen(temp)!=4)
		return 1;
	else
	{
		int i;
		for(i=0; i<4; i++)
			temp[i] = tolower(temp[i]);
	}
	if(strcmp("exit", temp)==0)
		return 0;	
	return 1;

}// end goAgain

void clean(int argc, char **argv)
{
	int i;
	

	for(i=0; i<argc; i++)
		free(argv[i]);
	free(argv);
	


}


int makeargs(char *s, char *** argv)
{
   if(strlen(s)==0)
   	return -1;
   int i=0, words=0, j=0;
   char cur;
	
   
   while(i<strlen(s))
   {
       cur=s[i];	       
       if(cur==' ')
	   words++;
       i++;
       
   }
   words=words+1;

   *argv=(char**)calloc(words+1, sizeof(char*));

   i=0, j=0;
   int wordLen=0, wordNum=0;

   do
   {
	cur=s[i];

		       
	if((cur==' '||i==strlen(s)))
        {
   
	   char word[wordLen+1];
	   int k;
	   for(k=0; k<wordLen+1; k++)
	   {
		if(k==wordLen)
		   word[k]='\0';
		else		
		   word[k]=s[j];
		j++;
	   }

	   (*argv)[wordNum]=(char*)calloc(strlen(word)+1, sizeof(char));

	   strcpy((*argv)[wordNum], word);
	  


	   wordNum++;
           j=i+1;
           wordLen=0;
	   
        }
        else
            wordLen++;
	
        i++;
        
        
   }while(i<=strlen(s));
  


   (*argv)[wordNum]= '\0';


 
   return words;
}
void noPipe(char** prePipe, char*** argvPre, int* argcPre, char** input, Node** head)
{
	int error=-99;
	int k;
	FILE* fin = NULL;
	FILE* fout = NULL;
	for(k=0; k<*argcPre; k++)
	{
		if(strcmp((*argvPre)[k], "$PATH")==0)
		{
			free((*argvPre)[k]);
			(*argvPre)[k]= (char*)malloc((strlen(getenv("PATH"))+1)*sizeof(char));			
			strcpy((*argvPre)[k], getenv("PATH"));
		}
	}

	//hasInRedirect(input, &fin);

	char temp [MOST];
	char end [MOST];
	int i, j, m;
	for(i=0; i!=strlen(*prePipe)&&(*prePipe)[i]!='<'; i++)
	{
		end[i]=(*prePipe)[i];

	}
	if(end[i-1]== ' ')
		m=i-1;
	else	
		m=i;
	if(i!=strlen(*prePipe))
	{	
		i++;
		if((*prePipe)[i]==' ')
			i++;	
		for(j=0; i<(strlen(*prePipe))&&(*prePipe)[i]!='>'; i++)
		{
			temp[j]=(*prePipe)[i];
			j++;
		}
		if(temp[j-1]==' ')
			temp[j-1]='\0';
		else
			temp[j]='\0';
		for(j=m; i<strlen((*prePipe)); i++, j++)
		{
			end[j]=(*prePipe)[i];
		}
		end[j]='\0';
		free(*prePipe);
		*prePipe= (char*)calloc((strlen(end)+1), sizeof(char));
		strcpy(*prePipe, end);
		
		fin=freopen(temp, "r", stdin);
	}
	clean(*argcPre, *argvPre);
	*argcPre =makeargs(*prePipe, argvPre);


	char temp2 [MOST];
	
	for(i=0; i!=strlen(*prePipe)&&(*prePipe)[i]!='>'; i++)
	{

	}
	

	if(i<strlen(*prePipe)-1)
	{	
		i++;
		if((*prePipe)[i]==' ')
			i++;
		for(j=0; i<(strlen(*prePipe)+1); i++)
		{
			temp2[j]=(*prePipe)[i];
			j++;
		}

		for(j=0; (*prePipe)[j]!='>'; j++)
		{
			end[j]=(*prePipe)[j];
			
		}
		if(end[j-1]==' ')
			end[j-1]='\0';
		else
			end[j]='\0';
		free(*prePipe);
		*prePipe= (char*)calloc((strlen(end)+1), sizeof(char));
		strcpy(*prePipe, end);
		
		fout=freopen(temp2, "w", stdout);
	}
	clean(*argcPre, *argvPre);
	*argcPre =makeargs(*prePipe, argvPre);
	
	
	
	error=execvp((*argvPre)[0], *argvPre);
	if(fin!=NULL)
	{
		fclose(fin);
	}
	if(fout!=NULL)
		fclose(fout);
	if(error!=-99)
	{
		printf("%s: cmd not found\n", (*argvPre)[0]);
		strcpy(*input,"exit");
	}

	int d;
	for(d=0; d<*argcPre; d++)
		printf("%s\n", (*argvPre)[d]);



}

void withPipe(char** prePipe, char** postPipe, char*** argvPre, char*** argvPost, int* argcPre, int* argcPost, char** input, Node** head)
{
	int fd[2];
	pipe(fd);
	if(fork()==0)
	{
		close(fd[0]);
		close(1);
		dup(fd[1]);
		close(fd[1]);
		noPipe(prePipe, argvPre, argcPre, input, head);
		

	}

	else
	{	
		close(fd[1]);
		close(0);
		dup(fd[0]);
		close(fd[0]);
		noPipe(postPipe, argvPost, argcPost, input, head);

	}


}

int getInput(char** input, FILE* fin)
{
	
	int index =0;
	char cur;
	
	cur = fgetc(fin);
	if(fin!=stdin && feof(fin))
	{
		return -1;
	}
	

	
	if(cur=='\n')
	{
		return 2;
	}
	while(cur!='\n')
	{
		
		if(cur!='='&& (cur!=' ' || (index>0 && (*input)[index-1]!=' ')))		
		{
			(*input)[index]=cur;
			index++;
		}
		else if(cur == '=')
		{
			if((*input)[index-1]!=' ')
			{
				(*input)[index]=' ';
				(*input)[index+1]='=';
				(*input)[index+2]= ' ';
				index+=3;
			}
			else
			{
				(*input)[index]=cur;
				(*input)[index+1]=' ';
				
				index+=2;
			}
		}
		
		cur = fgetc(fin);
		
	}
	if(index>0 && (*input)[index-1]==' ')
	{
		(*input)[index-1] = '\0';
		
	}
	else
	{
		(*input)[index] = '\0';
		
	}
	
	return 0;
}



void defineAlias(char* argv, Node** head, char* input)
{
	int i=6, j=0;
	char temp1 [MOST], temp2[MOST];
	while(i<strlen(argv)&&(argv[i]!='='))
	{
		if(argv[i]!='\''&&argv[i]!='\"')
		{		       
			temp1[j]=argv[i];
			j++;
			
		}
		i++;
		
	}
	
	if(argv[i]!='=')
	{
		printf("%s: not found\n", argv);
		return;
	}
	if(argv[i-1]==' ')
		temp1[j-1]='\0';
	else
		temp1[j]='\0';


	i++;
	j=0;
	if(argv[i]==' ')
		i++;
	while(i<strlen(argv))
	{
		if(argv[i]!='\''&&argv[i]!='\"')
		{		       
			temp2[j]=argv[i];
			j++;
			
		}
		i++;
		
	}
	temp2[j]='\0';
	
	
		
		
	
	
	Node* oldAlias=findAlias(head, temp1);
	
	if(oldAlias!=NULL)
	{			
		
		deleteNode(temp1, head);
	}
	Alias* a= createAlias(temp2, temp1);	
	oldAlias=createNode(a);
	addOrdered(oldAlias, head);
	
	
	//strcpy(input, "exit");
	

}

void isAlias(char** potentialAlias, Node* head)
{
	Node* cur=head;
	
	while(cur!=NULL)
	{
		if(strcmp(cur->data->als, *potentialAlias)==0)
		{
			free(*potentialAlias);
			(*potentialAlias)=(char*)calloc(strlen(cur->data->cmd)+1, sizeof(char));
			strcpy(*potentialAlias, cur->data->cmd);
		}
		cur=cur->next;
		
	}
	
		


}

void removeAlias(char* prePipe, Node** head, char* input)
{
	char temp [MOST];
	int i=8, j=0;
	while(i<strlen(prePipe))
	{
		temp[j]= prePipe[i];
		i++;
		j++;
	}
	temp[j]= '\0';
	deleteNode(temp, head);
}


void changePath(char * prePipe)//linux/limits.h  PATH_MAX
{
	char temp [MOST], temp2[MOST];
	char PATH [MOST];
	strcpy(temp, prePipe);
	strtok(temp, "$PATH:");
	int j = strlen(temp)+5;
	//printf("%d\n", j);
	
	
	int i;
	for(i=0; j<(strlen(prePipe)+1); i++)
	{
		temp[i]=prePipe[j];
		j++;
	
	}
	strcpy(temp2, (getenv("PATH")));
	
	strcpy(PATH, strcat(temp2, temp));
	
	int error;
	error=setenv("PATH", PATH, 1);
	if(error!=0)
		printf("Invalid path\n");
	
	
}


void changeDir(char* prePipe, char** argvPre)
{
	//char CWD [MOST];
	//getcwd(CWD, MOST);
	int error;
	if(strcmp(prePipe, "cd")==0)
	{
		error=chdir("/home");
	}

	else
	{
				
		error=chdir(argvPre[1]);
		
	}
	if(error!=0)
	{
		perror("invalid path");
	}
	
	
	

}

void executeInput(Node** head, char** input, Node* history_head, int* HISTORYCOUNT, int* usedHist, int* histSize)
{

	
	char * prePipe = NULL, * postPipe = NULL;
	char **argvPre = NULL, **argvPost=NULL;
	int res, argcPre, argcPost, status;
	
		
	if(strcmp(*input, "history")==0)
	{
		if(history_head==NULL)
			printf("no history\n");		

		Node* cur=history_head;
		int i;
		for(i=0; i<(*histSize-*HISTORYCOUNT); i++)
			cur=cur->next;
		while(cur!=NULL)
		{
			printf("%d %s\n", i, cur->data->cmd);
			cur=cur->next;
			i++;
		}
		
	}
	
		
	else if(strcmp(*input, "alias")==0)
	{
		printList(*head);
	}
	
	else
	{
		res = parsePipe(*input, &prePipe, &postPipe);
		isAlias(&prePipe, *head);
		if(strcmp(prePipe, "!!")==0||prePipe[0]=='!')
		{
			if(histSize==0)
			{
				printf("invalid cmd: no history\n");
				return;
			}
			else
			{
				int error = historyCmds(&prePipe, history_head, *HISTORYCOUNT, *histSize);
				if(error==1)
				{
					printf("invalid cmd: not that much history\n");
					return;
				}
			}
			*usedHist=1;
		}	
	
		else if(strcmp(*input, "exit")!=0)
		{
		
		
			argcPre =makeargs(prePipe, &argvPre);
		
			if(strcmp(argvPre[0], "alias")==0)
			{
				defineAlias(prePipe, head, *input);
			
			}
			else if(strcmp(argvPre[0], "HISTORYCOUNT")==0)
			{			
				(*HISTORYCOUNT)=atoi(argvPre[2]);
			
			}
			else if(strcmp(argvPre[0], "PATH")==0)
			{
				changePath(prePipe);
				
			}
			
		
			else if(strcmp(argvPre[0], "cd")==0)
			{
				changeDir(prePipe, argvPre);

			}
		
		
			else if(strcmp(argvPre[0], "unalias")==0)
			{
				removeAlias(prePipe, head, *input);
			}
			else 
			{
							
				if(res==1)
				{
					isAlias(&postPipe, *head);
					if(strcmp(postPipe, "!!")==0||postPipe[0]=='!')
					{
						historyCmds(&postPipe, history_head, *HISTORYCOUNT, *histSize);
					}
					argcPost =makeargs(postPipe, &argvPost);
				}
				if(fork()==0)
				{								
				
									
				
					if(res == 1)
					{
						withPipe(&prePipe, &postPipe, &argvPre, &argvPost, &argcPre, &argcPost, input, head);	
					}
					else
					{
						noPipe(&prePipe, &argvPre, &argcPre, input, head);				
					}				
				}
				waitpid(-1, &status, 0);
				
			
			}
			clean(argcPre, argvPre);
			if(res==1)
			{
				free(postPipe);
				clean(argcPost, argvPost);
			}
		
								
		}
		free(prePipe);
	}
	
}




void getFileInput(char** input, FILE* fin, Node** head, Node* history_head, int* HISTORYCOUNT, int* usedHist, int* histSize)
{
	int end;
	end=getInput(input, fin);
	
	while(end!=-1 && strcmp(*input, "exit")!=0)
	{
		if(end==0)		
			executeInput(head, input, history_head, HISTORYCOUNT, usedHist, histSize);
		end=getInput(input, fin);
		

	}
	fclose(fin);
}

void getHistInput(char** input, FILE* finHist, Node** history_head, int* histSize)
{
	
	int end;
	end =getInput(input, finHist);
	
	while(end!=-1)
	{
		if(end!=2)		
		{
			Alias* a=createAlias(*input, " ");
			Node* n = createNode(a);
			addLast(n, history_head);
			(*histSize)++;
			
		}
		end=getInput(input, finHist);

	}
	


}


void keepHistory(FILE* fin, char* input, int* histSize, Node** history_head)
{
		
	Alias* a=createAlias(input, " ");
	Node* n=createNode(a);
	addLast(n, history_head);
	fprintf(fin,"%s\n", input);
	(*histSize)=(*histSize)+1;
	
	
}

int historyCmds(char** prePipe, Node* historyhead, int HISTORYCOUNT, int histSize)
{
		
	if(strcmp(*prePipe, "!!")==0)
	{
		strcpy(*prePipe, (historyhead->data->cmd));
	}
	else
	{
		char number [MOST];
		int i=1;
		int num;
		while(i<(strlen(*prePipe)+1))
		{
			number[i-1]=(*prePipe)[i];
			i++;
		}
		num=atoi(number);
		if(num>histSize)
			return 1;
		Node* cur = historyhead;
		for(i=histSize; i>num; i--)
			cur=cur->next;
		strcpy(*prePipe, (cur->data->cmd));
		
	}
	return 0;
			
}


int main()
{
	
	char * input = NULL;
	Node * head = NULL;
	Node * history_head = NULL;
	int HISTORYCOUNT=0;
	int histSize=0;
	int usedHist=1;
	
	input = (char*)malloc(MOST* sizeof(char));
	input[0]='-';
	
	FILE * fin=NULL;
	FILE * finHist=NULL;
	finHist=fopen(".sssh_history", "rw");
	fin = fopen(".ssshrc", "r");
	getFileInput(&input, fin, &head, history_head, &HISTORYCOUNT, &usedHist, &histSize);
	getHistInput(&input, finHist, &history_head, &HISTORYCOUNT);
	input[0]='-';
	
	
	


	usedHist=0;
	while(input[0]=='-'||goAgain(input))
	{	
			
		printf("?:  ");

		getInput(&input, stdin);
		executeInput(&head, &input, history_head, &HISTORYCOUNT, &usedHist, &histSize);
		if(usedHist==0 && (strcmp(input, "exit")!=0))
		{
			keepHistory(finHist, input, &histSize, &history_head);	
		}	
		usedHist=0;
	}
	
	
	clearList(&head);
	clearList(&history_head);
	free(input);
	fclose(finHist);

	return 0;
}

