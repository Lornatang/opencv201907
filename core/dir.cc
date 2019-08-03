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

#include "../include/dir.hpp"

/**
 * Create a directory if the file directory does not exist.
 * Args:
 *   dirName: The name of the folder you entered.
 * Returns:
 *   success create return 0, else return -1.
 * @author: Changyu Liu.
 * @last modify time: 2019.8.2
 */
int __mkdir__(const char *dirName) {
  if (mkdir(dirName, 0777) != -1)
    return 0;
  else
    return -1;
}

/**
 * Delete files generated during program running.
 * Args:
 *   dirName: Folder name to delete.
 * Returns:
 *   success create return 0, else return -1.
 * @author: Changyu Liu.
 * @last modify time: 2019.7.30.
 */
int __rmdir__(const char *dirName) {
  char cur_dir[] = ".";
  char up_dir[] = "..";
  char dir_name[128];
  DIR *dirp;
  struct dirent *dp;
  struct stat dir_stat{};

  if (0 != access(dirName, F_OK))
    return 0;

  if (0 > stat(dirName, &dir_stat)) {
    perror("get directory stat error");
    return -1;
  }

  if (S_ISREG(dir_stat.st_mode)) {
    remove(dirName);
  } else if (S_ISDIR(dir_stat.st_mode)) {
    dirp = opendir(dirName);
    while ((dp = readdir(dirp)) != nullptr) {
      if ((0 == strcmp(cur_dir, dp->d_name)) ||
          (0 == strcmp(up_dir, dp->d_name))) {
        continue;
      }

      sprintf(dir_name, "%s/%s", dirName, dp->d_name);
      __rmdir__(dir_name);
    }
    closedir(dirp);

    rmdir(dirName);
  } else
    perror("unknow file type!");

  return 0;
}