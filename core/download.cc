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

#include "../include/download.hpp"

using namespace std;

/**
 * Ignore Case and case.
 * Args:
 *  str: Strings to be compared
 *  sub: Original string
 * Returns:
 *  NULL
 * @ author: Changyu Liu
 * @ time: 2019.8.2
 */
char *strncasestr(char *str, const char *sub) {
  if (!str || !sub) return nullptr;

  int len = strlen(sub);
  if (len == 0) return nullptr;

  while (*str) {
    if (strncasecmp(str, sub, len) == 0) return str;
    ++str;
  }
  return nullptr;
}

/**
 * resolve domain.
 * Args:
 *  url: Incoming URL links
 *  info: Information parsed from incoming URL links
 * Returns:
 *  resolve success return 0, resolve faile return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.8.2
 */
int parser_URL(char *url, http_t *info) {
  char *tmp = url, *start = nullptr, *end = nullptr;
  int len = 0;

  /* 跳过http:// */
  if (strncasestr(tmp, "http://")) {
    tmp += strlen("http://");
  }
  start = tmp;
  if (!(tmp = strchr(start, '/'))) {
    lprintf(MSG_ERROR, "url invaild\n");
    return -1;
  }
  end = tmp;

  /*解析端口号和主机*/
  info->port = 80;   //先附默认值80

  len = _MIN(end - start, HOST_NAME_LEN - 1);
  strncpy(info->host_name, start, len);
  info->host_name[len] = '\0';

  if ((tmp = strchr(start, ':')) && tmp < end) {
    info->port = atoi(tmp + 1);
    if (info->port <= 0 || info->port >= 65535) {
      lprintf(MSG_ERROR, "url port invaild\n");
      return -1;
    }
    /* 覆盖之前的赋值 */
    len = _MIN(tmp - start, HOST_NAME_LEN - 1);
    strncpy(info->host_name, start, len);
    info->host_name[len] = '\0';
  }

  /* 复制uri */
  start = end;
  strncpy(info->url, start, URI_MAX_LEN - 1);

  lprintf(MSG_INFO, "parse url ok\nhost:%s, port:%d, uri:%s\n",
          info->host_name, info->port, info->url);
  return 0;
}


/**
 * resolve dns
 * Args:
 *   host_name: Domain Name Address to Resolve
 * Returns:
 *   resolve success return dns address, else return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.8.2
 */
unsigned long dns(char *host_name) {
  struct hostent *host;
  struct in_addr addr{};
  char **pp;

  host = gethostbyname(host_name);
  if (host == nullptr) {
    lprintf(MSG_ERROR, "gethostbyname %s failed\n", host_name);
    return -1;
  }

  pp = host->h_addr_list;

  if (*pp != NULL) {
    addr.s_addr = *((unsigned int *) *pp);
    lprintf(MSG_INFO, "%s address is %s\n", host_name, inet_ntoa(addr));
    return addr.s_addr;
  }

  return -1;
}

/**
 * set connect time out
 * Args:
 *   sock: Configure the connection status of Scoket
 * Returns:
 *   connet success return 0, else return -1.
 * @ author: Changyu Liu
 * @ last modifly time: 2019.8.2
 */
int set_socket_option(int sock) {
  struct timeval timeout;

  timeout.tv_sec = RCV_SND_TIMEOUT / 1000;
  timeout.tv_usec = RCV_SND_TIMEOUT % 1000 * 1000;
  lprintf(MSG_DEBUG, "%ds %dus\n", (int) timeout.tv_sec, (int) timeout.tv_usec);
  // set socket is success.

  // set send time out status
  if (-1 == setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout,
                       sizeof(struct timeval))) {
    lprintf(MSG_ERROR, "setsockopt error: %m\n");
    return -1;
  }

  // set receive time out staus
  if (-1 == setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout,
                       sizeof(struct timeval))) {
    lprintf(MSG_ERROR, "setsockopt error: %m\n");
    return -1;
  }

  return 0;
}

/**
 * connet to server func
 * Args:
 *   info: Information to connect to the server.
 * Returns:
 *   connect server success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.8.2
 * */
int connect_server(http_t *info) {
  int sockfd;
  struct sockaddr_in server;
  unsigned long addr = 0;
  unsigned short port = info->port;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == sockfd) {
    lprintf(MSG_ERROR, "socket create failed\n");
    close(sockfd);
    return -1;
  }

  if (-1 == set_socket_option(sockfd)) {
    close(sockfd);
    return -1;
  }

  if ((addr = dns(info->host_name)) == -1) {
    lprintf(MSG_ERROR, "Get Dns Failed\n");
    close(sockfd);
    return -1;
  }
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = addr;

  if (-1 ==
      connect(sockfd, (struct sockaddr *) &server, sizeof(struct sockaddr))) {
    lprintf(MSG_ERROR, "connect failed: %m\n");
    close(sockfd);
    return -1;
  }

  info->sock = sockfd;
  return 0;
}

/**
 * Send server requests
 * Args:
 *   info: Request information byte stream
 * Returns:
 *   Send the correct request bytes to the server
 * @ author: Changyu Liu
 * @ last modifly time: 2019.8.2
 */
int send_request(http_t *info) {

  memset(info->buffer, 0x0, RECV_BUF);
  snprintf(info->buffer, RECV_BUF - 1,
           "GET %s HTTP/1.1\r\n"
           "Accept: */*\r\n"
           "User-Agent: Mozilla/5.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n"
           "Host: %s\r\n"
           "Connection: Close\r\n\r\n",
           info->url, info->host_name);

  lprintf(MSG_DEBUG, "request:\n%s\n", info->buffer);
  return send(info->sock, info->buffer, strlen(info->buffer), 0);
}

/**
 * resolve response header
 * Args:
 *   info: Check whether the server header is status information
 * Returns:
 *   resolve success return 0, else return -1.
 * @ author: Changyu Liu
 * @ last modifly time: 2019.8.2
 */
int parse_http_header(http_t *info) {
  char *p = NULL;

  // resove first line
  fgets(info->buffer, RECV_BUF, info->in);
  p = strchr(info->buffer, ' ');
  // check http/https is legitimate
  if (!p || !strcasestr(info->buffer, "HTTP")) {
    lprintf(MSG_ERROR, "bad http head\n");
    return -1;
  }
  info->status_code = atoi(p + 1);
  lprintf(MSG_DEBUG, "http status code: %d\n", info->status_code);

  // Loop Read Resolve http Header
  while (fgets(info->buffer, RECV_BUF, info->in)) {
    // Judging whether the head has finished reading
    if (!strcmp(info->buffer, "\r\n")) return 0;

    lprintf(MSG_DEBUG, "%s", info->buffer);

    if ((p = strncasestr(info->buffer, (char *) "Content-length"))) {
      p = strchr(p, ':');
      p += 2;  // Skip the colon and the space behind it
      info->len = atoi(p);
      lprintf(MSG_INFO, "Content-length: %d\n", info->len);
    } else if ((p = strncasestr(info->buffer, (char *) "Transfer-Encoding")) != nullptr) {
      if ((strncasestr(info->buffer, (char *) "chunked"))) {
        info->chunked_flag = 1;
      } else {
        // Unsupported Coding Transfer.
        lprintf(MSG_ERROR, "Not support %s", info->buffer);
        return -1;
      }
      lprintf(MSG_INFO, "%s", info->buffer);
    } else {
      if ((p = strncasestr(info->buffer, (char *) "Location"))) {
        p = strchr(p, ':');
        p += 2;  // Skip the colon and the space behind it
        strncpy(info->location, p, URI_MAX_LEN - 1);
        lprintf(MSG_INFO, "Location: %s\n", info->location);
      }
    }
  }
  lprintf(MSG_ERROR, "bad http/https head\n");
  return -1;
}

/**
 * Save the content of the server response.
 * Args:
 *   info: Server connection information
 *   buf: Server buffer area
 *   len: Server buffer byte length
 * Returns:
 *   success return 0, else return -1.
 * @ author: Changyu Liu
 * @ last modifly time: 2019.8.2
 */
int save_data(http_t *info, const char *buf, int len) {
  int total_len = len;
  int write_len = 0;

  // open file
  if (!info->save_file) {
    info->save_file = fopen(info->save_path, "w");
    if (!info->save_file) {
      lprintf(MSG_ERROR, "fopen %s error: %m\n", info->save_path);
      return -1;
    }
  }

  while (total_len) {
    write_len = fwrite(buf, sizeof(char), len, info->save_file);
    if (write_len < len && errno != EINTR) {
      lprintf(MSG_ERROR, "fwrite error: %m\n");
      return -1;
    }
    total_len -= write_len;
  }
  return 0;
}

/**
 * read file data
 * Args:
 *   info: file information
 *   len: file buffer byte length
 * Returns:
 *   success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.8.2
 */
int read_data(http_t *info, int len) {
  int total_len = len;
  int read_len = 0;
  int rtn_len = 0;

  while (total_len) {
    read_len = _MIN(total_len, RECV_BUF);
    // lprintf(MSG_DEBUG, "need read len: %d\n", read_len);
    rtn_len = fread(info->buffer, sizeof(char), read_len, info->in);
    if (rtn_len < read_len) {
      if (ferror(info->in)) {
        if (errno == EINTR);
        else if (errno == EAGAIN || errno == EWOULDBLOCK)  // time out
        {
          lprintf(MSG_ERROR, "socket recvice timeout: %dms\n", RCV_SND_TIMEOUT);
          total_len -= rtn_len;
          lprintf(MSG_DEBUG, "read len: %d\n", rtn_len);
          break;
        } else  // other error
        {
          lprintf(MSG_ERROR, "fread error: %m\n");
          break;
        }
      } else  // read file end
      {
        lprintf(MSG_ERROR, "socket closed by peer\n");
        total_len -= rtn_len;
        lprintf(MSG_DEBUG, "read len: %d\n", rtn_len);
        break;
      }
    }

    total_len -= rtn_len;
    lprintf(MSG_DEBUG, "read len: %d\n", rtn_len);
    if (-1 == save_data(info, info->buffer, rtn_len)) {
      return -1;
    }
    info->recv_data_len += rtn_len;
  }
  if (total_len != 0) {
    lprintf(MSG_ERROR, "we need to read %d bytes, but read %d bytes now\n", len,
            len - total_len);
    return -1;
  }
  return 0;
}

/**
 * Chunked data sent back by receiving server
 * Args:
 *   info: Chunked data info
 * Returns:
 *   success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.8.2
 */
int recv_chunked_response(http_t *info) {
  long part_len;

  do {
    // Get the length of this section
    fgets(info->buffer, RECV_BUF, info->in);
    part_len = strtol(info->buffer, NULL, 16);
    lprintf(MSG_DEBUG, "part len: %ld\n", part_len);
    if (-1 == read_data(info, part_len)) return -1;

    // Read the r\n characters behind
    if (2 != fread(info->buffer, sizeof(char), 2, info->in)) {
      lprintf(MSG_ERROR, "fread \\r\\n error : %m\n");
      return -1;
    }
  } while (part_len);
  return 0;
}

/**
 * Calculate average download speed
 * Args:
 *   info: download file data info
 * Returns:
 *   success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.8.2
 */
float calc_download_speed(http_t *info) {
  int diff_time = 0;

  diff_time = info->end_recv_time - info->start_recv_time;
  /* 最小间隔1s，避免计算浮点数结果为inf */
  if (0 == diff_time) diff_time = 1;
  float speed = (float) info->recv_data_len / diff_time;

  return speed;
}

/**
 * receive response header
 * Args:
 *   info: header data info
 * Returns:
 *  success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.8.2
 */
int recv_response(http_t *info) {
  int total_len = info->len;

  if (info->chunked_flag) return recv_chunked_response(info);

  if (-1 == read_data(info, total_len)) return -1;

  return 0;
}

/**
 * Clean up all downloads
 * Args:
 *   info: header data info
 * Returns:
 *   success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modifly time: 2019.8.2
 */
void clean_up(http_t *info) {
  if (info->in) fclose(info->in);
  if (-1 != info->sock) close(info->sock);
  if (info->save_file) fclose(info->save_file);
  free(info);
}

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
int download(char *url, char *save_path) {
  printf("start download!\n");
  http_t *info = nullptr;
  char tmp[URI_MAX_LEN] = {0};

  if (!url || !save_path) {
    printf("check url or save path exists!\n");
    return -1;
  }

  // Initialization structure
  info = (http_t *) malloc(sizeof(http_t));
  if (!info) {
    lprintf(MSG_ERROR, "malloc failed\n");
    return -1;
  } else
    printf("malloc space successful!\n");
  memset(info, 0x0, sizeof(http_t));
  info->sock = -1;
  info->save_path = save_path;

  // resolve url
  if (-1 == parser_URL(url, info)) {
    printf("parer url error!\n");
    clean_up(info);
    return -1;
  }

  // Connect to server
  if (-1 == connect_server(info)) {
    clean_up(info);
    return -1;
  }

  // Send HTTP request message
  if (-1 == send_request(info)) {
    clean_up(info);
    return -1;
  }

  // Receiving the header information of the response
  info->in = fdopen(info->sock, "r");
  if (!info->in) {
    lprintf(MSG_ERROR, "fdopen error\n");
    clean_up(info);
    return -1;
  }

  // Analytical Head
  if (-1 == parse_http_header(info)) {
    clean_up(info);
    return -1;
  }
  if (info->status_code == HTTP_OK) {
    lprintf(MSG_DEBUG, "recv data now\n");
    info->start_recv_time = time(0);
    if (-1 == recv_response(info)) {
      clean_up(info);
      return -1;
    }

    info->end_recv_time = time(nullptr);
    lprintf(MSG_INFO, "recv %d bytes\n", info->recv_data_len);
    lprintf(MSG_INFO, "Average download speed: %.2fKB/s\n",
            calc_download_speed(info) / 1000);
  } else if (info->status_code == HTTP_REDIRECT) {
    lprintf(MSG_INFO, "redirect: %s\n", info->location);
    strncpy(tmp, info->location, URI_MAX_LEN - 1);
    clean_up(info);
    return download(tmp, save_path);
  } else if (info->status_code == HTTP_NOT_FOUND) {
    lprintf(MSG_ERROR, "Page not found\n");
    clean_up(info);
    return -1;
  } else {
    lprintf(MSG_INFO, "Not supported http code %d\n", info->status_code);
    clean_up(info);
  }

  clean_up(info);
  return 0;
}

int main(int argc, const char *argv[]) {
  download((char *) argv[1], (char *) argv[2]);
  return 0;
}