# Overview
- A Peer-to-Peer file sharing system for peers to share file among themselves.
- Hybrid architecture is used where few trackers are responsible for handling the peer requests and enabling them to share files among themselves.
- Files are shared among peers where chunks of a single file are fetched from multiple peers simultaneously to speed up the process.
- This reduces the load on a single server for downloading files and enables higher speed transfers by downloading file from multiple peers.

# Architecture
- 

# Execution
1. Create user
```
create_user <username> <password>
```

2. Login user
```
login <username> <password>
```

3. Create group
```
create_group <group_id>
```

4. Join group
```
join_group <group_id>
```

5. Leave group
```
leave_group <group_id>
```

6. List pending join
```
list_requests <group_id>
```

7. Accept group joining request
```
accept_request <group_id> <username>
```

8. List all groups
```
list_groups
```

9. List all files
```
list_files <group_id>
```

10. Upload file
```
upload_file <filepath> <group_id>
```

11. Download file
```
download_file <group_id> <filename> <destination_path>
```

12. Logout
```
logout
```

13. Show downloads
```
show_downloads
```

14. Stop sharing
```
stop_share <group_id> <filename>
```

# Working
1. User should create an account and register with tracker.
2. Login using the user credentials.
3. Tracker maintains information of clients with their files(shared by client) to assist the clients for the communication between peers.
4. User can create Group and hence will become admin of that group.
5. User can fetch list of all Groups in server.
6. User can join/leave group.
7. Group admin can accept group join requests.
8. Fetch list of all sharable files in a Group.
9. Download:
    - Retrieve peer information from tracker for the file.
    - Download file from multiple peers (different pieces of file from different peers - ​piece selection algorithm​) simultaneously and all the files which client downloads will be shareable to other users in the same group. File integrity is ensured using SHA1 comparison.
    - Piece selection algorithm used selects random piece and then downloads it from a random peer having that piece.
10. Show downloads.
11. Stop sharing file.
12. Logout from the terminal.