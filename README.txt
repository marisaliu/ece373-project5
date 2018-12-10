Partners: Dylan Banh, Marisa Liu

Summary/Solution Approach:
	Server:
		On startup, server initiliazes a deck, both players' hands, and their books
		It then accepts the clients connection to it and writes to client the initial hand and books
		Server then prompts client to send an input, to ask for a card, by writing a flag to the client. The server receives the rank input and goes through the normal processes for gofish. If the card requested was granted, an extra turn is given to the user and server writes another flag to client asking for input until the user no longer has a turn
		At the start of each players' turns, the server sends the user's hand and both players' books
		When 7 books are reached, server writes a prompt to client, asking if they would like to play again, and writes another flag to client, asking for user input
		if n/N is received by the server, it exits
	
	Client:
		On start up, client connects to server and receives the initial hand and books
		When client receives a flag indicating that a hand is being sent, a function is called to parse the string that contains the hand, such that the contents of the hand can be saved clientside
		When client receives a flag indicating that server is requesting user input for user to ask for a card, client waits for the user to input a rank, scanning the saved hand, making sure the rank requested is actually present in the hand, before writing to server the request
		When 7 books are reached, a flag is received by client asking whether or not user would like to play again - if n/N the client will write back an exit flag and then quit

	Errors we encountered:
		When receiving, sometimes the server or client would include characters from previous messages sent or would stall. This was because the number of bytes being received was different to the bytes being sent.
		Another error we encountered was that server would sometimes have a buffer overflow when performing specific functions after we modified them to return strings to write to client instead of just printing them to stdout. This was solved by going through each function and making sure each string being used in the function was properly malloc'ed.
		A third error we encountered was that when prompted to answer whether or not the user would like to play again, the server would receive the y/Y or n/N from client, but would not carry out the if statement if y/Y was received, even though n/N would function correctly.


