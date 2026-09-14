# C HTTP Server

A multi-client HTTP server built in C using TCP socket programming and `poll()`. The server handles HTTP requests, serves local files, and supports `GET` and `HEAD` requests.

## Features

- TCP socket communication
- Handles multiple client connections using `poll()`
- Supports HTTP `GET` and `HEAD` requests
- Serves local files
- Handles invalid or missing files
- HTTP response handling

## Technologies

- C
- TCP/IP
- HTTP
- POSIX Sockets
- `poll()`
- GCC

## How to Run

### 1. Clone the repository

```bash
git clone https://github.com/Adnan-Abdulle/C-HTTP-Server.git
cd C-HTTP-Server
```

### 2. Compile

```bash
gcc *.c -o server
```

### 3. Start the server

```bash
./server
```

The server runs on:

```text
localhost:8080
```

### 4. Test the server

Open another terminal and send a GET request:

```bash
curl http://localhost:8080/<filename>
```

Send a HEAD request:

```bash
curl -I http://localhost:8080/<filename>
```

You can also access the server from a browser:

```text
http://localhost:8080/<filename>
```


## Author

Adnan Abdulle
