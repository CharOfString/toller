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

#include <stdio.h>

#include "file_systems.h"
#include "../path_handler/path_handler.h"

int init_toller() {
    // Placeholder for future initialization code
    fprintf(stdout, "Initializing Toller file systems...\n");
    path_query_response_t data_dir = get_application_data_dir();
    fprintf(stdout, "Data: %s.\n", data_dir.path);
    path_query_response_t config_dir = get_application_config_dir();
    fprintf(stdout, "Config: %s.\n", config_dir.path);
    return 0;
}
