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
#include <unistd.h>
#include <sys/stat.h>

#include "./file_systems.h"
#include "../path_handler/path_handler.h"
#include "../log/log.h"

/**
 * @brief Initialize the file paths used by Toller.
 * 
 * @details This function determines the necessary directories for TOller.
 *          If the directory does NOT exist then the function will try to
 *          create it.
 * 
 * @see src/utils/path_handler/path_handler.h for details.
 *
 * @param (None)
 * @return (int) @c 0 on success, @c -1 on failure.
 */

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


/**
 * @brief Check if a file exists.
 * @details This function checks if a file exists at the specified path.
 *          If @c create_if_not_exist is set to @c 1 and the file does NOT
 *          exist, the function will try to create an empty file.
 * @warning Ensure that the parent directory of the target file exists before
 *          calling this function with @c create_if_not_exist set to @c 1, or
 *          file creation will always fail!!
 * 
 * @param target (const char*) The path to the file to check.
 * @param create_if_not_exist (const int) Whether to create the file if it does
 *                                        not exist.
 * @return (int) @c 0 if the file exists, @c -1 if it does not exist, and @c -2
 *               on operation failure.
 */

int check_file_exist(const char* target, const int create_if_not_exist) {
    // Check if file exists
    if (access(target, F_OK) == 0) {
        return 0;
    }

    if (create_if_not_exist) {
        // Try to create the file
        FILE* file = fopen(target, "w");
        if (file != NULL) {
            fclose(file);
            return 0;
        } else {
            write_log_with_tag(LOG_ERROR, "File Systems",
                "File creation failed",
                "Failed to create file: %s.", target);
            return -2;
        }
    }

    return -1;
}
