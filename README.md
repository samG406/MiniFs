#  MiniFS – Miniature File System Simulator

##  Introduction

**MiniFS** is a low-level file system emulator written in C that simulates essential behaviors of real-world file systems.  MiniFS offers hands-on exposure to core operating system concepts, such as block-level storage, file metadata management, in-memory caching, lazy writes, and file system consistency verification.

Built atop a virtual disk represented by a binary file, MiniFS hides the complexity of disk operations while exposing a user-friendly command-line interface for performing common file operations—such as creation, reading, writing, deletion, checking, and snapshotting.

This modular and extensible system is a powerful tool for students and educators alike, aiming to illuminate how file systems work behind the scenes.

---

##  Objectives

The core goals of the MiniFS project include:

- **Block-Based Storage Simulation**  
  - Use a 1MB virtual disk file (`disk.img`) divided into 1KB blocks.
  - Store and manage files within block allocations.

- **File Metadata Management**  
  - Maintain a file table with metadata (file name, size, starting block, status, unique ID).

- **Basic File Operations**  
  - Implement core commands: `open`, `write`, `read`, `delete`, `verify`, `flush`, `fsck`, `snapshot`.

- **Performance Optimization via Caching**  
  - Use an in-memory cache for dirty blocks.
  - Delay disk writes until a `flush` or `close` operation (lazy write strategy).

- **File System Consistency Checking**  
  - Include a built-in `fsck` utility to detect and report inconsistencies in the file table.

- **Snapshot and Recovery Support**  
  - Capture full state of the disk for backup or recovery with the `snapshot` command.

- **Modular and Extensible Architecture**  
  - Designed for future features such as journaling, block reuse, and performance metrics.
  - Easily upgradeable with new capabilities.

---

##   System Design

- **Disk Simulation**  
  - Virtual disk (`disk.img`) of 1MB size.
  - Divided into 1024 blocks of 1KB each.

- **File Table**  
  - Stores metadata for up to 128 files.
  - Metadata includes: file name, file size, start block, usage flag, unique timestamp ID.

- **In-Memory Cache & Lazy Writes**  
  - Write operations go to memory cache first.
  - Data is only committed to disk during `flush` or file `close`.

- **CLI Interface**  
  - Terminal commands allow interaction with the file system.

---

## Features Implemented

1. **Disk Initialization**
   - Command: `init`
   - Creates and formats the virtual disk.

2. **File Metadata Management**
   - File table holds up to 128 entries with name, size, start block, status flag, unique ID.

3. **File Operations**
   - `open`: Creates a new file if it doesn't exist.
   - `write`: Writes to the in-memory cache.
   - `read`: Reads directly from the disk.
   - `delete`: Removes a file and frees associated blocks.
   - `verify`: Displays current file metadata.
   - `flush`: Writes all cached data to disk.
   - `fsck`: Checks metadata consistency.
   - `snapshot`: Creates a backup of the disk image.
   - `setmode`: Switches between eager and lazy write modes.

4. **Performance Optimization**
   - Caching reduces unnecessary disk I/O by batching writes.

---

##  CLI Commands

```bash
./minifs init                         # Initialize the virtual disk
./minifs open filename.txt            # Create/open a file
./minifs write filename.txt "Hello"   # Write data to the file
./minifs read filename.txt            # Read data from the file
./minifs delete filename.txt          # Delete the file
./minifs flush                        # Flush cache to disk
./minifs fsck                         # Check file system consistency
./minifs snapshot                     # Create a disk snapshot
./minifs verify                       # List file metadata
./minifs setmode [eager|lazy]         # Switch write mode
