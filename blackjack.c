#include <stdio.h>
#include <stdlib.h>
#include "blackjack.h"

/* card tray object --------------------------------- */
int CardTray[N_CARDSET*N_CARD];
int cardIndex = 0;

/* player info -------------------------------------- */
int dollar[N_MAX_USER];							/* dollars that each player has */
int n_user;										/* number of users */

/* play yard information ---------------------------- */
int cardhold[N_MAX_USER + 1][N_MAX_CARDHOLD];	/* cards that currently the players hold */
int cardSum[N_MAX_USER + 1];		 			/* sum of the cards */
int bet[N_MAX_USER];							/* current betting */
int result[N_MAX_USER + 1];						/* round result */
int gameEnd = 0; 								/* game end flag */


/* card processing functions ------------------------ */

/* calculate the actual card number in the blackjack game */
int getCardNum(int cardnum) {
	int facevalue = cardnum % 100;
	switch (facevalue) {
	case 1:						/* A => 11 */
		return 11;
	case 11: case 12: case 13:	/* J, Q, K => 10 */
		return 10;
	default:
		return cardnum % 100;
	}
}

/* print the card information (e.g. DiaA) */
void printCard(int cardnum) {
	/* ------------------------------------------------------------
	 * suitvalue: CLV=1, DIA=2, HRT=3, SPD=4
	 * facevalue: ACE=1, ... , JACK=11, QUEEN=12, KING=13
	 * ex) CLVAce=101, DIA8=208, HRTQueen=312, SPD6=406
	 * ------------------------------------------------------------ */
	int suitvalue = cardnum / 100;
	int facevalue = cardnum % 100;
	switch (suitvalue) {
	case 1:
		printf("CLV");
		break;
	case 2:
		printf("DIA");
		break;
	case 3:
		printf("HRT");
		break;
	case 4:
		printf("SPD");
		break;
	}
	switch (facevalue) {
	case 11:
		printf("Jack ");
		break;
	case 12:
		printf("Queen ");
		break;
	case 13:
		printf("King ");
		break;
	default:
		printf("%d ", facevalue);
	}
}


/* card array controllers --------------------------- */

/* mix the card sets and put in the array */
void mixCardTray(void) {
	int i, j, suitvalue, facevalue;
	/* insert cards in order in the array */
	for (i = 0; i < N_CARD; i++) {
		suitvalue = i / N_MAX_CARDNUM % 4 + 1;
		facevalue = i % N_MAX_CARDNUM + 1;
		CardTray[i] = 100 * suitvalue + facevalue;
	}
	/* shuffle the cards */
	for (i = 0; i < N_CARDSET*N_CARD; i++) {
		j = rand() % (N_CARDSET*N_CARD);
		swap(&CardTray[i], &CardTray[j]);
	}
	printf(" --> card is mixed and put into the tray\n");
}

/* get one card from the tray */
int pullCard(void) {
	return CardTray[cardIndex++];
}


/* playing game functions --------------------------- */

/* player settiing */
int configUser(void) {
	int num;
	while (1) {
		printf("Input the number of players (MAX:%d): ", N_MAX_USER);
		num = getIntegerInput();
		if (num <= 0)
			printf("invalid input players (%d)\n", num);\
		else if (num > N_MAX_USER)
			printf("Too many players!\n");
		else
			return num;
	}
}

/* betting */
void betDollar(int user) {
	/* your betting */
	while (user == 0) {
		printf("   -> your betting (total:$%d) : ", dollar[user]);
		bet[user] = getIntegerInput();
		if (bet[user] <= 0)
			printf("   -> invalid input for betting $%d\n", bet[user]);
		else if (bet[user] > dollar[user])
			printf("   -> you only have $%d! bet again\n", dollar[user]);
		else
			return;
	}
	/* players' betting */
	do {
		bet[user] = rand() % N_MAX_BET + 1;
	} while (bet[user] > dollar[user]);
	printf("   -> player%d bets $%d (out of $%d)\n", user, bet[user], dollar[user]);
}

/* offering initial 2 cards */
void offerCards(void) {
	int i;
	/* return when there are not enough cards to offer */
	if (cardIndex >= N_CARDSET * N_CARD - (n_user + 1) * 2) {
		cardIndex = N_CARDSET * N_CARD;
		return;
	}
	/* give the first card */
	cardhold[n_user][0] = pullCard();
	for (i = 0; i < n_user; i++) {
		cardhold[i][0] = pullCard();
	}
	/* give the second card */
	cardhold[n_user][1] = pullCard();
	for (i = 0; i < n_user; i++) {
		cardhold[i][1] = pullCard();
	}
}

/* print initial card status */
void printCardInitialStatus(void) {
	int i;
	printf("\n --- server      : X ");
	printCard(cardhold[n_user][1]);
	printf("\n   -> you        : ");
	printCard(cardhold[0][0]);
	printCard(cardhold[0][1]);
	for (i = 1; i < n_user; i++) {
		printf("\n   -> player %d   : ", i);
		printCard(cardhold[i][0]);
		printCard(cardhold[i][1]);
	}
	printf("\n");
}

/* play game */
void playGame(int user, int cardcnt) {
	if (user == 0)
		printf(">>> my turn! -------------\n");
	else if (user == n_user)
		printf("\n>>> server turn! ------------------------\n");
	else
		printf("\n>>> player %d turn! -------------\n", user);

	while (!isEmptyTray()) /* do until the card tray is empty */
	{
		printUserCardStatus(user, cardcnt);

		/* check the card status */
		if (calcStepResult(user, cardcnt) == 1) {
			/* get action (GO? STOP?) */
			if (getAction(user, cardcnt) == 1) {
				cardcnt++;
				continue;
			}
			result[user] = 1;  /* STOP */
			break;
		}
		else if (calcStepResult(user, cardcnt) == 2) {
			result[user] = 2;  /* DEAD */
			if (user == n_user)
				printf("server DEAD (sum:%d)\n", cardSum[user]);
			else
				printf("DEAD (sum:%d) --> -$%d ($%d)\n",
					cardSum[user], bet[user], dollar[user] - bet[user]);
			break;
		}
		else if (calcStepResult(user, cardcnt) == 3) {
			result[user] = 3;  /* blackjack */
			if (user == n_user)
				printf("Black Jack!T_T... server win\n");
			else
				printf("Black Jack!congratulation, you win!! --> +$%d ($%d)\n",
					2 * bet[user], dollar[user] + 2 * bet[user]);
			break;
		}
	}
}

/* get action */
int getAction(int user, int cardcnt) {
	/* 21 but not blackjack */
	if (cardSum[user] == 21 && cardcnt > 2) {
		printf("\n");
		return 0;
	}
	/* your turn */
	if (user == 0) {
		printf("Action? (0 - go, others - stay) :");
		if (getIntegerInput() == 0) {
			cardhold[user][cardcnt] = pullCard();
			return 1;
		}
		else
			return 0;
	}
	/* computer player's turn */
	/* GO: less than 17, STOP: more than 17 */
	else {
		if (cardSum[user] < N_MAX_GO) {
			printf("GO!\n");
			cardhold[user][cardcnt] = pullCard();
			return 1;
		}
		else {
			printf("STAY!\n");
			return 0;
		}
	}
}

/* print card status */
void printUserCardStatus(int user, int cardcnt) {
	int i;
	printf("   -> card : ");
	for (i = 0; i < cardcnt; i++)
		printCard(cardhold[user][i]);
	printf("\t ::: ");
}

/* calculate the card sum and see if : 1. under 21, 2. over 21, 3. blackjack */
int calcStepResult(int user, int cardcnt) {
	int i, ace = 0;
	cardSum[user] = 0;
	for (i = 0; i < cardcnt; i++) {
		cardSum[user] += getCardNum(cardhold[user][i]);
		if (getCardNum(cardhold[user][i]) == 11)
			ace++;
	}
	/* under 21 */
	if (cardSum[user] < 21)
		return 1;
	/* over 21 */
	else if (cardSum[user] > 21) {
		/* A => 1 */
		if (ace > 0) {
			while (cardSum[user] > 21 && ace > 0) {
				cardSum[user] -= 10;
				ace--;
			}
			if (cardSum[user] < 21)
				return 1;
		}
		return 2;
	}
	/* blackjack */
	else if (cardSum[user] == 21 && cardcnt == 2)
		return 3;
	/* 21 but not blackjack */
	else if (cardSum[user] == 21)
		return 1;
	/* error */
	else
		return -1;
}

/* print round result */
void checkResult(int user, int result) {
	if (user == 0)
		printf("   -> your result : ");
	else
		printf("   -> %d'th player's result : ", user);

	switch (result) {
	case 1:
		if (cardSum[user] >= cardSum[n_user] || cardSum[n_user] > 21)
			printf("win (sum:%d) --> $%d\n", cardSum[user], dollar[user] += bet[user]);
		else
			printf("lose! (sum:%d) --> $%d\n", cardSum[user], dollar[user] -= bet[user]);
		break;
	case 2:
		printf("lose due to overflow! ($%d)\n", dollar[user] -= bet[user]);
		break;
	case 3:
		printf("BlackJack! win ($%d)\n", dollar[user] += 2 * bet[user]);
		break;
	}

	if (dollar[user] == 0) {
		if (user == 0)
			printf("   -> you are bankrupted! game will be ended\n");
		else
			printf("   -> player%d is bankrupted! game will be ended\n", user);
		gameEnd = 1;
	}
}

/* print dealer result */
void printDealerResult() {
	switch (result[n_user]) {
	case 1:
		printf("[[[[[[[ server result is ....  %d ]]]]]]]\n", cardSum[n_user]);
		break;
	case 2:
		printf("[[[[[[[ server result is .... ....overflow!! ]]]]]]]\n");
		break;
	case 3:
		printf("[[[[[[[ server result is ....Blackjack, T_T all remained players lose! ]]]]]]]\n");
		break;
	}
}

/* print final winner */
void checkWinner() {
	int i, winner = 0;
	printf("game end! your money : $%d, players' money : ", dollar[0]);
	for (i = 1; i < n_user; i++) {
		printf("$%d ", dollar[i]);
		if (dollar[i] > dollar[winner])
			winner = i;
	}
	if (winner == 0)
		printf("\n\n\nyou win!!!!\n");
	else
		printf("\n\n\nplayer %d's win\n", winner);
}

/* check game finish */
int finishGame() {
	if (isEmptyTray()) {
		printf("card ran out of the tray!! finishing the game...\n\n\n");
		gameEnd = 1;
		return 1;
	}
	return 0;
}

/* check card tray is empty */
int isEmptyTray() {
	if (cardIndex >= N_CARDSET * N_CARD) {
		gameEnd = 1;
		return 1;
	}
	else
		return 0;
}
