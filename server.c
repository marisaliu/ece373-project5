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
	
/////////////////////Player 1's Turn///////////////////////////////////
while((turn == 1) && (win == 0)){
	playAgain = 0;	rio_writen(connfd, "10", 10);
		strcpy(buf, display_hand(&user));                          //Display player 1's hand
		rio_writen(connfd, buf, 150);
		strcpy(buf, display_book(&user,1));                        //Display player 1's book 
		rio_writen(connfd, buf, 150);
		strcpy(buf, display_book(&computer,2)); 
		rio_writen(connfd, buf, 150);
    if(user.hand_size == 0){                      //If player's hand is empty, player will draw a card and end their turn
	  nextCard = next_card();
	  if(add_card(&user,nextCard) != 0) return -1;
	  sz = snprintf(NULL, 0,"  - Go Fish, Player 1 draws %c%c,\n", nextCard->rank, nextCard->suit);
    tempStr = (char *)malloc(sz + 1);
    snprintf(tempStr, sz+1, "  - Go Fish, Player 1 draws %c%c,\n", nextCard->rank, nextCard->suit);
	  strcpy(buf, tempStr);
		free(tempStr);
		rio_writen(connfd, buf, 150);
  } 
	else{
		rio_writen(connfd, "2", 150);
		while(rio_readlineb(&rio, inputRank, 5)<2);
	  temp = copy_hand_list(&user);
    transferCards = search(&computer, inputRank[0]); //Check player 2's hand to see if they have that rank
     /////////////If they have the rank transfer the cards///////////////////////////////////  
	  if(transferCards == 1){          
		strcpy(buf, "TEST\n ");
		rio_writen(connfd, buf, strlen(buf));
		strcpy(buf, "  - Player 2 has");
			rio_writen(connfd, buf, strlen(buf));
			strcpy(buf, transfer_cards(&computer, &user, inputRank[0]));
			rio_writen(connfd, buf, strlen(buf));
  	  if(strcmp(transfer_cards(&computer, &user, inputRank[0]), "0") == 0) return -1;
      bookAdded = check_add_book(&user, inputRank[0]);
      if(bookAdded != 0){
				strcpy(buf, print_book_match(bookAdded,temp,1));
				rio_writen(connfd, buf, strlen(buf));
				sz = snprintf(NULL,0,"  - Player 1 books %c\n", bookAdded);
				tempStr = (char *)malloc(sz + 1);
				snprintf(tempStr, sz+1, "  - Player 1 books %c\n", bookAdded);
				strcpy(buf, tempStr);
				free(tempStr);
				rio_writen(connfd, buf,strlen(buf));
				win = game_over(&user);
        if(win == 1) break;
	    }
			strcpy(buf, "  \n");
			rio_writen(connfd, buf, strlen(buf));
	    strcpy(buf, "  - Player 1 gets another turn\n");
			rio_writen(connfd, buf, strlen(buf)); 
    }
     ////////////Go Fish/////////////////////////////////////////////////////////////
    else{                     
			strcpy(buf, " TEST2\n");
			rio_writen(connfd, buf, strlen(buf));
			sz = snprintf(NULL, 0, "  - Player 2 has no %c's\n", inputRank[0]);
			tempStr = (char *)malloc(sz + 1);
      snprintf(tempStr, sz+1, "  - Player 2 has no %c's\n", inputRank[0]);
      strcpy(buf, tempStr);
			free(tempStr);
			rio_writen(connfd, buf, strlen(buf));
			nextCard = next_card();                                    //Draw a card from deck
      if(add_card(&user, nextCard) != 0) return -1;
			sz = snprintf(NULL, 0,"  - Go Fish, Player 1 draws %c%c,\n", nextCard->rank, nextCard->suit);
			tempStr = (char *)malloc(sz + 1);
			snprintf(tempStr, sz+1, "  - Go Fish, Player 1 draws %c%c,\n", nextCard->rank, nextCard->suit);
			strcpy(buf, tempStr);
			free(tempStr);
			rio_writen(connfd, buf,strlen(buf));     
			bookAdded = check_add_book(&user, nextCard->rank);         //Checks if a book is made 
      if(bookAdded != 0){
        strcpy(buf, print_book_match(bookAdded,temp,1));
				rio_writen(connfd, buf, strlen(buf));
				sz = snprintf(NULL,0,"  - Player 1 books %c\n", bookAdded);
				tempStr = (char *)malloc(sz + 1);
				snprintf(tempStr, sz+1, "  - Player 1 books %c\n", bookAdded);
				strcpy(buf, tempStr);
				free(tempStr);
				rio_writen(connfd, buf, strlen(buf));
				win = game_over(&user);
        if(win == 1) break;
      }
 	    if(nextCard->rank != inputRank[0]){
        strcpy(buf, "  - Player 2's turn\n");
				rio_writen(connfd, buf, strlen(buf));
				  strcpy(buf, "  \n");
				rio_writen(connfd, buf, strlen(buf));
			turn = 0;
			}
      else{
        strcpy(buf, "  - Player 1 gets another turn\n");             //If the card they draw is what they asked for they get another turn
  		  rio_writen(connfd, buf,strlen(buf));
			  strcpy(buf, "  \n");
				rio_writen(connfd, buf, strlen(buf));
			} 
	  }
  }      
////////////////////Player 2's turn/////////////////////////////////////////////
  while((turn == 0) && (win == 0)){
    strcpy(buf, display_hand(&user));                               //Display player 1's hand
		rio_writen(connfd, buf, strlen(buf));
		strcpy(buf, display_book(&user,1));                         //Display user 1's book
		rio_writen(connfd, buf, strlen(buf));
		strcpy(buf, display_book(&computer,2));                         //Display user 2's book
    rio_writen(connfd, buf, strlen(buf));
		if(computer.hand_size == 0){
		  nextCard = next_card();
		  //add_card(&computer,nextCard);
			if(add_card(&computer,nextCard) != 0) return -1;
			strcpy(buf, "  - Go Fish, Player 2 draws a card");
			rio_writen(connfd, buf, strlen(buf));
			turn = 1;
		}
		else{
			inputRank[0] =  computer_play(&computer);             //Prompt player 1 to enter a rank
      temp = copy_hand_list(&computer);
      transferCards = search(&user, inputRank[0]);          //Check player 2's hand to see if they have that rank
    ///////////////////////If they have the card - transfer cards//////////////
      if(transferCards == 1){
	  		strcpy(buf, "  \n ");
		  	rio_writen(connfd, buf, strlen(buf));
        strcpy(buf, "  - Player 1 has");
				rio_writen(connfd, buf, strlen(buf));

			  strcpy(buf, transfer_cards(&user, &computer, inputRank[0]));
				rio_writen(connfd, buf, strlen(buf));
        bookAdded = check_add_book(&computer, inputRank[0]);
 				strcpy(buf, "  \n ");
		  	rio_writen(connfd, buf, strlen(buf));
       if(bookAdded != 0){
          strcpy(buf, print_book_match(bookAdded,temp,2));
					rio_writen(connfd, buf, strlen(buf));
					strcpy(buf, "   \n");
					rio_writen(connfd, buf, strlen(buf));
					sz = snprintf(NULL,0,"  - Player 2 books %c\n", bookAdded);
					tempStr = (char *)malloc(sz + 1);
					snprintf(tempStr, sz+1, "  - Player 2 books %c\n", bookAdded);
					strcpy(buf, tempStr);
					free(tempStr);
					rio_writen(connfd, buf, strlen(buf));
					win = game_over(&computer);
          if(win == 1) break;
			  }  
	      strcpy(buf, "Player 2 gets another turn\n");
				rio_writen(connfd, buf, strlen(buf));
			}
    /////////////////////Go Fish//////////////////////////////////////////////
      else{                            //If they dont have the card exit the loop and switch to user 2's turn
			strcpy(buf, "   \n");
			rio_writen(connfd, buf, strlen(buf));
			 sz = snprintf(NULL, 0, "  - Player 1 has no %c's\n", inputRank[0]);
				tempStr = (char *)malloc(sz+1);
				snprintf(tempStr, sz+1, "  - Player 1 has no %c's\n", inputRank[0]);
        strcpy(buf, tempStr);
				free(tempStr);
				rio_writen(connfd, buf, strlen(buf));
				nextCard = next_card();  //Draw a card from deck
        if(add_card(&computer, nextCard) != 0) return -1;
        strcpy(buf, "  - Go Fish, Player 2 draws a card\n");
				rio_writen(connfd, buf, strlen(buf));
				bookAdded = check_add_book(&computer, nextCard->rank);
        if(bookAdded != 0){
					sz = snprintf(NULL, 0, "  - Player 2 drew %c%c\n", nextCard->rank, nextCard->suit);
					tempStr = (char *)malloc(sz+1);
					snprintf(tempStr, sz+1, "  - Player 2 drew %c%c\n", nextCard->rank, nextCard->suit);
					strcpy(buf, tempStr);
					free(tempStr);
					rio_writen(connfd, buf, strlen(buf));
          strcpy(buf, print_book_match(bookAdded,temp,2));
					rio_writen(connfd, buf, strlen(buf));
					sz = snprintf(NULL,0,"  - Player 2 books %c\n", bookAdded);
					tempStr = (char *)malloc(sz + 1);
					snprintf(tempStr, sz+1, "  - Player 2 books %c\n", bookAdded);
					strcpy(buf, tempStr);
					free(tempStr);
					rio_writen(connfd, buf, strlen(buf));
					win = game_over(&computer);
          if(win == 1) break;
        }
				if(nextCard->rank != inputRank[0]){
				  strcpy(buf, "  - Player 1's turn\n");
					rio_writen(connfd, buf, strlen(buf));
					turn = 1; 
			  }
        else{
					strcpy(buf, "  - Player 2 gets another turn\n");
					rio_writen(connfd, buf, strlen(buf));
					strcpy(buf, "  \n");
					rio_writen(connfd, buf, strlen(buf));
				}
	    }
    }
	}
}


////////////////////////End of Game!/////////////////////////////////////////////
    rio_writen(connfd, "10", 10);
		strcpy(buf, display_hand(&user));
		rio_writen(connfd, buf, strlen(buf));
		strcpy(buf, display_book(&user, 1));
		rio_writen(connfd, buf, strlen(buf));
		strcpy(buf, display_book(&computer,2));
		rio_writen(connfd, buf, strlen(buf));
		strcpy(buf, "  \n");
		rio_writen(connfd, buf, strlen(buf));
    if(game_over(&user) == 1){
			sz = snprintf(NULL, 0, "Player 1 Wins! %d-%d\n", strlen(user.book), strlen(computer.book));
			tempStr = (char *)malloc(sz+1);
      snprintf(tempStr, sz+1, "Player 1 Wins! %d-%d\n", strlen(user.book), strlen(computer.book));
			strcpy(buf, tempStr);
			free(tempStr);
			rio_writen(connfd, buf, sz+1);
		}
    else if(game_over(&computer) == 1){
      sz = snprintf(NULL, 0, "Player 2 Wins! %d-%d\n", strlen(computer.book), strlen(user.book));
			tempStr = (char *)malloc(sz+1);
			snprintf(tempStr, sz+1, "Player 2 Wins! %d-%d\n", strlen(computer.book), strlen(user.book));
			strcpy(buf, tempStr);
			free(tempStr);
			rio_writen(connfd, buf, sz+1);
		}
    else{
			strcpy(buf, "ERROR! - No one won!\n");
			rio_writen(connfd, buf, strlen(buf));
		}
		strcpy(buf, "   \n");
		rio_writen(connfd, buf, strlen(buf));
    strcpy(buf,"Do you want to play again?[Y/N]\n");
		rio_writen(connfd, buf, strlen(buf));
		 

/////////////////////Play Again///////////////////////////////////////////////
		
   // char input;
    while(playAgain == 0){
			rio_writen(connfd, "3", 150);
			while((n = rio_readlineb(&rio, inputRank, 4)) < 2);
  if(strcmp(inputRank, "y\n") ==0){
  if(reset_player(&user) != 0) return -1;
		if(reset_player(&computer) != 0) return -1;
		playAgain = 1;
		win = 0;
 
		if(shuffle() != 0) return -1;                                  //shuffle card
		 if(deal_player_cards(&user) != 0) return -1;   //deal cards
  if(deal_player_cards(&computer) != 0) return -1;
    user.hand_size = 7;
	 computer.hand_size = 7;
	 break;
      }
      else if(tolower(inputRank[0]) == 'n'){

        exit(0);
      }
      else {
        strcpy(buf, "Error! Please enter Y or N\n");
				rio_writen(connfd, buf, strlen(buf));
			}
    }
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
    initTurn = 1;
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
//     if(playAgain == 0) break;
  }
}
}
void *thread(void *vargp) 
{  
    int connfd = *((int *)vargp);
		    Pthread_detach(pthread_self()); //line:conc:echoservert:detach
				    Free(vargp);                    //line:conc:echoservert:free
								gofish(connfd);
								    Close(connfd);
										    return NULL;
												}
