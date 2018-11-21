/* 
 * echoservert.c - A concurrent echo server using threads
 */
/* $begin echoservertmain */
#include "csapp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "deck.h"
#include "player.h"
#include <ctype.h>

void *thread(void *vargp);
/*
void echo(int connfd)
{
    int n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("server received %d bytes\n", n);
        Rio_writen(connfd, buf, n);
    }
}
*/
/*
char * serialize_char(char* buf, char in);
char * serialize_card(char* buf, struct card in);
char * serialize_hand(char* buf, struct hand in);
*/
int main(int argc, char **argv) 
{
    int listenfd, *connfdp;
    char *buf[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
//    pthread_t tid; 
    char bookAdded;
    char inputRank;
    int transferCards;
    int play = 1; //1 true, 0 false
    int turn = 1; //1 true, 0 false
    int win = 0;  //1 true, 0 false
    int playAgain;
    struct card* nextCard;
    struct hand* temp;
    user.book[0] = '\0'; 
    computer.book[0] = '\0';

/////////////////////////Set up ports///////////////////////
    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }
    listenfd = Open_listenfd(argv[1]);  

    clientlen=sizeof(struct sockaddr_storage);
    connfdp = Malloc(sizeof(int)); //line:conc:echoservert:beginmalloc
    *connfdp = Accept(listenfd, (SA *) &clientaddr, &clientlen); //line:conc:echoservert:endmalloc
  //    Pthread_create(&tid, NULL, thread, connfdp);
  
//////////////////////Starts the game/////////////////////////
  while(play==1){                              //start game
    playAgain = 0;
    if(shuffle() != 0) return -1;                                  //shuffle cards
    if(deal_player_cards(&user) != 0) return -1;   //deal cards
    if(deal_player_cards(&computer) != 0) return -1;
    user.hand_size = 7;
    computer.hand_size = 7;
///////////////////////Loops Through Players////////////////////////////////   
    while(win == 0){
////////////////////Player 1's Turn///////////////////////////////////
      while((turn==1) && (win == 0)){

//write hand's ranks into string to send to client
	temp = user.card_list;
	while(temp != NULL){
	  buf[0] = strncat(buf[0],temp->top.rank,1);
	  buf[0] = strncat(buf[0], " ",1);
	  temp = temp->next;
	}
	rio_writen(connfdp, buf, strlen(buf[0])); 	 //sending hand string to client

	display_book(&user,1);                        //Display player 1's book 
        display_book(&computer,2);                    //Display user 1's book
        if(user.hand_size == 0){                      //If player's hand is empty, player will draw a card and end their turn
	  nextCard = next_card();
	  add_card(&user,nextCard);
	  if(add_card(&user,nextCard) != 0) return -1;
	  printf("\n  - Go Fish, Player 1 draws %c%c,", nextCard->rank, nextCard->suit);
	  turn = 0;
	} 
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////HANDLE CLIENT TO SERVER INTERACTION HERE///////////////////////
////////////////////////////////////////////////////////////////////////////////////
	else{
	  if((inputRank = rio_readn(connfdp, buf, 1)) < 0){ //receive a message of 1 char
	    printf("\nERROR RECEIVING FROM CLIENT");
	  } 
          temp = copy_hand_list(&user);
          transferCards = search(&computer, inputRank); //Check player 2's hand to see if they have that rank
     /////////////If they have the rank transfer the cards///////////////////////////////////  
	  if(transferCards == 1){          
            printf("  - Player 2 has");
  	    transfer_cards(&computer, &user, inputRank);
  	    if(transfer_cards(&computer, &user, inputRank) < 0) return -1;
            bookAdded = check_add_book(&user, inputRank);
            if(bookAdded != 0){
	      print_book_match(bookAdded,temp,1);
              win = game_over(&user);
              if(win == 1) break;
	    }
	    printf("\n  - Player 1 gets another turn");
          }
     ////////////Go Fish/////////////////////////////////////////////////////////////
          else{                          
            printf("  - Player 2 has no %c's", inputRank);
            nextCard = next_card();                                    //Draw a card from deck
            add_card(&user, nextCard);
            if(add_card(&user, nextCard) != 0) return -1;
            printf("\n  - Go Fish, Player 1 draws %c%c", nextCard->rank, nextCard->suit);
            bookAdded = check_add_book(&user, nextCard->rank);         //Checks if a book is made 
            if(bookAdded != 0){
              print_book_match(bookAdded, temp,1);
              win = game_over(&user);
              if(win == 1) break;
            }
 	    if(nextCard->rank != inputRank){
              printf("\n  - Player 2's turn");
              turn = 0;
            }
            else{
              printf("\n  - Player 1 gets another turn");             //If the card they draw is what they asked for they get another turn
            } 
	  }
        }
      } 

////////////////////Player 2's turn/////////////////////////////////////////////
        while((turn == 0) && (win == 0)){
        display_hand(&user);                               //Display player 1's hand
        display_book(&user,1);                         //Display user 1's book
        display_book(&computer,2);                         //Display user 2's book
        if(computer.hand_size == 0){
	  nextCard = next_card();
	  add_card(&computer,nextCard);
	  if(add_card(&computer,nextCard) != 0) return -1;
	  printf("\n  - Go Fish, Player 2 draws a card");
	  turn = 1;
	}
	else{
	  inputRank =  computer_play(&computer);             //Prompt player 1 to enter a rank
       	  temp = copy_hand_list(&computer);
          transferCards = search(&user, inputRank);          //Check player 2's hand to see if they have that rank
    ///////////////////////If they have the card - transfer cards//////////////
          if(transferCards == 1){                       
            printf("\n  - Player 1 has");
	    transfer_cards(&user, &computer, inputRank);
            bookAdded = check_add_book(&computer, inputRank);
            if(bookAdded != 0){
	      print_book_match(bookAdded,temp, 2);
              win = game_over(&computer);
              if(win == 1) break;
 	    }  
	      printf("\nPlayer 2 gets another turn");
          }
    /////////////////////Go Fish//////////////////////////////////////////////
          else{                            //If they dont have the card exit the loop and switch to user 2's turn
            printf("\n  - Player 1 has no %c's", inputRank);
            nextCard = next_card();  //Draw a card from deck
            if(add_card(&computer, nextCard) != 0) return -1;
            printf("\n  - Go Fish, Player 2 draws a card");
            bookAdded = check_add_book(&computer, nextCard->rank);
            if(bookAdded != 0){
	      printf("/n  - Player 2 drew %c%c", nextCard->rank, nextCard->suit);
	      print_book_match(bookAdded,temp,2);
              win = game_over(&computer);
              if(win == 1) break;
            }
	    if(nextCard->rank != inputRank){
              printf("\n  - Player 1's turn");
	      turn = 1; 
            }
            else{
              printf("\n  - Player 2 gets another turn");
            }
	  }
        }
      }
    }


////////////////////////End of Game!/////////////////////////////////////////////
    display_hand(&user);
    display_book(&user, 1);
    display_book(&computer,2);

    if(game_over(&user) == 1){
      printf("\nPlayer 1 Wins! %d-%d", strlen(user.book), strlen(computer.book));
    }
    else if(game_over(&computer) == 1){
      printf("\nPlayer 2 Wins! %d-%d", strlen(computer.book), strlen(user.book));
    }
    else{printf("\nERROR! - No one won!");}
    printf("\nDo you want to play again?[Y/N]");

/////////////////////Play Again///////////////////////////////////////////////

    char input;
    while(playAgain == 0){
      scanf(" %c", &input);
      while((getchar()) != '\n');
      if(tolower(input) == 'y'){
        if(reset_player(&user) != 0) return -1;
	if(reset_player(&computer) != 0) return -1;
	playAgain = 1;
	win = 0;
        break;
      }
      else if(tolower(input) == 'n'){
        play = 0;
        exit(0);
      }
      else {
        printf("\nError! Please enter Y or N");
      }
    }
  }
}

/* Thread routine */
/*void *thread(void *vargp) 
{  
    char connfd = *((char *)vargp);
    Pthread_detach(pthread_self()); //line:conc:echoservert:detach
    Free(vargp);                    //line:conc:echoservert:free
    inputRank = connfd;
    Close(connfd);
    return NULL;
}*/
/* $end echoservertmain */
/* 
char * serialize_char(char* buf, char in){
  buf[0] = in;
  return buf+sizeof(char);
}
char * serialize_card(char* buf, struct card in){
  buf[0] = serialize_char(in->suit);
  buf[1] = serialize_char(in->rank);
  return buf+sizeof(struct card);
}
char * serialize_hand(char* buf, struct hand in){
  buf[0] = serialize_card(in->top);
  buf[1] = in->next;  //serialize a pointer?
  return buf+sizeof(struct hand);
}*/
