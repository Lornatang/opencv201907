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

#ifndef OCR_LOG_HPP
#define OCR_LOG_HPP

#define MSG_DEBUG   0x01
#define MSG_INFO    0x02
#define MSG_WARNING 0x03
#define MSG_ERROR   0x04

static int print_level = MSG_INFO | MSG_DEBUG | MSG_WARNING | MSG_ERROR;

/**
 * print error.
 * @example:
 *   [MSG_ERROR][parser_URL(101)]:url invalid."
 * @author: Changyu Liu
 * @last modify time: 2019.8.1
 */
#define lprintf(level, format, argv...)                                       \
  do {                                                                        \
    if (level & print_level)                                                  \
      printf("[%s][%s(%d)]:" format, #level, __FUNCTION__, __LINE__, ##argv); \
  } while (0)

#endif  // OCR_LOG_HPP