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
#include <string.h>

#include "path_handler.h"
#include "../log/log.h"

static char* duplicate_string(const char* src) {
    size_t len;
    char* dst;

    if (src == NULL) {
        return NULL;
    }

    len = strlen(src) + 1;
    if (len > MAX_PATH_LENGTH) {
        return NULL;
    }

    dst = malloc(len);
    if (dst == NULL) {
        return NULL;
    }

    memcpy(dst, src, len);
    return dst;
}

/**
 * @brief Get the system data directory path.
 * 
 * @details The function first gets @c XDG_DATA_HOME variable, and returns if
 *          it is set. If not, it will try to get @c HOME variable and return
 *          the path as @c HOME/.local/share if avaliable. If failed, then it
 *          will return @c NULL in the path part.
 * @note The function is actually returning a struct, not just a string.
 * @see The struct @ref path_query_response_t defined in @ref path_handler.h
 * @return (path_query_response_t) A struct containing status code and system
 *                                 data directory path (if found).
 */

path_query_response_t get_system_data_dir() {
    path_query_response_t response;
    const char* xdg_data = getenv("XDG_DATA_HOME");
    if (xdg_data != NULL) {
        response.path = duplicate_string(xdg_data);
        response.status_code = (response.path != NULL) ? 0 : -2;
        return response;
    } else {
        const char* home_dir = getenv("HOME");
        if (home_dir != NULL) {
            char temp_path[MAX_PATH_LENGTH];
            int written = snprintf(temp_path, sizeof(temp_path),
                "%s/.local/share", home_dir);

            if (written < 0 || written >= (int)sizeof(temp_path)) {
                // While exceeding the max buffer size
                write_log_with_tag(LOG_ERROR, "Path Handler",
                    "Error getting path",
                    "The generated system data directory path exceeds the ",
                    "MAX_PATH_LEN");
                response.status_code = -2;
                response.path = NULL;
                return response;
            }

            response.path = duplicate_string(temp_path);
            if (response.path == NULL) {
                response.status_code = -2;
                return response;
            }

            response.status_code = -1;
            return response;
        } else {
            response.status_code = -2;
            response.path = NULL;
            return response;
        }
    }
}
