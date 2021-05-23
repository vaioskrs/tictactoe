#include <stdio.h>
#include <cnaiapi.h>
#include <stdlib.h>
#define BUFFSIZE 128
#define RECEIVED_PROMPT	"Received>"
int check_for_winner(char board[BOARDSIZE]);
char receive_choice(connection conn);
int send_int(connection conn, int k);
int is_correct(char, char board[BOARDSIZE]);
void send_board(connection conn, char board[BOARDSIZE]);
void player_turn(char choice);
void display_board(char board[BOARDSIZE]);
void send_results(connection conn, char p);
int main(int argc, char *argv[]) {
	char choice;
	char board[BOARDSIZE];
	int turn = 0;
	char buff[BUFFSIZE];
	char pX = '-';
	char pY = '-';
	char c = 'x';
	connection	connX;
	connection	connY;
	connection	conn0;
	connection	conn1;
	appnum a = 20000;
	printf("Waiting.\n");
	conn0 = await_contact(a);
	if (conn0 < 0)
		exit(1);
	connX = conn0;
	int id = 0;
	send_int(connX, id);
	printf("First player connect.\n");
	printf("Waiting second.\n");
	conn1 = await_contact(a);
	if (conn1 < 0)
		exit(1);
	connY = conn1;
	id = 1;
	send_int(connY, id);
	printf("Second player connect.\n");
	char board [3][3] = {{'1','2','3'},{'4','5','6'},{'7','8','9'}};
	send_board(connY, board);
	send_int(connX, turn);
	while (turn < 9) 
	{
		send_board(connX, board);
		int ic = 0;
		send_int(connX, ic);
		while (ic == 0) 
		{
			choice = receive_choice(connX);
			ic = is_correct(choice, board);
			send_int(connX, ic);
		}
		if (ic == -1) 
		{
			turn = 20;
			break;
		}
		player_turn(choice);
		send_board(connX, board);
		display_board(); 
		send_board(connX, board);
		send_board(connY, board);
		send_results(connX, pX);
		send_results(connY, pY);
		send_eof(connX);
		send_eof(connY);
		printf("\nClosed connection.\n\n");
	}
	return 0;
}
void send_results(connection conn, char p) 
{
	char buff[BUFFSIZE];
	char wm[8];
	int i;
	wm[0] = 'Y';
	wm[1] = 'o';
	wm[2] = 'u';
	wm[3] = ' ';
	wm[4] = 'w';
	wm[5] = 'i';
	wm[6] = 'n';
	wm[7] = '\0';
	char lm[9];
	lm[0] = 'Y';
	lm[1] = 'o';
	lm[2] = 'u';
	lm[3] = ' ';
	lm[4] = 'l';
	lm[5] = 'o';
	lm[6] = 's';
	lm[7] = 't';
	lm[8] = '\0';
	char is[5];
	is[0] = 'D';
	is[1] = 'r';
	is[2] = 'a';
	is[3] = 'w';
	is[4] = '\0';
	fflush(stdout);
	if (p == 'W') 
	{
		for (i = 0; i < 9; i++) 
		{
			buff[i] = wm[i];
		}
	}
	else if (p == 'L') 
	{
		for (i = 0; i < 10; i++) 
		{
			buff[i] = lm[i];
		}
	}
	else 
	{
		for (i = 0; i < 6; i++) 
		{
			buff[i] = is[i];
		}
	}
	send(conn, buff, i, 0);
}
void player_turn(choice) 
{
    int choice;
    int row = 0, column = 0;

    if (turn == 'X')
    {
        cout << "Player one play x: ";
    }
    else if (turn == 'O')
    {
        cout << "Player two play o: ";
    }
    cin >> choice;
    switch (choice)
    {
        case 1: row = 0; column = 0; break;
        case 2: row = 0; column = 1; break;
        case 3: row = 0; column = 2; break;
        case 4: row = 1; column = 0; break;
        case 5: row = 1; column = 1; break;
        case 6: row = 1; column = 2; break;
        case 7: row = 2; column = 0; break;
        case 8: row = 2; column = 1; break;
        case 9: row = 2; column = 2; break;
        default:
        cout << "Enter value again\n";
            player_turn();
    }
    if (turn == 'X' && board[row][column] != 'X' && board[row][column] != 'O')
    {
        board[row][column] = 'X';
        turn = 'O';
    }
    else if (turn == 'O' && board[row][column] != 'X' && board[row][column] != 'O')
    {
        board[row][column] = 'O';
        turn = 'X';
    }
    else
    {
        cout << "Try again\n";
        player_turn();
    }
}
void send_board(connection conn, char board[BOARDSIZE]) 
{
	int len = BOARDSIZE;
	char buff[BUFFSIZE];
	int i;
	fflush(stdout);
	for (i = 0; i < BOARDSIZE; i++) 
	{
		buff[i] = board[i];
	}
	write(STDOUT_FILENO, buff, len);
	send(conn, buff, len, 0);
}
void display_board(board)
{
    cout << "---------------------" << endl << endl;
    cout << "     |     |     " << endl;
    cout << "  " << board[0][0] << "  |  " << board[0][1] << "  |  " << board[0][2] << endl;
    cout << "_____|_____|_____" << endl;
    cout << "     |     |     " << endl;
    cout << "  " << board[1][0] << "  |  " << board[1][1] << "  |  " << board[1][2] << endl;
    cout << "_____|_____|_____" << endl;
    cout << "     |     |     " << endl;
    cout << "  " << board[2][0] << "  |  " << board[2][1] << "  |  " << board[2][2] << endl;
    cout << "     |     |     " << endl;
}

char receive_choice(connection conn) 
{
	int len;
	char buff[BUFFSIZE];
	printf(RECEIVED_PROMPT);
	fflush(stdout);
	len = recv(conn, buff, BUFFSIZE, 0);
	write(STDOUT_FILENO, buff, len);
	return buff[0];
}
int check_for_winner(char board[BOARDSIZE]) 
{
	if (board[2] == board[6] && board[6] == board[10])
		return 1;
	if (board[25] == board[29] && board[29] == board[33])
		return 1;
	if (board[48] == board[52] && board[52] == board[56])
		return 1;
	if (board[2] == board[25] && board[25] == board[48])
		return 1;
	if (board[6] == board[29] && board[29] == board[52])
		return 1;
	if (board[10] == board[33] && board[33] == board[56])
		return 1;
	if (board[2] == board[29] && board[29] == board[56])
		return 1;
	if (board[10] == board[29] && board[29] == board[48])
		return 1;
	return 0;
}
int send_int(connection conn, int k) {
	char	buff[BUFFSIZE];
	buff[0] = k + '0';
	buff[1] = '\0';
	(void)fflush(stdout);
	if (send(conn, buff, 2, 0) != -1) {
		return 1;
	}
	else
		return -1;
}
int is_correct(char choice, char board[BOARDSIZE]) 
{
	if (choice == '0' || choice == '1' || choice == '2' || choice == '3' || choice == '4' || choice == '5' || choice == '6' || choice == '7' || choice == '8') 
	{
		if (choice == '0') {
			if (board[2] == 'O' || board[2] == 'X') 
			{
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '1') {
			if (board[6] == 'O' || board[6] == 'X') 
			{
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '2') {
			if (board[10] == 'O' || board[10] == 'X') 
			{
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '3') {
			if (board[25] == 'O' || board[25] == 'X') 
			{
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '4') {
			if (board[29] == 'O' || board[29] == 'X') 
			{
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '5') {
			if (board[33] == 'O' || board[33] == 'X') 
			{
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '6') {
			if (board[48] == 'O' || board[48] == 'X') 
			{
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '7') {
			if (board[52] == 'O' || board[52] == 'X') 
			{
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '8') {
			if (board[56] == 'O' || board[56] == 'X') 
			{
				return 0;
			}
			else {
				return 1;
			}
		}
	}
	else {
		return -1;
	}
}
