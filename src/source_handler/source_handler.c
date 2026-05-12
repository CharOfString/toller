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
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <cjson/cJSON.h>

#include "./source_handler.h"
#include "../path_handler/path_handler.h"
#include "../utils/log/log.h"
#include "../global_vars/global_vars.h"


/**
 * @brief Validate if the source file is valid and ready to be used.
 * 
 * @details The function first checks if the target file exists, then it reads
 *          the file and checks if all the required fields are present. Finally
 *          it checks if the package is actually avaliable...
 * @note If @c silently_fail is set to @c 1, the function will NO longer report
 *       any error or warning.
 * @param target (char*) The path to the source file to validate.
 * @param silently_fail (int) Whether to fail silently.
 * @return (int) @c 0 if the source is valid, @c -1 if the source is invalid,
 *               and @c -2 on operation failure.
 */

int validate_source(char* target, int silently_fail) {
    /* First check if file exists */
    int file_existance_check_result = check_file_exist(target, 0);
    if (file_existance_check_result == -2) {
        if (!silently_fail) {
            write_log_with_tag(LOG_ERROR, "Source Handler",
                "Validation failed",
                "The target file %s does NOT exist.", target);
        }
        return -2;
    }

    /* Try to open the file */
    int target_fd = open(target, O_RDONLY);
    if (target_fd == -1) {
        if (!silently_fail) {
            write_log_with_tag(LOG_ERROR, "Source Handler",
                "Validation failed",
                "Failed to open the target file %s.", target);
        }
        return -2;
    }


    /* Lock the file */
    struct flock target_flock;
    target_flock.l_type = F_RDLCK;
    target_flock.l_whence = SEEK_SET;
    target_flock.l_start = 0;
    target_flock.l_len = 0;  /* Zero indicates until EOF */
    target_flock.l_pid = getpid();

    if (fcntl(target_fd, F_SETLKW, &target_flock) == -1) {
        if (!silently_fail) {
            write_log_with_tag(LOG_ERROR, "Source Handler",
                "Validation failed",
                "Failed to acquire read lock on %s.", target);
        }

        close(target_fd);
        return -2;
    }

    /* Read the file */
    off_t source_read_size = lseek(target_fd, 0, SEEK_END);
    if (source_read_size == -1) {
        if (!silently_fail) {
            write_log_with_tag(LOG_ERROR, "Source Handler",
                "Validation failed", "Failed to read %s.", target);
        }

        target_flock.l_type = F_UNLCK;
        if (fcntl(target_fd, F_SETLK, &target_flock) == -1) {
            if (!silently_fail) {
                write_log_with_tag(LOG_WARNING, "Source Handler",
                    "Unlocking failed",
                    "Failed to release lock on %s.", target);
            }
        }
        close(target_fd);
        return -2;
    }

    /* Rewind to file beginning & read to buffer */
    lseek(target_fd, 0, SEEK_SET);
    char* buffer = malloc(source_read_size + 1);

    if (!buffer) {
        target_flock.l_type = F_UNLCK;
        if (fcntl(target_fd, F_SETLK, &target_flock) == -1) {
            if (!silently_fail) {
                write_log_with_tag(LOG_WARNING, "Source Handler",
                    "Unlocking failed",
                    "Failed to release lock on %s.", target);
            }
        }
        close(target_fd);
        return -2;
    }

    ssize_t bytes_read = read(target_fd, buffer, source_read_size);
    if (bytes_read != source_read_size) {
        if (!silently_fail) {
            write_log_with_tag(LOG_ERROR, "Source Handler",
                "Validation failed",
                "Failed to read the content of %s, read size MISMATCH!!",
                target);
        }
        free(buffer);

        target_flock.l_type = F_UNLCK;
        if (fcntl(target_fd, F_SETLK, &target_flock) == -1) {
            if (!silently_fail) {
                write_log_with_tag(LOG_WARNING, "Source Handler",
                    "Unlocking failed",
                    "Failed to release lock on %s.", target);
            }
        }
        close(target_fd);
        return -2;
    }
    buffer[source_read_size] = '\0';

    /* Unlock the file */
    target_flock.l_type = F_UNLCK;
    if (fcntl(target_fd, F_SETLK, &target_flock) == -1) {
        if (!silently_fail) {
            write_log_with_tag(LOG_WARNING, "Source Handler",
                "Unlocking failed",
                "Failed to release lock on %s.", target);
        }
    }
    close(target_fd);

    /* Try to parse the content as JSON */
    cJSON* json_source = cJSON_Parse(buffer);
    if (!json_source) {
        if (!silently_fail) {
            write_log_with_tag(LOG_WARNING, "Source Handler",
                "Parsing failed",
                "Failed to parse JSON content from %s.", target);
        }
        free(buffer);
        return -2;
    }

    /* Check the required fields */
    int result = 0;

    cJSON* name = cJSON_GetObjectItem(json_source, "name");
    cJSON* upstream = cJSON_GetObjectItem(json_source, "upstream");
    cJSON* path = cJSON_GetObjectItem(json_source, "path");
    if (!(cJSON_IsString(name) && cJSON_IsString(upstream)
            && cJSON_IsString(path))) {
        if (!silently_fail) {
            write_log_with_tag(LOG_WARNING, "Source Handler",
                "Validation failed",
                "The source file %s is missing required fields.", target);
        }
        cJSON_Delete(json_source);
        free(buffer);
        return -1;
    }

    /* Get the package installation path */
    path_query_response_t package_install_path_response
        = get_package_install_dir();
    if (package_install_path_response.status_code == -2) {
        if (!silently_fail) {
            write_log_with_tag(LOG_ERROR, "Source Handler",
                "Validation failed",
                "Failed to get the package installation path.");
        }

        cJSON_Delete(json_source);
        free(buffer);
        return -2;
    }

    /* Check if path exists */
    char* folder_name = path->valuestring;
    char full_path[MAX_PATH_LENGTH];
    int written = snprintf(full_path, sizeof(full_path), "%s/%s",
        package_install_path_response.path, folder_name);

    if (written < 0 || written >= (int) sizeof(full_path)) {
        if (!silently_fail) {
            write_log_with_tag(LOG_ERROR, "Source Handler",
                "Error getting path",
                "Invalid package installation path length.");
        }

        cJSON_Delete(json_source);
        free(buffer);
        return -2;
    }

    if (check_directory_exist(full_path, silently_fail) != 0) {
        if (!silently_fail) {
            write_log_with_tag(LOG_WARNING, "Source Handler",
                "Validation failed",
                "The path specified in %s does NOT exist.", target);
        }
        result = -1;
    }

    /* Clean up */
    cJSON_Delete(json_source);
    free(buffer);

    return result;
}
