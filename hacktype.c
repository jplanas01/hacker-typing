#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#define MAXCHARS 4

char *read_file(char *filename) {
	FILE *f = fopen(filename, "rb");
	long fsize = 0;
	char *string;
	
	if (f == NULL) {
		printf("Error opening file %s.\n", filename);
		exit(EXIT_FAILURE);
	}
	
	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	string = malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = 0;
	return string;
}

void hacktype(char *str) {
	int i = 0;
	char c[2];
	while (*str != '\0') {
		fread(c, sizeof(char), 1, stdin);
		for (i = 0; i < rand() % (MAXCHARS) + 1 && *str != '\0'; i++) {
			/* Could also be done in theory with a printf and a fixed (but
			 * changing) max length and an increasing pointer.
			 */
			putchar(*str++);
		}
	}
}

/* Disables terminal echoing of user-typed gibberish. Stolen from some website I
 * can't remember the name of, along with a Stack Overflow post.
 */
void disable_echo(struct termios *oflags) {
	struct termios nflags;
	tcgetattr(STDIN_FILENO, oflags);
	memcpy(&nflags, oflags, sizeof(struct termios));
	nflags.c_lflag &= ~ECHO;
	nflags.c_lflag &= ~ICANON;
	nflags.c_lflag |= ECHONL;
	nflags.c_cc[VMIN] = 1;
	nflags.c_cc[VTIME] = 0;

	if (tcsetattr(STDIN_FILENO, TCSANOW, &nflags) != 0) {
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

void enable_echo(struct termios *oflags) {
	if (tcsetattr(STDIN_FILENO, TCSANOW, oflags) != 0) {
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *argv[]) {
	struct termios oflags;
	char *target;

	disable_echo(&oflags);
	srand(time(NULL));

	if (argc > 1) {
		target = read_file(argv[1]);
	} else {
		target = "hello world! asdfhasdfasdfasdfasdfasd";
	}
	hacktype(target);
	putchar('\n');
	
	if (argc > 1) {
		free(target);
	}
	enable_echo(&oflags);
}
