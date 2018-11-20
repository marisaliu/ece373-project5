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

int main(int argc, char **argv) 
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;
		int play = 1;

    if (argc != 3) {
	fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	exit(0);
    }
    host = argv[1];
    port = argv[2];

    clientfd = Open_clientfd(host, port);
    struct hand* hand = Rio_readinitb(&rio, clientfd);

		//while play is true prompt the user for a rank and then display his hand and the books
    while (play) {
			buf = user_play(&hand);
			Rio_writen(clientfd, buf, strlen(buf)); //writes/sends it to server
			Rio_readlineb(&rio, buf, MAXLINE);  //reads in from server
			display_hand(&hand);
			Fputs(buf, stdout);
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}

//////////////////////////////////////////////////////////////////
///////////////////Prompts for User Input/////////////////////////
/////////////////////////////////////////////////////////////////
char user_play(struct hand* temp)
{ 
  char input;
  int boolInHand = 0;
  while(boolInHand == 0)
  {
    printf("\nPlayer 1's turn, enter a Rank: ");
    scanf("%c", &input);
    while((getchar()) != '\n');
    while(temp != NULL)
    {
      if(temp->top.rank== toupper(input))
      { 
        boolInHand = 1;
        return toupper(input);
      }
      temp = temp->next;
    }
    printf("Error - must have at least one card from rank to play");
  }
}


