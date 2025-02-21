1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  when memory was scarce, using `execvp` directly was highly expensive. To optimize resource usage, programmers preferred using `fork` to take advantage of the copy-on-write model. This is one reason why the `fork/execvp` pattern is commonly used. Another benefit of `fork` is that it completely separates the memory space of the parent and child processes. This isolation ensures that the child process can be replaced with another process without any unintended memory modifications between the parent and child.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  The return value of `fork` can be examined to determine its status. If the returned value is less than 1, it indicates that a fork error has occurred.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  `execvp()` searches for the specified file or program in system-defined directories and those listed in the `$PATH` variable. The `$PATH` variable is crucial in this process.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  I can think of two main purposes. First, calling `wait()` ensures that the child process is properly terminated, preventing the creation of a zombie process. Second, it allows us to retrieve the exit status of the child process. Without `wait()`, the child process would remain as a zombie process.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  In the lecture, we focused on examining the exit status, which is important because it helps the programmer determine whether the child process completed successfully or encountered an issue during execution.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  I used a boolean flag to track double quotes. When inside quotes, all spaces are ignored. Quoted arguments are essential in Linux because they allow spaces to be included as part of a parameter; otherwise, Linux interprets each word as a separate argument.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I'm following the lecture instructions this time, so my code structure is much improved compared to the previous assignment. I replaced spaces with `'\0'` and implemented a boolean flag to detect double quotes. However, I faced some challenges. Initially, I was dynamically allocating both the struct and the array inside it, which led to unexpected behavior when freeing memory and resetting values to zero.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals are a basic form of inter-process communication (IPC) used to send messages to running processes, triggering specific behaviors. The way I see the difference between IPC and signals is that IPC allows processes to exchange information, whereas signals are one-way interrupts sent to a process to pause, stop, or alter its normal execution flow.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  - **SIGKILL**: Forcefully terminates a process without allowing any cleanup. It is the harshest way to end a process.  
- **SIGTERM**: Politely requests a process to terminate, giving it a chance to perform cleanup before exiting. It is a more graceful alternative to SIGKILL.  
- **SIGINT**: Terminates a process when triggered by the user pressing *Ctrl+C* on the keyboard. It functions similarly to SIGTERM, but requires manual input.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  The **SIGSTOP** signal halts a process and cannot be handled, ignored, or blocked, unlike **SIGTSTP**, which is an interactive signal. Unlike **SIGINT**, which can be caught or ignored, **SIGSTOP**—similar to **SIGKILL**—is managed by the kernel to handle unresponsive processes. It serves as a way to forcibly pause or suspend a process.
