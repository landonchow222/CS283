1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() stops reading when it encounters either the end-of-file (EOF) or a newline character. Once it stops, it stores the input in the buffer and appends a null-terminating byte ('\0').

Since our goal is to read user input line by line, fgets() is a suitable choice for this application.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  The primary reason for using malloc() instead of a fixed-size array is to avoid stack overflow errors. Since heap memory is significantly larger than stack memory, dynamically allocating memory allows us to handle large user inputs without exceeding stack limits.

Additionally, using malloc() provides flexibility in memory management. Once the buffer is no longer needed, we can free the allocated memory, making it available for other purposes.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  The primary reason for trimming spaces is to ensure that the shell correctly processes the input and accurately executes the intended commands. Without trimming, extra spaces could cause identical commands to appear different, leading to incorrect comparisons and unexpected behavior.
4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Input Redirection – Our shell should support redirecting input from the user into a file. This isn't particularly challenging since we are already capturing user input from stdin and writing it to a file.

Output Redirection – Our shell should be able to redirect command output into a file. This is more complex because some values are returned, while others are printed. We need to ensure that both types of output are redirected properly without displaying them on the shell.

Combined Redirection – Our shell should support redirecting both stdout and stderr to the same file. While conceptually similar to output redirection, this introduces an additional challenge of handling the correct syntax for merging these streams
    

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection involves saving a program's input or output to a file, while piping directs the output of one command as input to another command. 

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  The primary reason for keeping output and errors separate is to address the semipredicate problem, which ensures that outputs and errors can be distinguished. In programming, a semipredicate problem arises when a function returns a meaningful value but also uses a valid return value to indicate failure, making it unclear whether the result signifies success or an error.

In a shell, separating standard output (stdout) and standard error (stderr) prevents them from being mixed when redirecting or piping. If both were combined into a single stream, it would be difficult to differentiate between actual output and error messages.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  The configuration can be customized based on user preference. However, a common convention is that a negative return value indicates a command failure, while a non-negative return value signifies success. Users can check the status of the last executed command by typing $?.

Our shell should also display error messages in the terminal so users can see them even when redirecting or piping. Additionally, we should provide a way to merge STDOUT and STDERR. One approach could be using special characters (e.g., >$, <$, >>$, <<$) to include headers like "output" and "error" for differentiation. Users could also omit $ to make the shell behave like a standard Linux shell, and use & to redirect merged outputs into a file.