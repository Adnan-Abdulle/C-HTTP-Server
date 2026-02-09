#include <stdio.h>
#include "read_write.h"
#include "errors.h"
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include "socket.h"
#define MAX_METHOD 16
#define MAX_PATH 2048
#define MAX_POLL 10

ssize_t readFile(int fd);
void handle_get(int client, char* path);
void handle_head(int client, char* path);
void handle_request(int client, char* buff);
void poll_loop(int fd);
void start_server(void);



ssize_t readFile(int fd){

    if (fd == -1) {
        perror("Error: open failed");
        return -1;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Error: fstat failed");
        return -1;
    }
    if (!S_ISREG(sb.st_mode)){
        fprintf(stderr, "Error: This is not a regular file\n");
        return -1;
    }

    return sb.st_size;
}

void handle_get(int client, char* path) {
    ssize_t n;
    char fullPath[MAX_PATH];
    char fileBuff[4096];
    char header[256];

    snprintf(fullPath, sizeof(fullPath), "test/%s", path +1);

    int file = open(fullPath, O_RDONLY);

    if (file < 0) {
        file_not_found(client);
        return;
    }

    ssize_t fileSize = readFile(file);

    snprintf(header, sizeof(header),"HTTP/1.1 200 OK\r\n"
                                    "Content-Type: text/html\r\n"
                                    "Content-Length: %zd\r\n\r\n", fileSize);

    write_fully(client, header, strlen(header));
    while ( (n = read(file,fileBuff, sizeof(fileBuff))) > 0){
        write_fully(client, fileBuff,(size_t)n);
    }
    close(file);
}

void handle_head(int client, char* path) {

    char header[256];

    char fullPath[MAX_PATH];

    snprintf(fullPath, sizeof(fullPath), "test/%s", path +1);

    int file = open(fullPath, O_RDONLY);
    if (file < 0){
        file_not_found(client);
        return;
    }

    ssize_t fileSize = readFile(file);

    snprintf(header, sizeof(header),"HTTP/1.1 200 OK\r\n"
                                    "Content-Type: text/html\r\n"
                                    "Content-Length: %zd\r\n\r\n", fileSize);
    write_fully(client, header, strlen(header));

    close(file);
}


void handle_request(int client, char* buff){
    char method[MAX_METHOD];
    char path[MAX_PATH];

    sscanf(buff,"%s %s", method, path);

    if (strcmp(method, "GET") == 0){
        handle_get(client, path);
    } else if (strcmp(method, "HEAD") == 0){
        handle_head(client, path);
    } else {
        not_implemented(client);
    }

}

void poll_loop(int fd) {

    struct pollfd poll_fds[MAX_POLL];
    int client;
    poll_fds[0].fd = fd;
    poll_fds[0].events = POLLIN;
    int count = 1;
    while (1) {

        if (poll(poll_fds,(nfds_t)count, -1) < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("poll failed");
            return;
        }

        if (poll_fds[0].revents & POLLIN) {
            client = accept_socket(fd);
            if (client == -1) {
                if (errno == EINTR) {
                    continue;
                }
                perror("accept");
                continue;
            }
            if (count >= MAX_POLL){
                close(client);
                continue;
            }
            printf("\nClient connected, handling request...\n");
            poll_fds[count].fd = client;
            poll_fds[count].events = POLLIN;
            count++;
        }
        for (int i = 1; i < count; ++i) {

            if (poll_fds[i].revents & POLLIN){
                char buff[1024];
                ssize_t n = read(poll_fds[i].fd, buff, sizeof(buff) - 1);
                if (n <= 0) {
                    close(poll_fds[i].fd);
                    poll_fds[i] = poll_fds[count - 1];
                    count--;
                    i--;
                    continue;
                }
                buff[n] = '\0';
                handle_request(poll_fds[i].fd, buff);

                shutdown(poll_fds[i].fd, SHUT_WR);
                close(poll_fds[i].fd);
                poll_fds[i] = poll_fds[count - 1];
                count--;
                i--;
            }
        }
    }
}


void start_server(void){

    int fd = make_socket();
    bind_socket(fd);
    listen_socket(fd);

    printf("Server successfully created!!\n");

    poll_loop(fd);

}


int main(void) {

    start_server();
    return 0;
}
