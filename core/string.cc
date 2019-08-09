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

#include "dlcv/string.hpp"

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
size_t __strlen__(const char *__s) {
  const char *p = __s;
  while (*__s != '\0') __s++;
  return __s - p;
}

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
char *__strcat__(char *__s1, const char *__s2) {
  if (__s1 == nullptr || __s2 == nullptr)
    return __s1;
  char *p = __s1;
  while (*__s1) __s1++;
  while ((*__s1++ = *__s2++) != '\0');
  return p;
}

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
char *__strncat__(char *__s1, const char *__s2, size_t n) {
  char *p = __s1;
  while (*p != '\0') p++;
  while (n-- && (*p++ = *__s2++));
  return __s1;
}

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
char *__strcpy__(char *__dst, const char *__src) {
  char *p = __dst;
  while ((*__dst++ = *__src++) != '\0');
  return p;
}

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
char *strncpy(char *__dst, const char *__src, size_t __n) {
  char *p = __dst;
  while (--__n && (*__dst++ = *__src++) != '\0');
  return p;
}

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
int __strcmp__(const char *__s1, const char *__s2) {
  if (__s1 == nullptr || __s2 == nullptr)
    return -1;
  while (*__s1 == *__s2) {
    if (*__s1 == '\0')
      return 0;
    __s1++;
    __s2++;
  }
  return *__s1 - *__s2;
}

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
int strncmp(const char *__s1, const char *__s2, size_t __n) {
  if (__s1 == nullptr || __s2 == nullptr || __n < 0)
    return -1;
  while (--__n && (*__s1++ == *__s2++));
  return *__s1 - *__s2;
}