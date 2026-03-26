#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <time.h>
#include <string.h>

void print_manual() {
	fprintf(stdout, "Toller (Experimental)\n");
	fprintf(stdout, "Useage: \n");
	fprintf(stdout, "\t--help, -h\tShow this help message.\n");
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		print_manual();
		return 0;
	}

	if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
		print_manual();
		return 0;
	} else {
		fprintf(stderr, "Unknown argument: %s.\n", argv[1]);
		print_manual();
		return 1;
	}

	return 0;
}
