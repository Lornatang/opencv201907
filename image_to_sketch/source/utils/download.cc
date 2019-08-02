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


#include "../../include/download.hpp"

void print_error(const char *message) {
  perror(message);
  exit(1);
}

int get_http_info(const char *host_name, HTTP_INFO *http_info) {
  int a, b, c, d;
  if (sscanf(host_name, "%d.%d.%d.%d", &a, &b, &c, &d) == 4) {
    if (a >= 0x0 && a <= 0xff && b >= 0x0 && b <= 0xff && c >= 0 && c <= 0xff && d >= 0 && d <= 0xff) {
      strcpy(http_info->ip_addr, host_name);
      strcpy(http_info->aliases, "no aliases");
      return 0;
    }
  }
  struct hostent *phost = gethostbyname(host_name);
  if (!phost)
    return -1;

  inet_ntop(AF_INET, phost->h_addr_list[0], http_info->ip_addr, sizeof(http_info->ip_addr));
  if (*phost->h_aliases != nullptr)
    strcpy(http_info->aliases, phost->h_aliases[0]);
  else
    strcpy(http_info->aliases, "no aliases");
  return 0;
}

int send_http_header(int sfd, HOST_INFO host_info) {
  char http_header[BUFSIZ];
  bzero(http_header, sizeof(http_header));
  sprintf(http_header, \
      "GET %s HTTP/1.1\r\n"\
      "Host: %s\r\n"\
      "Connection: Keep-Alive\r\n"\
      "Content-Type: application/octet-stream\r\n"\
      "\r\n", host_info.file_path, host_info.host_name);
  return write(sfd, http_header, strlen(http_header));
}

int parse_http_header(int sfd, HTTP_INFO *http_info) {
  char buffer[BUFSIZ], temp[BUFSIZ], *ptr;
  bzero(buffer, sizeof(buffer));
  bzero(temp, sizeof(temp));
  int len, n = 0;
  while ((len = read(sfd, buffer, 1)) != 0) {
    temp[n] = *buffer;
    if (*buffer == '\n') {
      ptr = strstr(temp, "HTTP/1.1");
      if (ptr != nullptr) {
        ptr = strchr(ptr, ' ');
        ptr++;
        http_info->status_code = atoi(ptr);
      }
      ptr = strstr(temp, "Content-Length:");
      if (ptr != nullptr) {
        ptr = strchr(ptr, ':');
        ptr++;
        http_info->file_size = strtoul(ptr, nullptr, 10);
      }
      ptr = strstr(temp, "Content-Type:");
      if (ptr != nullptr) {
        ptr = strchr(ptr, ':');
        ptr++;
        strcpy(http_info->content_type, ptr);
        http_info->content_type[strlen(ptr) - 1] = '\0';
      }
      if (temp[0] == '\r' && temp[1] == '\n')
        break;
      bzero(temp, sizeof(temp));
      n = -1;
    }
    n++;
  }
  return sfd;
}

int connect_server(const char *ip_addr, unsigned short port) {
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1)
    print_error("socket");

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;

  serv_addr.sin_port = htons(port);

  int ret = inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr.s_addr);
  if (ret == -1)
    print_error("inet_pton");

  int flags = fcntl(sfd, F_GETFL, 0);

  flags |= O_NONBLOCK;
  fcntl(sfd, F_SETFL, flags);

  ret = connect(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  if (ret != 0 && errno == EINPROGRESS) {
    printf("connecting server,please wait for 10 seconds\n");
    struct pollfd pfd;
    socklen_t optlen;
    int optval, timeout = 10 * 1000;
    pfd.fd = sfd;
    pfd.events = POLLOUT;

    if ((ret = poll(&pfd, 1, timeout)) == 1) {
      optlen = sizeof(optval);
      ret = getsockopt(sfd, SOL_SOCKET, SO_ERROR, &optval, &optlen);
      errno = optval;
      ret = optval == 0 ? 0 : -1;
      if (!ret)
        printf("connection %s\n", strerror(errno));
    } else if (!ret) {
      errno = ETIMEDOUT;
      ret = -1;
    } else
      print_error("poll failed");
  }
  flags &= ~O_NONBLOCK;
  fcntl(sfd, F_SETFL, flags);
  return ret == -1 ? -1 : sfd;
}

//void print_progress_bar(const char *file_name, float sum, float file_size) {
//  float percent = (sum / file_size) * 100;
//  char *sign = "#";
//  if ((int) percent != 0) {
//    sign = (char *) malloc((int) percent + 1);
//    strncpy(sign, "####################################################", (int) percent);
//  }
//  printf("%s %7.2f%% [%-*.*s] %.2f/%.2f mb\r", file_name, percent, 50, (int) percent / 2, sign, sum / 1024.0 / 1024.0,
//         file_size / 1024.0 / 1024.0);
//  if ((int) percent != 0)
//    free(sign);
//  fflush(stdout);
//}

unsigned long download(int sfd, HOST_INFO host_info, HTTP_INFO http_info) {
  umask(0111);
  int len;
  unsigned long sum = 0;
  char buffer[BUFSIZ] = {0};
  int fd = open(host_info.new_name, O_CREAT | O_WRONLY | O_TRUNC, 0777);
  if (fd == -1)
    print_error("open");
  while ((len = read(sfd, buffer, sizeof(buffer))) > 0) {
    write(fd, buffer, len);
    sum += len;
    // print_progress_bar(host_info.new_name, (float) sum, (float) http_info.file_size);
    if (http_info.file_size == sum) {
      printf("\n");
      break;
    }
  }
  close(fd);
  close(sfd);
  return sum;
}

void parse_http_url(char *url, HOST_INFO *host_info) {
  const char *protocol[] = {"http://", "https://", nullptr};
  int i, len = 0;

  for (i = 0; protocol[i] != nullptr; i++)
    if (!strncmp(url, protocol[i], strlen(protocol[i]))) {
      len = strlen(protocol[i]);
      break;
    }


  char *ptr = strchr(url + len, ':');
  if (ptr != nullptr) {
    strncpy(host_info->host_name, url + len, strlen(url + len) - strlen(ptr));
    sscanf(++ptr, "%5hu", &host_info->port);
    if (host_info->port > 65535) {
      printf("invalid port\n");
      exit(1);
    }
    while (*ptr != '/')
      ptr++;
    strcpy(host_info->file_path, ptr);
    ptr = strrchr(host_info->file_path, '/');
    ptr++;
    strcpy(host_info->new_name, ptr);
  } else {
    ptr = strchr(url + len, '/');
    strncpy(host_info->host_name, url + len, strlen(url + len) - strlen(ptr));
    host_info->port = 80;
    strcpy(host_info->file_path, ptr);
    ptr = strrchr(host_info->file_path, '/');
    ptr++;
    strcpy(host_info->new_name, ptr);
  }
}

unsigned long get_file_size(const char *file_name) {
  struct stat buf{};
  if (stat(file_name, &buf) == -1)
    return 0;
  else
    return buf.st_size;
}

int download(char *url, char *fileName) {
  bzero(&host_info, sizeof(host_info));
  bzero(&http_info, sizeof(http_info));
  parse_http_url(url, &host_info);

  if (fileName != nullptr)
    strcpy(host_info.new_name, fileName);

  if (get_http_info(host_info.host_name, &http_info) == -1)
    print_error("gethostbyname");

  int sfd = connect_server(http_info.ip_addr, host_info.port);
  if (sfd == -1)
    print_error("connect failed");
  printf("waiting for http response\n");
  int ret = send_http_header(sfd, host_info);
  if (ret == -1)
    print_error("write");

  sfd = parse_http_header(sfd, &http_info);

  printf("http response:\n\tstatus code: %d\n", http_info.status_code);
  printf("\thost: %s:%hu\n", http_info.ip_addr, host_info.port);
  printf("\taliases: %s\n", http_info.aliases);
  printf("\tcontent-type:%s\n", http_info.content_type);
  if (http_info.file_size > 1024 * 1024)
    printf("\tcontent-length: %.2f mb\n", (float) http_info.file_size / 1024.0 / 1024.0);
  else
    printf("\tcontent-length: %lu bytes\n", http_info.file_size);

  if (http_info.status_code != 200)
    printf("warning:not found file %s\n", host_info.new_name);

  unsigned long download_size = download(sfd, host_info, http_info);
  unsigned long file_size = get_file_size(host_info.new_name);
  if (download_size != file_size) {
    printf("download %s failure\n", host_info.new_name);
    remove(host_info.new_name);
  }
  return 0;
}
