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

#ifndef DLCV_STRING_HPP
#define DLCV_STRING_HPP

#include <cstdio>

/**
 * Calculates the length of the string.
 * Args:
 *   __s: origin string.
 * Returns:
 *   string length.
 * Example:
 *   const char *__s = "abc";
 *   size_t length = __strlen__(__s);
 *   cout << "string __s length: " << length << endl;
 * Output:
 *   length: 3.
 * @author: Changyu Liu
 * @last modify time: 2019.8.5
 */
size_t __strlen__(const char *);

/**
 * Concatenate two strings
 * Args:
 *   __s1: new string
 *   __s2: origin string
 * Returns:
 *   __s1.
 * Example:
 *   char *__s1 = "abc";
 *   const char *__s2 = "def";
 *   const char *s = __strcat__(__s1, __s2);
 *   cout << "new string: " << s << endl;
 * Output:
 *   new string: abcdef.
 * @author: Changyu Liu
 * @last modify time: 2019.8.5
 */
char *__strcat__(char *, const char *);

/**
 * Add the first n characters to the end of the __s1 string and override the
 * '\0' at the end of the __s1 string.
 * Args:
 *   __s1: new string
 *   __s2: origin string
 *   n: first n characters
 * Returns:
 *   __s1 point.
 * Example:
 *   char *__s1 = "abc";
 *   const char *__s2 = "def";
 *   int n = 2;
 *   const char *s = __strncat__(__s1, __s2, n);
 *   cout << "new string: " << s << endl;
 * Output:
 *   new string: abcde.
 * @author: Changyu Liu
 * @last modify time: 2019.8.5
 */
char *__strncat__(char *, const char *, size_t);

/**
 * Copy src string to string dst.
 * Args:
 *   __dst: new string
 *   __src: origin string
 * Returns:
 *   __dst.
 * Example:
 *   char *__dst1 = nullptr;
 *   const char *__s2 = "abcdef";
 *   const char *s = __strcpy__(__s1, __s2);
 *   cout << "new string: " << s << endl;
 * Output:
 *   new string: abcdef.
 * @author: Changyu Liu
 * @last modify time: 2019.8.5
 */
char *__strcpy__(char *, const char *);

/**
 * Copy src string to string dst.
 * Args:
 *   __dst: new string.
 *   __src: origin string.
 *   __n: few string.
 * Returns:
 *   __dst.
 * Example:
 *   char *__dst1 = "123";
 *   const char *__s2 = "abcdef";
 *   const char *s = __strncpy__(__s1, __s2, 3);
 *   cout << "new string: " << s << endl;
 * Output:
 *   new string: 123abc.
 * @author: Changyu Liu
 * @last modify time: 2019.8.5
 */
char *strncpy(char *, const char *, size_t );

/**
 * Compare two strings to see if they are equal.
 * Args:
 *   __s1: compare string
 *   __s2: compare string
 * Returns:
 *   equal return 0, s1 > s2 return > 0, s1 < s2 return < 0.
 * Example:
 *   const char *__s1 = "abc";
 *   const char *__s2 = "abcdef";
 *   cout << "is equal? : " << __strcmp__(__s1, __s2) << endl;
 * Output:
 *   is equal? : -1.
 * @author: Changyu Liu
 * @last modify time: 2019.8.5
 */
int __strcmp__(const char *, const char *);

/**
 * Compare the first few strings.
 * Args:
 *   __s1: compare string.
 *   __s2: compare string.
 *   __n: a few string.
 * Returns:
 *   s1 > s2 return > 0, s1 < s2 return < 0, equal return 0.
 * Example:
 *   const char *__s1 = "abc";
 *   const char *__s2 = "abcdef";
 *   cout << "is equal? : " << __strncmp__(__s1, __s2, 4) << endl;
 * Output:
 *   is equal? : -15.
 * @author: Changyu Liu
 * @last modify time: 2019.8.5
 */
int strncmp(const char *, const char *, size_t);

#endif // DLCV_STRING_HPP
