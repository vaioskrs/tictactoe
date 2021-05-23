#include <conio.h>
#include <time.h>
#include <cnaiapi.h>
#include <stdlib.h>
#include <stdio.h>

#define RECEIVED_PROMPT	"Received>"
#define INPUT_PROMPT "Input> "
#define BUFFSIZE 128
int		len;
void get_conf(computer *comp, appnum *a);
int recvln(connection, char *, int);
int readln(char *, int);
char	buff[BUFFSIZE];
int receive_int(connection conn);
void receive_results(connection conn);
void receive_board(connection conn);
int send_choice(connection conn, int ic);
int main(int argc, char *argv[]) 
{
	appnum a;
	int id;
	computer comp;
	connection conn;
	get_conf(&comp, &a);
	conn = make_contact(comp, a);
	if (conn < 0)
		exit(1);
	printf("Connected to game.\n");
	id = receive_int(conn);
	if (id == 0)
		printf("Turn of second player.\n");
	if (id == 1)
		receive_board(conn);
	int ic;
	int turn = receive_int(conn);
	while (turn < 9) 
	{
		receive_board(conn);
		printf("Play.\n");
		if (send_choice(conn, ic)) 
		{
			turn = 10;
			break;
		}
		receive_board(conn);
		turn = receive_int(conn);
	}
    if (turn == 20 || turn == 21 || turn == 10){
		if (turn == 10) 
		{
			printf("Out of time.\n");
		}
		else if (turn == 20) 
		{
			printf("Opponent fail to play.\n");
		}
		else 
		{
			printf("Opponent desconected.\n");
		}
    }
	else {
		receive_board(conn);
		receive_results(conn);
		send_eof(conn);
	}
	printf("\nClosed.\n\n");
	return 0;
}
void receive_results(connection conn) 
{
	char buff[BUFFSIZE];
	int len;
	len = recv(conn, buff, BUFFSIZE, 0);
	fflush(stdout);
	write(STDOUT_FILENO, buff, len);

}
void get_conf(computer *comp, appnum *a) 
{
    char compname[BUFFSIZE];
    char appname[BUFFSIZE];
    FILE *file = fopen("triliza.conf", "r");
    if (file == NULL) {
        printf("ERROR.");
        exit(1);
    }
    fscanf(file, "%s\n%s", compname, appname);
    fclose(file);
    *comp = cname_to_comp(compname);
    *a =(appnum) atoi(appname);
}
int send_choice(connection conn,int ic)
{
	char	buff[BUFFSIZE];
	int     len;
	time_t begin;
	time_t end;
	ic = receive_int(conn);
	while (ic == 0) {
		printf(INPUT_PROMPT);
		begin = time(NULL);
		fflush(stdout);
		len = readln(buff, BUFFSIZE);
		end = time(NULL);
		if((len > 0) && ((end - begin) < 10)){
			send(conn, buff, len, 0);
			ic = receive_int(conn);
			if (ic == 0) {
				printf("Wrong move.\n");
			}
		}
		else {
			buff[0] = 'o';
			buff[1] = 'u';
			buff[2] = 't';
			buff[3] = ' ';
			buff[4] = 'o';
			buff[5] = 'f';
			buff[6] = ' ';
			buff[7] = 't';
			buff[8] = 'i';
			buff[9] = 'm';
			buff[10] = 'e';
			send(conn, buff, 11, 0);
			ic = -1;
			return -1;
			break;
		}
	}

	return 0;
}
void receive_board(connection conn) 
{
	int len;
	char buff[BUFFSIZE];
	printf(RECEIVED_PROMPT);
	len = recv(conn, buff, BUFFSIZE, 0);
	fflush(stdout);
	write(STDOUT_FILENO, buff, len);
}
int receive_int(connection conn) 
{
	int k;
	int len;
	char buff[BUFFSIZE];
	fflush(stdout);
	len = recv(conn, buff, BUFFSIZE, 0);
	k = buff[0] - '0';
	return k;
}
