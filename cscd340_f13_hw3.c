#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include "linkedList.h"
#include "alias.h"
#define MOST 1000



int parsePipe(char * s, char **leftSide, char ** rightSide)
{

   int i=0, words=0, j=0, hasPipe=0;
   char cur;
   char* sInput;
   char first [100], second[100];
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
	if(array[len-1]=='\n')
		array[len-1]='\0';
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
			temp[i]=tolower(temp[i]);
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
       if(cur==' '||cur == '=')
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

		       
	if(cur==' '||cur == '='||i==strlen(s))
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
void noPipe(char** argvPre, int argcPre, char** input, Node** head)
{
	int error=-99;
	int i;
	for(i=0; i<argcPre; i++)
	{
		if(strcmp(argvPre[i], "$PATH")==0)
		{
			free(argvPre[i]);
			argvPre[i]= (char*)malloc((strlen(getenv("PATH"))+1)*sizeof(char));			
			strcpy(argvPre[i], getenv("PATH"));
		}
	}
	error=execvp(argvPre[0], argvPre);
	if(error!=-99)
	{
		printf("%s: cmd not found\n", argvPre[0]);
		strcpy(*input,"exit");
	}



}

void withPipe(char** argvPre, char** argvPost, int argcPre, int argcPost, char** input, Node** head)
{
	int fd[2];
	pipe(fd);
	if(fork()==0)
	{
		close(fd[0]);
		close(1);
		dup(fd[1]);
		close(fd[1]);
		noPipe(argvPre, argcPre, input, head);
		

	}

	else
	{	
		close(fd[1]);
		close(0);
		dup(fd[0]);
		close(fd[0]);
		noPipe(argvPost, argcPost, input, head);

	}


}

void getInput(char** input)
{
	
	int index =0;
	char cur;
	
	cur = getchar();
	while(cur!='\n')
	{
		(*input)[index]=cur;
		index++;
		cur = getchar();
	}
	(*input)[index] = '\0';
}



void defineAlias(char* argv, Node** head, char* input)
{
	int i=6, j=0;
	char cur;
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


void changePath(char * prePipe)
{
	char temp [MOST];
	char * PATH = (char*)malloc(MOST* sizeof(char));
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
	//printf("%s\n", temp);
	
	strcpy(PATH, strcat(getenv("PATH"), temp));
	
	int error;
	error=setenv("PATH", PATH, 1);
	//printf("%d\n", error);
	if(error==-1)
		printf("Invalid path\n");
	free(PATH);
}

void changeDir(char* prePipe)
{
	char CWD [MOST];
	getcwd(CWD, MOST);
	printf("%s\n", CWD);
	if(strcmp(prePipe, "cd")==0)
	{
		chdir("/home");
	}
	getcwd(CWD, MOST);
	printf("%s\n", CWD);


}

void executeInput(Node** head, char** input)
{

	int status;
	
	char * prePipe = NULL, * postPipe = NULL;
	char **argvPre = NULL, **argvPost=NULL;
	int res, argcPre, argcPost;
			
		
		if(strcmp(*input, "alias")==0)
		{
						
			printList(*head);
		}
		
		
			
		
		else if(strcmp(*input, "exit")!=0)
		{
			res = parsePipe(*input, &prePipe, &postPipe);
			isAlias(&prePipe, *head);
			
			argcPre =makeargs(prePipe, &argvPre);
			if(strcmp(argvPre[0], "alias")==0)
			{
				defineAlias(prePipe, head, *input);
				
			}
			else if(strcmp(argvPre[0], "PATH")==0)
			{
				changePath(prePipe);
					
			
			}

			else if(strcmp(argvPre[0], "cd")==0)
			{
				changeDir(prePipe);

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
					argcPost =makeargs(postPipe, &argvPost);
				}
				if(fork()==0)
				{								
					if(res == 1)
					{
						withPipe(argvPre, argvPost, argcPre, argcPost, input, head);	
					}
					else
					{
						noPipe(argvPre, argcPre, input, head);				
					}				
				}
				waitpid(-1, &status, 0);
				
				
			}
			free(prePipe);
			clean(argcPre, argvPre);
			if(res==1)
			{
				free(postPipe);
				clean(argcPost, argvPost);
			}
			
									
		}
}

void getFileInput(char** input, FILE* fin, Node** head)
{
	
	fgets(*input, MOST, fin);
	
	while(!feof(fin)&&strcmp(*input, "exit")!=0)
	{
		strip(*input);
		executeInput(head, input);
		fgets(*input, MOST, fin);

	}
	fclose(fin);
}


int main()
{
	
	char * input = NULL;
	Node * head=NULL;
	
	input = (char*)malloc(MOST* sizeof(char));
	
	FILE * fin=NULL;
	
	fin = fopen(".ssshrc", "r");
	getFileInput(&input, fin, &head);
	/*if(strcmp(input, "exit"))
		exit(1);*/

	while(input==NULL||goAgain(input))
	{		
		printf("?:  ");
		getInput(&input);
		executeInput(&head, &input);	
		
	}
	
	clearList(&head);
	free(input);



	return 0;
}

