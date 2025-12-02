# Socket-Programming

Socket programming in C on Linux provides a means for two processes, potentially on different machines, to communicate over a network. It involves using system calls to create and manage "sockets," which act as endpoints for network communication.

---

## Project overview

This repository contains a collection of C network programming examples and supporting materials focused on socket programming concepts (TCP and UDP), server models (single-process, multi-process, multi-thread, select/IO multiplexing), and small utility programs (for example: getting host IP).

The code is primarily written in C with some Makefiles included. It appears intended for learning and experimenting with different socket/server designs.

### What the folders likely contain

- **.vscode/** — Editor settings or workspace tasks used for development in VS Code.
- **00.Materials/** — Supporting materials such as lecture notes, slides, or reference files used alongside the examples.
- **01.Basic Web Server(TCP)/** — Minimal TCP server (and possibly a matching client) that accepts connections and sends static responses (good for learning the basics of `socket()`, `bind()`, `listen()`, `accept()`).
- **02.Simplex Web Server(TCP)/** — Simplex (one-directional) TCP example demonstrating sending or receiving in one direction.
- **03.Get IP(Host)/** — Utility program(s) to obtain the IP address of the host or perform DNS lookups (e.g., `gethostname()`, `gethostbyname()` / `getaddrinfo()`).
- **04.Multi Process Web Server/** — Example implementing a preforking or fork-per-connection server using `fork()` to handle clients concurrently.
- **05.Multi Thread Web Server/** — Multithreaded server example using `pthread`s to handle multiple client connections in parallel.
- **06.Multi Client With Select/** — Example that demonstrates `select()` for handling multiple clients without using threads or processes (I/O multiplexing).
- **07.Select Function/** — Focused examples showing how `select()` works (fd_sets, timeouts, read/write sets, etc.).
- **08.Basic Web Server(UDP)/** — Example of a basic UDP server and client, showing `recvfrom()`/`sendto()` and connectionless communication.

## How to run the examples

> These are general instructions — exact filenames or Makefile targets may differ. Check each folder for a `Makefile` or `.c` files.

1. Clone the repository (if you haven't already):

```bash
git clone https://github.com/prahadeeshwaran-k/Socket-Programming.git
cd Socket-Programming
```

2. Inspect a folder, build and run an example. Example (replace with actual filenames found in the folder):

```bash
cd 01.Basic\ Web\ Server(TCP)
ls -la
# If there's a Makefile
make
# or compile manually
gcc -o basic_server server.c
./basic_server 8080
```

3. For clients, open another terminal and connect using `telnet` / `nc` (netcat) or the included client executable:

```bash
telnet 127.0.0.1 8080
# or
nc 127.0.0.1 8080
```

4. For UDP examples, use `nc -u` or the provided client program.

## Learning notes & tips

- Use `strace` (Linux) to trace system calls and better understand queueing/accept behavior.
- For multi-threaded servers, watch for shared-data races; use mutexes when modifying global state.
- For select-based servers, carefully manage file descriptor sets and update `nfds` (max fd + 1).
- Use `SO_REUSEADDR` when restarting servers rapidly during development.


---


