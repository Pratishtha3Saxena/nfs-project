# NFS (Network File System) Project

## Overview

This repository contains the source code for a Network File System (NFS) implemented in C. The project aims to provide a distributed file system that allows seamless file sharing and access across a network.


## Getting Started

### Prerequisites

- Ensure you have [C compiler](https://gcc.gnu.org/install/index.html) installed.

### Building

1. Clone the repository:

   ```bash
   git clone https://github.com/your-username/nfs-project.git
   cd nfs-project

2. Starting the naming server 
    ```bash 
    cd naming_server
    make clean;make
    ./NMS
3. Starting the storage server 
    Copy the code in 'storage_server' file in the directory you want to turn into a storage server's directory
    ```bash
    make clean;make
    ./SS <port_client_connection> <port_for_naming_server_connection>
    Example ./SS 8001 9001
4. Starting Client
    Goto home folder 
    ```bash
    cd client
    make
    ./a.out


    Perform Non Priority-Based Operations:



    Read File:
    
    READ <file_path>
    Example: READ /path/to/file.txt

    Write File:


    WRITE <file_path> 
    <content>
    Example: WRITE /path/to/file.txt "New content"

    Get Permissions:

    
    ADDITIONAL_INFO <file_path>
    Example: ADDITIONAL_INFO /path/to/file.txt


    The Naming Server (NS) will provide the client with the appropriate port of the connected Storage Server (SS) for the requested file operation. The Storage Server, once connected, is capable of handling multiple clients concurrently using a concurrency mechanism.

    Perform Priority-Based Operations:

    Create File:

    CREATE_FILE <file_path> 
    Example: CREATE_FILE /path/to/new_file.txt 

    Create Directory:

    CREATE_DIR <directory_path> 
    Example: CREATE_DIR /path/to/new_dir

    Delete File:

    DELETE_FILE <file_path>
    Example: DELETE_FILE /path/to/file_to_delete.txt

    Delete Directory:

    DELETE_DIR <directory_path>
    Example: DELETE_DIR /path/to/directory_to_delete

    Copy File:

    COPY_FILE <source_path> <destination_path>
    Example: COPY_FILE /path/to/source_file.txt /path/to/destination_directory


    Copy Dir:

    COPY_DIR <source_path> <destination_path>
    Example: COPY_DIR /path/to/source_directory /path/to/destination_directory


## Features

### The Naming Server (NMS)
The Naming Server (NS) autonomously issues commands to the connected Storage Server (SS) for priority-based operations (create, delete, and copy files) without direct client-to-SS connections.

### Multiple Clients Handling:

Multiple clients can simultaneously read the same file without any restrictions.
However, if one client starts writing to any part of the file, access to that file is denied to all other clients until the writing operation is complete.



### Redundancy and Server Disconnection:

The Naming Server (NS) automatically builds copies of every file from a connected Storage Server (SS) in two additional SSs when initially connected.

In the event of the main Storage Server going down, read operations can still be performed using the redundant copies. However, write operations are allowed only when the main server is available.

A ping message is displayed when the main server disconnects. Upon reconnection, the copies are checked for any updates, and the main server is updated accordingly.


## Additional Features
### Concurrency:

The system is designed to handle multiple clients and storage servers concurrently, ensuring efficient file access and management.

### Dynamic Scaling:

The NFS system supports dynamic scaling by allowing storage servers to join or leave the network seamlessly.

### Caching Mechanism (LRU):

Implement a caching mechanism based on the Least Recently Used (LRU) policy to enhance performance. This feature stores frequently accessed files in memory, ensuring quick access to recently used files while optimizing overall system efficiency. The LRU policy helps manage cache space by evicting the least recently used files when the cache reaches its capacity, maintaining a balance between memory utilization and performance.

### Efficient Searching Mechanism (Trie-based):

Enhance file lookup performance with a trie-based searching mechanism. This feature optimizes the process of locating files and directories within the NFS system, providing efficient and fast search operations. Tries are utilized to organize and index the file paths, resulting in a structured and scalable approach to handling large directory structures. The trie-based searching


## Conclusion

This NFS project represents a robust and feature-rich distributed file system designed to facilitate seamless file sharing and access in a networked environment. With advanced features such as concurrency, dynamic scaling, caching, efficient searching, security measures, and redundancy, the system is geared towards providing a reliable and efficient file management solution.

The implementation supports both non-priority and priority-based operations, allowing users to read, write, create, delete, and copy files with ease. The system's capability to handle multiple clients concurrently ensures efficient file access, while the redundancy mechanism adds an extra layer of reliability by maintaining copies of files across multiple storage servers.

The project's design philosophy emphasizes performance, scalability, and fault tolerance, making it suitable for a variety of use cases. As the system continues to evolve, additional features and optimizations may be introduced to further enhance its capabilities.

Thank you for exploring this NFS project. Feel free to contribute, provide feedback, or use it as a foundation for your own distributed file system endeavors. Happy coding!






