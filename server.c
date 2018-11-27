#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "deck.h"
#include "player.h"
#include <ctype.h>
#include "csapp.h"

  char bookAdded;
 // char inputRank;
  int transferCards;
  int play = 1; //1 true, 0 false
  int initTurn;
	int turn = 1; //1 true, 0 false
  int win = 0;  //1 true, 0 false
  int playAgain;
  struct card* nextCard;
  struct hand* temp;


void *thread(void *vargp);

void gofish(int connfd)
{
  int n;
  char buf[MAXLINE]; 
  char inputRank[MAXLINE];
  char *tempStr;
  size_t sz;
  rio_t rio;
  rio_readinitb(&rio, connfd);
	/*    while((n = Rio_readlineb(&rio, inputRank, MAXLINE)) != 0) {
        printf("server received %d bytes\n", n);
        Rio_writen(connfd, inputRank, n);
    }
*/
if(initTurn==1){
		initTurn=0;
		strcat(buf, display_hand(&user));                          //Display player 1's hand
		rio_writen(connfd, buf, strlen(buf)+1);
		memset(buf, 0, (strlen(buf)+1)*sizeof(buf[0]));
		while((Rio_readlineb(&rio, NULL, 1)) != 0);
		strcat(buf, display_book(&user,1));                        //Display player 1's book 
		strcat(buf, display_book(&computer,2));                    //Display user 1's book
		rio_writen(connfd, buf, strlen(buf)+1);
		memset(buf, 0, (strlen(buf)+1)*sizeof(buf[0]));
		while((Rio_readlineb(&rio, NULL, 1)) != 0);
}	
/////////////////////Player 1's Turn///////////////////////////////////
  strcat(buf, display_hand(&user));                          //Display player 1's hand
rio_writen(connfd, buf, strlen(buf)+1);
memset(buf, 0, (strlen(buf)+1)*sizeof(buf[0]));
//	strcat(buf, display_book(&user,1));                        //Display player 1's book 
//  strcat(buf, display_book(&computer,2));                    //Display user 1's book
//rio_writen(connfd, buf, strlen(buf)+1);
//memset(buf, 0, (strlen(buf)+1)*sizeof(buf[0]));

/*  if(user.hand_size == 0){                      //If player's hand is empty, player will draw a card and end their turn
	  nextCard = next_card();
	  add_card(&user,nextCard);
	  if(add_card(&user,nextCard) != 0) return -1;
	  sz = snprintf(NULL, 0,"\n  - Go Fish, Player 1 draws %c%c,", nextCard->rank, nextCard->suit);
    tempStr = (char *)malloc(sz + 1);
    snprintf(tempStr, sz+1, "\n  - Go Fish, Player 1 draws %c%c,", nextCard->rank, nextCard->suit);
	  strcat(buf, tempStr);
		free(tempStr);
  } 
	else{
	  while((n = rio_readlineb(&rio, inputRank, MAXLINE)) != 0) {
      rio_writen(connfd, inputRank, n);
    }  
	  temp = copy_hand_list(&user);
    transferCards = search(&computer, inputRank[0]); //Check player 2's hand to see if they have that rank
     /////////////If they have the rank transfer the cards///////////////////////////////////  
	  if(transferCards == 1){          
			strcat(buf, "  - Player 2 has"); 
			transfer_cards(&computer, &user, inputRank[0]);
  	  if(transfer_cards(&computer, &user, inputRank[0]) < 0) return -1;
      bookAdded = check_add_book(&user, inputRank[0]);
      if(bookAdded != 0){
				print_book_match(bookAdded,temp,1);
        win = game_over(&user);
        if(win == 1) return;
	    }
	    strcat(buf, "\n  - Player 1 gets another turn");
	   //send client hand again before restarting function
	    rio_writen(connfd, buf, strlen(buf)+1);
			strcpy(inputRank, display_hand(&user));
	    rio_writen(connfd, inputRank, n);
      display_book(&user,1);                        //Display player 1's book 
      display_book(&computer,2);                    //Display user 1's book
	    return;
    }
     ////////////Go Fish/////////////////////////////////////////////////////////////
    else{                          
			sz = snprintf(NULL, 0, "  - Player 2 has no %c's", inputRank[0]);
			tempStr = (char *)malloc(sz + 1);
      snprintf(tempStr, sz+1, "  - Player 2 has no %c's", inputRank[0]);
      strcat(buf, tempStr);
			free(tempStr);
			nextCard = next_card();                                    //Draw a card from deck
      add_card(&user, nextCard);
      if(add_card(&user, nextCard) != 0) return -1;
			sz = snprintf(NULL, 0,"\n  - Go Fish, Player 1 draws %c%c,", nextCard->rank, nextCard->suit);
			tempStr = (char *)malloc(sz + 1);
			snprintf(tempStr, sz+1, "\n  - Go Fish, Player 1 draws %c%c,", nextCard->rank, nextCard->suit);
			strcat(buf, tempStr);
			free(tempStr);     
      bookAdded = check_add_book(&user, nextCard->rank);         //Checks if a book is made 
      if(bookAdded != 0){
        print_book_match(bookAdded, temp,1);
        win = game_over(&user);
        if(win == 1) return;
      }
 	    if(nextCard->rank != inputRank[0]){
        strcat(buf, "\n  - Player 2's turn");
	      //send client hand again before restarting function
				rio_writen(connfd, buf, strlen(buf)+1);
		    strcpy(inputRank, display_hand(&user));
	      rio_writen(connfd, inputRank, n);
	      display_book(&user,1);                        //Display player 1's book 
        display_book(&computer,2);                    //Display user 1's book
				memset(buf, 0, (strlen(buf)+1)*sizeof(buf[0]));
				return;
      }
      else{
        strcat(buf, "\n  - Player 1 gets another turn");             //If the card they draw is what they asked for they get another turn
  		  rio_writen(connfd, buf, strlen(buf)+1);
				strcpy(inputRank, display_hand(&user));
	      rio_writen(connfd, inputRank, n);
	      display_book(&user,1);                        //Display player 1's book 
        display_book(&computer,2);                    //Display user 1's book
	     	memset(buf, 0, (strlen(buf)+1)*sizeof(buf[0]));
				return;
      } 
	  }
        
////////////////////Player 2's turn/////////////////////////////////////////////
  while((turn == 0) && (win == 0)){
    strcat(buf, display_hand(&user));                               //Display player 1's hand
    strcat(buf, display_book(&user,1));                         //Display user 1's book
    strcat(buf, display_book(&computer,2));                         //Display user 2's book
    if(computer.hand_size == 0){
		  nextCard = next_card();
		  add_card(&computer,nextCard);
			if(add_card(&computer,nextCard) != 0) return -1;
			strcat(buf, "\n  - Go Fish, Player 2 draws a card");
			turn = 1;
		}
		else{
			inputRank[0] =  computer_play(&computer);             //Prompt player 1 to enter a rank
      temp = copy_hand_list(&computer);
      transferCards = search(&user, inputRank[0]);          //Check player 2's hand to see if they have that rank
    ///////////////////////If they have the card - transfer cards//////////////
      if(transferCards == 1){                       
        strcat(buf, "\n  - Player 1 has");
				transfer_cards(&user, &computer, inputRank[0]);
        bookAdded = check_add_book(&computer, inputRank[0]);
        if(bookAdded != 0){
					print_book_match(bookAdded,temp, 2);
          win = game_over(&computer);
          if(win == 1) break;
			  }  
	      strcat(buf, "\nPlayer 2 gets another turn");
      }
    /////////////////////Go Fish//////////////////////////////////////////////
      else{                            //If they dont have the card exit the loop and switch to user 2's turn
        sz = snprintf(NULL, 0, "\n  - Player 1 has no %c's", inputRank[0]);
				tempStr = (char *)malloc(sz+1);
				snprintf(tempStr, sz+1, "\n  - Player 1 has no %c's", inputRank[0]);
        strcat(buf, tempStr);
				free(tempStr);
				nextCard = next_card();  //Draw a card from deck
        if(add_card(&computer, nextCard) != 0) return -1;
        strcat(buf, "\n  - Go Fish, Player 2 draws a card");
        bookAdded = check_add_book(&computer, nextCard->rank);
        if(bookAdded != 0){
					sz = snprintf(NULL, 0, "/n  - Player 2 drew %c%c", nextCard->rank, nextCard->suit);
					tempStr = (char *)malloc(sz+1);
					snprintf(tempStr, sz+1, "/n  - Player 2 drew %c%c", nextCard->rank, nextCard->suit);
					strcat(buf, tempStr);
					free(tempStr);
					print_book_match(bookAdded,temp,2);
          win = game_over(&computer);
          if(win == 1) break;
        }
				if(nextCard->rank != inputRank[0]){
				  strcat(buf, "\n  - Player 1's turn");
					turn = 1; 
			  }
        else{
					strcat(buf, "\n  - Player 2 gets another turn");
        }
	    }
    }
	rio_writen(connfd, buf, (strlen(buf)+1)*sizeof(buf[0]));
  memset(buf, 0, (strlen(buf)+1)*sizeof(buf[0]));
	}
}


////////////////////////End of Game!/////////////////////////////////////////////
    strcat(buf, display_hand(&user));
    strcat(buf, display_book(&user, 1));
    strcat(buf, display_book(&computer,2));

    if(game_over(&user) == 1){
			sz = snprintf(NULL, 0, "\nPlayer 1 Wins! %d-%d", strlen(user.book), strlen(computer.book));
			tempStr = (char *)malloc(sz+1);
      snprintf(tempStr, sz+1, "\nPlayer 1 Wins! %d-%d", strlen(user.book), strlen(computer.book));
			strcat(buf, tempStr);
			free(tempStr);
		}
    else if(game_over(&computer) == 1){
      sz = snprintf(NULL, 0, "\nPlayer 2 Wins! %d-%d", strlen(computer.book), strlen(user.book));
			tempStr = (char *)malloc(sz+1);
			snprintf(tempStr, sz+1, "\nPlayer 2 Wins! %d-%d", strlen(computer.book), strlen(user.book));
			strcat(buf, tempStr);
			free(tempStr);
		}
    else{strcat(buf, "\nERROR! - No one won!");}
    strcat(buf,"\nDo you want to play again?[Y/N]");
		rio_writen(connfd, buf, (strlen(buf)+1*sizeof(buf[0])));
		memset(buf, 0, (strlen(buf)+1)*sizeof(buf[0]));

/////////////////////Play Again///////////////////////////////////////////////

    char input;
    while(playAgain == 0){
		  while((n = rio_readlineb(&rio, inputRank, MAXLINE)) != 0) {
		    rio_writen(connfd, inputRank, n);
			 }  
     // while((getchar()) != '\n');
      if(tolower(inputRank) == 'y'){
        if(reset_player(&user) != 0) return -1;
				if(reset_player(&computer) != 0) return -1;
				playAgain = 1;
				win = 0;
        break;
      }
      else if(tolower(input) == 'n'){
        play = 0;
        exit(0);
      }*/
//client handles wrong inputs
/*      else {
        printf("\nError! Please enter Y or N");
      }*/ 
  //  }
  }


int main(int argc, char **argv) 
{
    int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid; 

    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }
    listenfd = Open_listenfd(argv[1]);
//////////////////////Starts the game/////////////////////////
   user.book[0] = '\0'; 
  computer.book[0] = '\0';
	
while(play==1){                              //start game
    playAgain = 0;
    if(shuffle() != 0) return -1;                                  //shuffle cards
    if(deal_player_cards(&user) != 0) return -1;   //deal cards
    if(deal_player_cards(&computer) != 0) return -1;
    user.hand_size = 7;
    computer.hand_size = 7;
///////////////////////Loops Through Players////////////////////////////////   
    while(win == 0){
    while (1) {
        clientlen=sizeof(struct sockaddr_storage);
	connfdp = Malloc(sizeof(int)); //line:conc:echoservert:beginmalloc
	*connfdp = Accept(listenfd, (SA *) &clientaddr, &clientlen); //line:conc:echoservert:endmalloc
	Pthread_create(&tid, NULL, thread, connfdp);
    }
  }
}
}

/* Thread routine */
void *thread(void *vargp) 
{  
    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self()); //line:conc:echoservert:detach
    Free(vargp);                    //line:conc:echoservert:free
		gofish(connfd);
    Close(connfd);
    return NULL;
}
/* $end echoservertmain */

