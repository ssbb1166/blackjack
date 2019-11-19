#include <stdio.h>

/* some utility functions =============================================== */

/* swap x and y */
void swap(int *x, int *y) {
	int tmp = *x;
	*x = *y;
	*y = tmp;
}

/* get an integer input */
int getIntegerInput(void) {
    /* ------------------------------------------------------------
     * get an integer input from standard input (keyboard)
     * return: input integer value
     *         (-1 is returned if keyboard input was not integer)
     * ------------------------------------------------------------ */
	int input, num;

	num = scanf("%d", &input);
	fflush(stdin);
	if (num != 1)  /* if it fails to get integer */
		input = -1;

	return input;
}
