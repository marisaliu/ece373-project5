#include "csapp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "card.h"
#include <ctype.h>

/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"

//////////////////////////////////////////////////////////////////
///////////////////Prompts for User Input/////////////////////////
/////////////////////////////////////////////////////////////////
char user_play(char* str)
{ 
	char input;
	int boolInHand = 0;
	char rank[(strlen(str)/2)+1];
	int x = 0;
	for(int i =17; i < strlen(str); i++){
		if(!isspace(str[i])){
			rank[x] = str[i];
			x++;
		}
	}
	while(boolInHand == 0)
	{
		printf("\nPlayer 1's turn, enter a Rank: ");
		scanf("%c", &input);
		while((getchar()) != '\n');
		for(int i = 0; i < strlen(rank); i++){
			if(rank[i]== toupper(input))
			{ 
				boolInHand = 1;
				return toupper(input);
			}
		}
		printf("Error - must have at least one card from rank to play");
	}
	return -1;
}


int main(int argc, char **argv) 
{
int index = 1;
    int clientfd;
    char *host, *port, buf[MAXLINE],buf2[MAXLINE];
    rio_t rio;
		int play = 1;
		int n;

    if (argc != 3) {
	fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	exit(0);
    }
    host = argv[1];
    port = argv[2];

    clientfd = Open_clientfd(host, port);
    rio_readinitb(&rio, clientfd);
		char inputRank;
//while play is true prompt the user for a rank and then display his hand and the books
    while (play) 
		{
//	    if(index%3 == 1){
//				buf[0] = '1';
	//			printf("--\n");
				rio_writen(clientfd, buf, 2); //writes/sends it to server
//printf("buf length: %d\n", strlen(buf));
//			  rio_readlineb(&rio,buf,MAXLINE);
				while((n=rio_readlineb(&rio,buf,MAXLINE))<2){
//					printf("received %d\n", n);
				//	rio_writen(clientfd, buf, 1);
				}
		//		strcpy(buf,"11");
		//		rio_writen(clientfd, buf, 2);
//				if(n>0)printf("after received %d\n", n);
//				printf("print out hand");
				Fputs(buf,stdout);
				fflush(stdout);
				memset(buf, 0, (strlen(buf)*sizeof(buf[0])));
//			}
//			else if(index%3 == 2){
			 	rio_writen(clientfd, buf2, 2); //writes/sends it to server
				while((n=rio_readlineb(&rio,buf2,MAXLINE))<2);
	     	Fputs(buf2,stdout);
				fflush(stdout);
				memset(buf2, 0, (strlen(buf)*sizeof(buf[0])));
//     		  }
//		  else{	
/*				inputRank = user_play(&buf);
				buf[0] = inputRank;
				rio_writen(clientfd, buf, strlen(buf)); //writes/sends it to server
				while(rio_readlineb(&rio, buf, MAXLINE)!=0);  //reads in from server
				Fputs(buf, stdout);*/
//			}
//			index++;
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}

