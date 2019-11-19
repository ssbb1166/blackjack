#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "blackjack.h"

/* card tray object ===================================================== */
extern int CardTray[N_CARDSET*N_CARD];
extern int cardIndex;

/* player info ========================================================== */
extern int dollar[N_MAX_USER];                        /* dollars that each player has */
extern int n_user;                                    /* number of users */

/* play yard information ================================================ */
extern int cardhold[N_MAX_USER + 1][N_MAX_CARDHOLD];  /* cards that currently the players hold */
extern int cardSum[N_MAX_USER + 1];                   /* sum of the cards */
extern int bet[N_MAX_USER];                           /* current betting */
extern int result[N_MAX_USER + 1];                    /* round result */
extern int gameEnd;                                   /* game end flag */


/* main ================================================================= */
int main(int argc, char *argv[]) {
	int user, roundIndex = 0;

	srand((unsigned)time(NULL));

	/* set the number of players */
	n_user = configUser();

	/* game initialization ------------------------------ */
	/* 1. players' dollar */
	for (user = 0; user < n_user; user++)
		dollar[user] = N_DOLLAR;
	/* 2. card tray */
	mixCardTray();

	/* game start --------------------------------------- */
	do {
		printf("\n------------------------------------------------");
		printf("\n------------ ROUND %d (cardIndex:%d)--------------------------", ++roundIndex, cardIndex);
		printf("\n------------------------------------------------\n\n");

		printf(" ------- BETTING STEP -------\n");
		for (user = 0; user < n_user; user++)
			betDollar(user);
		printf("----------------------------\n\n");

		offerCards();  /* give cards to all the players */
		if (finishGame()) break;  /* game ends if there is no card in the tray */

		printf(" ----------- CARD OFFERING ---------------");
		printCardInitialStatus();

		printf("\n------------------ GAME start --------------------------\n");
		/* each player's turn */
		for (user = 0; user <= n_user; user++)  /* each player: 0(me) ~ n_user(dealer) */
		{
			int cardcnt = 2;  /* initial card count */

			/* play turn */
			playGame(user, cardcnt);
			if (finishGame()) break;

			/* print dealer result */
			if (user == n_user)
				printDealerResult();
		}
		if (gameEnd == 1) break;

		/* result */
		printf("\n -------------------- ROUND %d result ....\n", roundIndex);
		for (user = 0; user < n_user; user++)
			checkResult(user, result[user]);

	} while (gameEnd == 0);

	printf("\n -------------------------------------------\n");
	printf("\n -------------------------------------------\n");
	printf("\n -------------------------------------------\n");
	printf("\n -------------------------------------------\n");
	checkWinner();
	getchar();  /* keep console window open */

	return 0;
}
