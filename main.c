#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "blackjack.h"

/* card tray object --------------------------------- */
extern int CardTray[N_CARDSET*N_CARD];
extern int cardIndex;

/* player info -------------------------------------- */
extern int dollar[N_MAX_USER];			/* dollars that each player has */
extern int n_user;						/* number of users */

/* play yard information ---------------------------- */
extern int cardhold[N_MAX_USER + 1][N_MAX_CARDHOLD];	/* cards that currently the players hold */
extern int cardSum[N_MAX_USER + 1];		/* sum of the cards */
extern int bet[N_MAX_USER];				/* current betting */
extern int result[N_MAX_USER + 1];		/* round result */
extern int gameEnd; 					/* game end flag */


int main(int argc, char *argv[]) {
	int i, roundIndex = 0;

	srand((unsigned)time(NULL));

	/* set the number of players */
	n_user = configUser();

	/* game initialization ------------------------------ */
	/* 1. players' dollar */
	for (i = 0; i < n_user; i++)
		dollar[i] = N_DOLLAR;
	/* 2. card tray */
	mixCardTray();

	/* game start --------------------------------------- */
	do {
		printf("\n------------------------------------------------");
		printf("\n------------ ROUND %d (cardIndex:%d)--------------------------", ++roundIndex, cardIndex);
		printf("\n------------------------------------------------\n\n");

		printf(" ------- BETTING STEP -------\n");
		for (i = 0; i < n_user; i++)
			betDollar(i);
		printf("----------------------------\n\n");

		offerCards();  /* give cards to all the players */
		if (finishGame()) break;

		printf(" ----------- CARD OFFERING ---------------");
		printCardInitialStatus();

		printf("\n------------------ GAME start --------------------------\n");
		/* each player's turn */
		for (i = 0; i <= n_user; i++)  /* each player: 0(me) ~ dealer(n_user) */
		{
			int cardcnt = 2;  /* initial card count */

			/* play turn */
			playGame(i, cardcnt);
			if (finishGame()) break;

			/* print dealer result */
			if (i == n_user)
				printDealerResult();
		}
		if (gameEnd == 1) break;

		/* result */
		printf("\n -------------------- ROUND %d result ....\n", roundIndex);
		for (i = 0; i < n_user; i++)
			checkResult(i, result[i]);

	} while (gameEnd == 0);

	printf("\n -------------------------------------------\n");
	printf("\n -------------------------------------------\n");
	printf("\n -------------------------------------------\n");
	printf("\n -------------------------------------------\n");
	checkWinner();
	system("pause");

	return 0;
}
