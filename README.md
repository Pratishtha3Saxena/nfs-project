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
Naming Server
The Naming Server(NS) servers as the central hub between the clients and the Storage servers. This is the central hub of file sharing and management for the clients to communicate with. The NS has the following key features:

It acts as an address resolver for clients to perform operations like Read,Write,Retrieve Info on the files in the storage server. It resolves the IP address and port number of the storage server for the client to communicate.
It allows the operation of privileged operations like Creating , Deleting and Copying Folders among storage servers by directly sending requests to the Storage Servers.
It ensures data redundancy and reliability of information by creating backups of all the information present in the storage servers and updating the backups continously.
It supports multi-client operation ie can cater to multiple client requests concurrently.

### Storage Server
The Storage Servers (SS) acts as the data stores In our distributed file system implementation in C, Storage Servers play a pivotal role as the backbone of the Network File System (NFS). These servers shoulder the critical responsibility of handling the physical storage and retrieval of files and folders within the network. Tasked with the management of data persistence, Storage Servers ensure that files are stored securely and efficiently, forming the bedrock of reliable file storage and access. By distributing data across multiple servers, our system aims to enhance performance, scalability, and fault tolerance, contributing to a robust and seamless file management experience for clients connected to the network. The Storage Servers, in essence, act as the guardians of data integrity, facilitating a distributed and resilient file storage infrastructure.
### Client
The client serves as a user interface to communicate with the Network File System, offering several essential functionalities:

Users can initiate a variety of requests such as Read, Write, Append, and more.
Robust error handling mechanisms are implemented on the client side.
The Network File System supports concurrent usage by multiple clients.

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






