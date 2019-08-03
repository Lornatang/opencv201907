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

/**
 * Image download function header file.
 * @author: Changyu Liu
 * @last modify time: 2019.8.2
 */

#ifndef DLCV_DOWNLOAD_HPP
#define DLCV_DOWNLOAD_HPP

#define HOST_NAME_LEN 256
#define URI_MAX_LEN 2048
#define RECV_BUF 8192
#define RCV_SND_TIMEOUT (10 * 1000)

#define MSG_DEBUG 0x01
#define MSG_INFO 0x02
#define MSG_ERROR 0x04

#define _MIN(x, y) ((x) > (y) ? (y) : (x))

#define HTTP_OK 200
#define HTTP_REDIRECT 302
#define HTTP_NOT_FOUND 404

#ifndef INCLUDE_PRINT_LEVEL
#define INCLUDE_PRINT_LEVEL
static int print_level = /*MSG_DEBUG |*/ MSG_INFO | MSG_ERROR;
#endif  // INCLUDE_PRINT_LEVEL

#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
#include <netdb.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctime>
#include <unistd.h>

#include "log.hpp"

typedef struct {
  int sock;  // Socket Communicating with Server
  FILE *in;  // Sock descriptor to file pointer, easy to read and write
  char host_name[HOST_NAME_LEN];
  int port;
  char url[URI_MAX_LEN];
  char buffer[RECV_BUF];
  int status_code;
  int chunked_flag;
  int len;
  char location[URI_MAX_LEN];
  const char *save_path;
  FILE *save_file;
  int recv_data_len;
  time_t start_recv_time;
  time_t end_recv_time;
} http_t;

/**
 * Ignore Case and case.
 * Args:
 *  str: Strings to be compared
 *  sub: Original string
 * Returns:
 *  NULL
 * @ author: Changyu Liu
 * @ time: 2019.7.25
 */
char *strncasestr(char *str, const char *sub);

/**
 * resolve domain.
 * Args:
 *  url: Incoming URL links
 *  info: Information parsed from incoming URL links
 * Returns:
 *  resolve success return 0, resolve faile return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.7.25
 */
int parser_URL(char *url, http_t *info);

/**
 * resolve dns
 * Args:
 *   host_name: Domain Name Address to Resolve
 * Returns:
 *   resolve success return dns address, else return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.7.25
 */
unsigned long dns(char *host_name);

/**
 * set connect time out
 * Args:
 *   sock: Configure the connection status of Scoket
 * Returns:
 *   connet success return 0, else return -1.
 * @ author: Changyu Liu
 * @ last modifly time: 2019.7.25
 */
int set_socket_option(int sock);

/**
 * connet to server func
 * Args:
 *   info: Information to connect to the server.
 * Returns:
 *   connect server success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.7.25
 * */
int connect_server(http_t *info);

/**
 * Send server requests
 * Args:
 *   info: Request information byte stream
 * Returns:
 *   Send the correct request bytes to the server
 * @ author: Changyu Liu
 * @ last modifly time: 2019.7.25
 */
int send_request(http_t *info);

/**
 * resolve response header
 * Args:
 *   info: Check whether the server header is status information
 * Returns:
 *   resolve success return 0, else return -1.
 * @ author: Changyu Liu
 * @ last modifly time: 2019.7.25
 */
int parse_http_header(http_t *info) ;

/**
 * Save the content of the server response.
 * Args:
 *   info: Server connection information
 *   buf: Server buffer area
 *   len: Server buffer byte length
 * Returns:
 *   success return 0, else return -1.
 * @ author: Changyu Liu
 * @ last modifly time: 2019.7.25
 */
int save_data(http_t *info, const char *buf, int len) ;

/**
 * read file data
 * Args:
 *   info: file information
 *   len: file buffer byte length
 * Returns:
 *   success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.7.25
 */
int read_data(http_t *info, int len);

/**
 * Chunked data sent back by receiving server
 * Args:
 *   info: Chunked data info
 * Returns:
 *   success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.7.25
 */
int recv_chunked_response(http_t *info);

/**
 * Calculate average download speed
 * Args:
 *   info: download file data info
 * Returns:
 *   success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.7.25
 */
float calc_download_speed(http_t *info);

/**
 * receive response header
 * Args:
 *   info: header data info
 * Returns:
 *  success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.7.25
 */
int recv_response(http_t *info);

/**
 * Clean up all downloads
 * Args:
 *   info: header data info
 * Returns:
 *   success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.7.25
 */
void clean_up(http_t *info) ;

/**
 * download file func.
 * Args:
 *   url: Video Link Address
 *   save_path: Video File Save Address
 * Returns:
 *   success return 0, else return -1
 * Example:
 *   ./download https://www.baidu.com baidu.txt
 * @ author: Changyu Liu
 * @ last modify time: 2019.8.2
 */
int download(char *url, char *save_path);

#endif  // DLCV_DOWNLOAD_HPP