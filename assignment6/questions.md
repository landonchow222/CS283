1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines that a command’s output is fully received by either using explicit message boundaries (such as delimiters or length-prefixed messages) or detecting TCP connection closure (if the server signals completion by closing the socket). Since TCP is a stream-oriented protocol and does not preserve message boundaries, clients must implement their own mechanisms to detect message completion.

Techniques to handle partial reads and ensure complete message transmission:

Length-Prefixed Messages: The server first sends the length of the message, and the client reads until it has received the expected number of bytes.
Delimiters (e.g., Newline \n, Null \0): The server appends a special character or sequence to mark the end of a message.
Timeouts & Idle Detection: If a message is expected but no new data arrives within a certain time, assume message completion or error.
TCP Connection Closure: If the server closes the socket after sending the message, the client detects EOF and assumes the transmission is complete.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol must explicitly define how to frame commands and responses because TCP only guarantees an ordered byte stream, not distinct messages.

Methods to define and detect message boundaries in a networked shell protocol:

Fixed-Length Messages: Every command has a predefined length, ensuring that the receiver knows how many bytes to read.
Length-Prefixed Messages: The sender first transmits the length of the command as a fixed-size integer, followed by the actual command data.
Delimiters (e.g., \n, \0): Commands end with a specific character (like a newline for shell commands).
Application-Level Protocol (e.g., JSON, TLV encoding): A structured format like JSON or TLV (Type-Length-Value) ensures that messages are self-contained.

3. Describe the general differences between stateful and stateless protocols.

A stateful protocol maintains information about previous interactions, while a stateless protocol treats each request independently.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is "unreliable" because it does not guarantee delivery, order, or error correction, but it is still useful in many scenarios due to its low latency and efficiency.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The operating system (OS) provides the Socket API as an abstraction for network communication. This allows applications to send and receive data over networks without dealing with low-level hardware details.
