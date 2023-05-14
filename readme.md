# ThreadPool
This is a CMD tool that implement a threadpool mechanism, and implement syncronization 
The objective is to leverage multi-threading to improve the performance of an encryption algorithm in a multi-core system.
Many web-servers use the same approach to suport multiple clients in the same time.

# Task Description
You are provided with a shared object (SO) library that contains two functions: encode and decode. Additionally, a simple tester-main program is included to demonstrate the basic functionality of the library. The encryption algorithm is not optimized and takes approximately 10ms for each character. It can handle a maximum of 1024 bytes of data; anything beyond that will be ignored.

Your task is to implement a command-line tool that utilizes the aforementioned algorithm. The tool should accept input from standard input (stdin) and write the encrypted/decrypted data to standard output (stdout). The tool should use the -e flag for encryption and the -d flag for decryption.


Usage Example
To encrypt a file named my_original_file and store the result in encrypted_file:
```bash
coder key -e < my_original_file > encrypted_file
```

To decrypt a file named my_encrypted_file and restore the original content to my_original_file:
```bash
coder key -d < my_encrypted_file > my_original_file
```