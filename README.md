# 🐚 Simple Shell (hsh)

> A minimal UNIX command line interpreter written in C.

---

## 📌 Description
**Simple Shell** is a UNIX command line interpreter developed in C as part of
the Holberton School curriculum.  
The project focuses on understanding how a shell works internally by
recreating core features of the standard UNIX shell (`/bin/sh`).

The shell reads user input, parses commands, and executes programs using
system calls.

---

## 🧠 Background
A shell acts as an interface between the user and the operating system.
It allows users to execute commands, manage processes, and interact
with the system efficiently.

This project is inspired by the original UNIX shell created by **Ken Thompson**.

---

## ✨ Features
✔ Interactive mode  
✔ Non-interactive mode  
✔ Command execution with arguments  
✔ PATH environment variable handling  
✔ Built-in command:
- `exit` — exits the shell  
✔ Error handling similar to `/bin/sh`  
✔ No memory leaks  

---

## ⚙️ How It Works
1. Displays a prompt in interactive mode
2. Reads input using `getline`
3. Parses commands and arguments
4. Creates a child process with `fork`
5. Executes commands using `execve`
6. Parent process waits for the child to finish

---

## 🚀 Usage

### 🔧 Compilation
```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```
---

### 📄Interactive Mode
- **$ ./hsh**
- **($) /bin/ls**
- **hsh main.c shell.c**
- **($) exit**
### 📄 Non-Interactive Mode
- **$ echo "/bin/ls" | ./hsh**
- **hsh main.c shell.c**

---

### ❌ Error Handling

Error messages are printed in a format similar to /bin/sh.
The program name displayed matches argv[0].

Example:
```bash
$ echo "qwerty" | ./hsh
./hsh: 1: qwerty: not found
```
---

### 📋 Requirements
	•	Ubuntu 20.04 LTS
	•	gcc compiler
	•	Compilation flags:
	•	-Wall
	•	-Werror
	•	-Wextra
	•	-pedantic
	•	-std=gnu89
	•	Betty coding style
	•	Maximum 5 functions per file
	•	No memory leaks

---

### 🧩 Allowed Functions

Only approved system calls and functions are used, such as:
`fork, execve, wait, getline, malloc, free, access, stat,
`perror, write, and read.


---
### 🗂 Project Structure
 **holbertonschool-simple_shell/**

| File                 | Purpose                                |
| -------------------- | -------------------------------------- |
| `README.md`          | Documentation                          |
| `man_1_simple_shell` | Documentation                          |
| `AUTHORS`            | Documentation                          |
| `Simple shell 0.1`   | Display prompt and wait for user       |
| `Simple shell 0.2`   | Handle command lines without arguments |
| `Simple shell 0.3`   | Handle PATH and fork/execve            |
| `Simple shell 0.4`   | Implement exit built-in                |
| `Simple shell 1.0`   | Implement env built-in                 |

## 📝 Contributors

This project was developed and tested by:

- **Reem Abdulhadi Alshehri**
- **Maryam Alessa 13** 



