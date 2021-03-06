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
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;
		int play = 1;
		char hand[MAXLINE];
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
			while(rio_readlineb(&rio,buf,150)<2);
		  if(strcmp(buf,"10") == 0){
				while(rio_readlineb(&rio, buf, 150)<2);
				strcpy(hand,buf);
				printf("\n");
				Fputs(hand,stdout);
				printf("\n");
				while(rio_readlineb(&rio, buf, 150)<2);
				Fputs(buf, stdout);
				printf("\n");
			}
			else if(strcmp(buf,"2") == 0){
				inputRank = user_play(hand);
				sprintf(buf, "%c%c%c", inputRank, inputRank, inputRank);
				rio_writen(clientfd, buf,	4); //writes/sends it to server
				printf("\n");
			}
			else if(strcmp(buf,"3") == 0){
				Fgets(buf, MAXLINE, stdin); 
				Rio_writen(clientfd, buf, strlen(buf));
				if(strcmp(buf,"N\n") == 0 || strcmp(buf,"n\n") == 0){
					play = 0;
					exit(0);
				}
			}
		 else{
			 if(strlen(buf) > 1){
					Fputs(buf,stdout);
			 }	
		}
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}

