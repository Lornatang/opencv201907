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

#ifndef ITS_DOWNLOAD_HPP
#define ITS_DOWNLOAD_HPP

#include <sys/ioctl.h>
#include <poll.h>
#include <ctime>
#include <sys/socket.h>
#include <cctype>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <strings.h>
#include <cstring>
#include <cerrno>

typedef struct HTTP_INFO {
  unsigned long file_size;
  char content_type[256];
  char ip_addr[INET_ADDRSTRLEN];
  char aliases[256];
  int status_code;
} HTTP_INFO;

typedef struct HOST_INFO {
  char host_name[256];
  char file_path[1024];
  unsigned short port;
  char new_name[256];
} HOST_INFO;

HOST_INFO host_info;
HTTP_INFO http_info;

void print_error(const char *message);

int get_http_info(const char *host_name, HTTP_INFO *http_info);

int send_http_header(int sfd, HOST_INFO host_info);

int parse_http_header(int sfd, HTTP_INFO *http_info);

int connect_server(const char *ip_addr, unsigned short port);

// void print_progress_bar(const char *file_name, float sum, float file_size);

unsigned long download(int sfd, HOST_INFO host_info, HTTP_INFO http_info);

void parse_http_url(char *url, HOST_INFO *host_info);

unsigned long get_file_size(const char *file_name);

int download(char *url, char *fileName);

#endif  // ITS_DOWNLOAD_HPP