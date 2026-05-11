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

#include "./file_systems.h"
#include "../path_handler/path_handler.h"
#include "../log/log.h"

int init_toller() {
    write_log_with_tag(LOG_INFO, "File Systems",
        "Initialization",
        "Initializing file systems and related directories...");

    path_query_response_t data_dir = get_application_data_dir();
    if (data_dir.status_code < 0) {
        write_log_with_tag(LOG_ERROR, "File Systems",
            "Initialization failed",
            "Failed to create application data directory.");
        return -1;
    }

    write_log_with_tag(LOG_OK, "File Systems",
        "Initialization",
        "Found data directory at: %s.", data_dir.path);

    path_query_response_t config_dir = get_application_config_dir();
    if (config_dir.status_code < 0) {
        write_log_with_tag(LOG_ERROR, "File Systems",
            "Initialization failed",
            "Failed to create application config directory.");
        return -1;
    }

    write_log_with_tag(LOG_OK, "File Systems",
        "Initialization",
        "Found config directory at: %s.", config_dir.path);

    path_query_response_t package_installation_dir = get_package_install_dir();
    if (package_installation_dir.status_code < 0) {
        write_log_with_tag(LOG_ERROR, "File Systems",
            "Initialization failed",
            "Failed to create package installation directory.");
        return -1;
    }

    write_log_with_tag(LOG_OK, "File Systems",
        "Initialization",
        "Found package installation directory at: %s.",
        package_installation_dir.path);

    path_query_response_t package_info_dir = get_package_info_dir();
    if (package_info_dir.status_code < 0) {
        write_log_with_tag(LOG_ERROR, "File Systems",
            "Initialization failed",
            "Failed to create package info directory.");
        return -1;
    }

    write_log_with_tag(LOG_OK, "File Systems",
        "Initialization",
        "Found package info directory at: %s.",
        package_info_dir.path);

    write_log_with_tag(LOG_OK, "File Systems",
        "Initialization",
        "Initialization succeed!");
    return 0;
}
