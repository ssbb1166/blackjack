#define N_CARDSET			1
#define N_CARD				52
#define N_DOLLAR			50

#define N_MAX_CARDNUM		13
#define N_MAX_USER			5
#define N_MAX_CARDHOLD		10
#define N_MAX_GO			17
#define N_MAX_BET			5

/* some utility functions */
void swap(int *x, int *y);
int getIntegerInput(void);

/* card processing functions */
int getCardNum(int cardnum);
void printCard(int cardnum);

/* card array controllers */
void mixCardTray(void);
int pullCard(void);

/* playing game functions */
int configUser(void);
void betDollar(int user);
void offerCards(void);
void printCardInitialStatus(void);
void playGame(int user, int cardcnt);
int getAction(int user, int cardcnt);
void printUserCardStatus(int user, int cardcnt);
int calcStepResult(int user, int cardcnt);
void checkResult(int user, int result);
void printDealerResult(void);
void checkWinner(void);
int isEmptyTray(void);
int finishGame(void);
