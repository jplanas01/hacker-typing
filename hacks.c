#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define CLEAR_SCREEN printf("\e[1;1H\e[2J")
#define SLPTIME 50000

/* Eventually, I'd like to implement this using ncurses or something like that.
 * You know, without redrawing the screen every time.
 */


/* Fills the target string with jumbled letters. If the current letter has
 * already been unmasked, it is not modified.
 */
void jumble_str(char *s, const int len, char *bool_arr) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
	int i;

    for (i = 0; i < len; ++i) {
		if (!bool_arr[i]) {
			s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
		}
    }

    s[len] = 0;
}

/* Checks that the string hasn't been completely revealed. If the sum of all the
 * boolean values for each letter isn't equal to the length of the string, then
 * there are still letters left to unmask.
 */
char check_complete(char *bool_arr, int len) {
	int i, total = 0;
	for (i = 0; i < len; i++) {
		total += bool_arr[i];
	}
	
	if (total == len) {
		return 1;
	}
	return 0;
}

/* Removes carriage returns and linefeeds for prettier printing */
void filter_lines(char *str, int len) {
	int i;
	for (i = 0; i < len; i++) {
		if (str[i] == '\n' || str[i] == '\r') {
			str[i] = ' ';
		}
	}
}

/* Continually jumbles a string, sometimes reveals a letter, and prints the
 * jumbled string and clears the string. Tracking of revealed letters is done
 * through a char array.
 */
void crack_pass2 (char *str, int slp) {
	int len = strlen(str);
	int i;
	char jumbled[len + 1];
	char show_letter[len];
	memset(show_letter, 0, len);
	
	filter_lines(str, len);
	
	while(!check_complete(show_letter, len)) {
		usleep(slp);
		jumble_str(jumbled, len, show_letter);
		
		for (i = 0; i < len; i++) {
			if (rand() % 100 > 97) {
				jumbled[i] = str[i];
				show_letter[i] = 1;
			}
		}
		printf("%s", jumbled);
		fflush(stdout);
		CLEAR_SCREEN;
	}
	
	printf("%s", str);
	putchar('\n');
}
		
int main(int argc, char *argv[]) {
	srand(time(NULL));
	
	CLEAR_SCREEN;
	if (argc > 1) {
		crack_pass2(argv[1], SLPTIME);
	} else {
		crack_pass2("helloworld9999", SLPTIME);
	}
}
