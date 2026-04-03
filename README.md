# Operating Systems

This repository contains coursework materials for the CS330 Operating Systems course, encompassing 11 comprehensive lab assignments and several system-level projects designed to provide hands-on experience with core operating systems concepts.

## Technology Stack

This repository is primarily written in **C (92.8%)**, with components in **Assembly (3.1%)** and **Shell Scripts (2.8%)**, reflecting the systems programming nature of operating systems coursework.

---

## Lab Assignments

### Lab 1
Introduction to operating systems fundamentals and basic system programming. Students learn foundational concepts including processes, system calls, and basic I/O operations through practical exercises.

### Lab 2
Focuses on process management and inter-process communication (IPC). Covers concepts like pipes, file descriptors, process creation, and synchronization primitives.

### Lab 3
**⚠️ Note:** Lab 3 was an examination-based assessment. Course content and solutions are not available publicly due to academic integrity policies.

### Lab 4
Delves into advanced memory management concepts including virtual memory, paging, and memory allocation strategies. Students implement memory management algorithms.

### Lab 5
Explores file systems and I/O operations in depth. Covers file descriptors, file operations, directory management, and system-level file handling.

### Lab 6
Focuses on process synchronization, mutual exclusion, and concurrency control. Includes semaphores, mutexes, and deadlock prevention techniques.

### Lab 7
Advanced process scheduling and CPU scheduling algorithms. Students implement and analyze different scheduling strategies and their performance implications.

### Lab 8
**⚠️ Note:** Lab 8 was an examination-based assessment. Course content and solutions are not available publicly due to academic integrity policies.

### Lab 9
Covers signal handling, inter-process communication mechanisms, and advanced system programming concepts.

### Lab 10
**⚠️ Note:** Lab 10 was an examination-based assessment. Course content and solutions are not available publicly due to academic integrity policies.

### Lab 11
Comprehensive systems project integrating multiple OS concepts learned throughout the course. Final assignment combining process management, memory management, and I/O operations.

---

## Project Modules

### fileAPI/
Implementation of file I/O operations and file handling abstractions. Provides practical experience with system calls related to file operations, including open, read, write, close, and seek operations.

### execshell/
Shell implementation project focusing on process execution and command interpretation. Students build a minimal shell that can parse and execute system commands, demonstrating understanding of process creation and management.

### gemOS/
Custom operating system kernel implementation project. This is an advanced module where students implement core OS functionality including process management, scheduling, memory management, and interrupt handling.

### memapi/
Memory management API implementation. Covers memory allocation, deallocation, and various memory management strategies. Students work with heap management and memory allocation algorithms.

---

## Supporting Files

### altpipe.txt
Quick reference guide for pipe system calls. Contains code snippets showing the equivalence between the `pipe()` function and the direct `syscall(SYS_pipe, fd)` approach, including necessary header files for system call programming.

### linux man pages.pdf
Comprehensive Linux manual pages reference document (16.6 MB). Serves as a reference for all system calls, library functions, and command-line utilities used throughout the coursework.

---

## Key Concepts Covered

- **Process Management:** Process creation, forking, process states, and lifecycle
- **Inter-Process Communication (IPC):** Pipes, message queues, shared memory, and signals
- **Synchronization:** Mutual exclusion, semaphores, mutexes, condition variables
- **Memory Management:** Virtual memory, paging, segmentation, memory allocation strategies
- **File Systems:** File operations, directory structures, inodes, file descriptors
- **CPU Scheduling:** Scheduling algorithms, context switching, CPU utilization
- **Signal Handling:** Signal mechanisms, handlers, and asynchronous event processing

---

## Repository Structure
```
Operating_Systems_CS330/
├── Lab 1/          # OS fundamentals and basic system programming
├── Lab 2/          # Process management and IPC
├── Lab 4/          # Memory management
├── Lab 5/          # File systems and I/O
├── Lab 6/          # Process synchronization
├── Lab 7/          # CPU scheduling
├── Lab 9/          # Signal handling and advanced IPC
├── Lab 11/         # Comprehensive systems integration project
├── fileAPI/        # File I/O abstraction module
├── execshell/      # Shell implementation
├── gemOS/          # Custom OS kernel
├── memapi/         # Memory management API
├── altpipe.txt     # Pipe syscall reference
└── linux man pages.pdf  # Linux documentation reference
```

---

## Learning Outcomes

Upon completing this coursework, students will have:

- Gained deep understanding of operating systems architecture and design
- Implemented core OS components from scratch
- Mastered low-level systems programming in C and Assembly
- Understood process and thread management, synchronization, and scheduling
- Learned file system concepts and I/O operations
- Practiced debugging and optimizing systems-level code
- Developed skills in systems programming, memory management, and concurrent programming

---

## Notes

- Labs 3, 8, and 10 were examination-based assessments and their content is not included in this public repository.
- All code examples follow Linux/POSIX standards and practices.
- Students are encouraged to review the Linux man pages PDF for detailed system call documentation.
- Each lab directory contains submission folders with completed assignments and reference implementations.
