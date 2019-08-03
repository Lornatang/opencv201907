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

#ifndef DLCV_LOG_HPP
#define DLCV_LOG_HPP

/**
 * print error.
 * example: "[MSG_ERROR][parser_URL(101)]:url invalid"
 * @ author: Changyu Liu
 * @ last modify time: 2019.8.2
 */
#define lprintf(level, format, argv...)                                       \
  do {                                                                        \
    if (level & print_level)                                                  \
      printf("[%s][%s(%d)]:" format, #level, __FUNCTION__, __LINE__, ##argv); \
  } while (0)

#endif  // DLCV_LOG_HPP