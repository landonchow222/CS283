1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  When memory is precious is a long time ago, it is extremely expensive to use `execvp` direclty. So, programmers tend to use `fork` to use the copy-on-write model. This is one of the reason why we use `fork/execvp`. Another reason is that with
    `fork`, we completely sepearate the memory with child and parent. This way we can replace the child process with other processes and there would be no accidental memory change between the two parent and child processes.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  We can see the integer value that fork returns. If it's less than 1 we know that there is a fork error.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() find the file/programs in system-defined directories or in $PATH. $PATH varibale plays a vital role in this process.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  There are two purposes I can think of right now. One is to finish the child process so that there would be no zombie process occurring. Another purpose is to view the exit bytes genearted by the child process. If we didn't call 'wait()',
    the chid process would become a zombie process.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  In the lecture, we just looked at the exit status. This is important because it gives the programmer an idea of whether the child process fully finishes or something went wrong along the process.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  I just used a boolean to detect the double quotes. If we are inside the quotes, ignore all spaces. THese quoted arguments are necessary because this is one of the way in Linux to pass-in space as a parameter. If not Linux will always get one-word argumetns.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I am following the instrutions from the lecture now so my code structure is way better than the pervious assignment. I replaced spaces with '\0' and also implement a bool to detect double quotation. Yes, there were challenges. First, I was dynamically allocating the struct and also dynamically allocating the array inside the struct. This results in weird behavior of the code in terms of freeing and setting memory to 0. 

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals are a simple form of IPC that is sent to processes which are running to trigger a specific behavior. The way I understand the difference between IPC and signal is that in IPC, processes communicate with each other whereas signals are one-way interrupt send to a process to stop/delay the normal flow of a process.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL:  Forcefully terminates a process. This is the worst form of terminating as no clean up is perfomred.
    SIGTERM: Requests a process to terminate. Allows a process to perform cleanup before exiting, considered more graceful than SIGKILL.
    SIGINT: Terminate the process. This is where we type ctrl-c from our keyboards. This is the similar to sigterm but the only difference is user has to press it from the keyboard.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  The SIGSTOP signal stops the process. It cannot be handled, ignored, or blocked unlike SIGTSTP which is more like an interactive signal. SIGSTOP cannot be caught or ignored like SIGINT because like SIGKILL, these are handled by Kernel to deal with unresponsive processes. It's like a way to forcefully exit or pause the process.
