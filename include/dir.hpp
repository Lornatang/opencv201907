/**
 * Copyright 2019 DayHR Authors. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ==============================================================================
 */

#ifndef DLCV_DIR_HPP
#define DLCV_DIR_HPP

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>

/**
 * check file or dir exists.
 * Args:
 *   filename: input file name.
 * Returns:
 *   if exists return 0, else return -1.
 * @author: Changyu Liu.
 * @last modify time: 2019.8.5.
 */
int __access__(const char *filename);

/**
 * Create a directory if the file directory does not exist.
 * Args:
 *   dirName: The name of the folder you entered.
 * Returns:
 *   success create return 0, else return -1.
 * @author: Changyu Liu.
 * @last modify time: 2019.7.30.
 */
int __mkdir__(const char *dirName);

/**
 * Delete files generated during program running.
 * Args:
 *   dirName: Folder name to delete.
 * Returns:
 *   success create return 0, else return -1.
 * @author: Changyu Liu.
 * @last modify time: 2019.7.30.
 */
int __rmdir__(const char *dirName);


#endif // DLCV_DIR_HPP
