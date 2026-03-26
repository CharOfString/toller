/*
 * Copyright (C) 2026 MarcusPy827
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "file_systems/file_systems.h"
#include "utils/log/log.h"

/**
 * @brief Print command usage help to standard output.
 * @param None
 * @return void
 */
void print_manual() {
	fprintf(stdout, "Toller (Experimental)\n");
	fprintf(stdout, "Useage: \n");
	fprintf(stdout, "\t--help, -h\tShow this help message.\n");
	fprintf(stdout, "\t--init, -i\tInitialize Toller for first run.\n");
}

/**
 * @brief Program entry point.
 *
 * @param argc Number of command-line arguments.
 * @param argv Command-line argument vector.
 * @return int Returns 0 on success, 1 for unknown arguments.
 */
int main(int argc, char *argv[]) {
	if (argc == 1) {
		print_manual();
		return 0;
	}

	if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
		print_manual();
		return 0;
	} else if (strcmp(argv[1], "--init") == 0 || strcmp(argv[1], "-i") == 0) {
		init_toller();
		return 0;
	} else {
		write_log_with_tag(LOG_ERROR, "Main", "Unknown argument", "%s.", argv[1]);
		print_manual();
		return 1;
	}

	return 0;
}
