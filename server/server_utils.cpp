extern std::unordered_map<std::string, User> user_list;
extern std::unordered_map<std::string, Group> group_list;

std::vector<std::string> tokenize_cmd(char* in)
{
    std::string cmd = in;
    std::vector<std::string> tokens;
    std::string token="";

    for(auto ch: cmd)
    {
        if(ch == ' ')
        {
            tokens.push_back(token);
            token = "";
        }
        else
        {
            token += ch;
        }
    }

    tokens.push_back(token);

    return tokens;
}

void get_tracker_ip_port(const char* trackerpath, std::string& tracker_ip, int& tracker_port)
{
    int fd = open(trackerpath, O_RDONLY);
    char buffer[1024];

    if(read(fd, buffer, 1024) == -1)
    {
        perror("Error reading tracker info");
        exit(1);
    }

    std:: string address = buffer;

    int pos = address.find(":");

    tracker_ip = address.substr(0, pos);
    tracker_port = atoi(address.substr(pos+1, address.size()).c_str());

    close(fd);
}

File get_file(std::string filepath)
{
    int fd = open(filepath.c_str(), O_RDONLY);
    if(fd == -1)
    {
        perror("Error opening file to upload");
        exit(1);
    }

    struct stat status;
    fstat(fd, &status);

    File file;
    file.name = filepath.substr(filepath.find_last_of("/"), filepath.size());
    file.path = filepath;
    file.size = status.st_size;
    file.chunks = status.st_size/CHUNK_SIZE;

    close(fd);

    return file;
}

char* handle_create_group(std::string groupid, std::string owner)
{
    if(owner == "")
    {
        return "Please login to create group.\n";
    }

    if(group_list.find(groupid) != group_list.end())
    {
        return "Group already exits.\n";
    }

    Group new_group;
    new_group.id = groupid;
    new_group.group_owner = owner;
    new_group.group_members.insert(owner);

    group_list[groupid] = new_group;
    user_list[owner].groups_created.push_back(groupid);

    return "Group created successfully.\n";
}

char* handle_accept_request(std::string groupid, std::string username)
{
    if(username != group_list[groupid].group_owner)
    {
        return "Only group owner can accept requests for his group.\n";
    }

    if(group_list.find(groupid) == group_list.end())
    {
        return "Group provided does not exist.\n";
    }
    if(group_list[groupid].request_list.find(username) == group_list[groupid].request_list.end())
    {
        return "No request from user found.\n";
    }

    group_list[groupid].request_list.erase(username);
    group_list[groupid].group_members.insert(username);

    return "User added to group successfully.\n";
}

char* handle_create_user(std::string username, std::string password)
{
    if(user_list.find(username) != user_list.end())
    {
        return "User already exists.\n";
    }

    struct User new_user;
    new_user.username = username;
    new_user.password = password;
    new_user.login = false;

    user_list[username] = new_user;

    return "User created successfully.\n";
}

void handle_download_file()
{

}

const char* handle_invalid_request(std::string cmd)
{
    return ("Invalid request: No command " + cmd + " found.\n").c_str();
}

char* handle_join_group(std::string user, std::string groupid)
{
    if(user == "")
    {
        return "Please login to join a group.\n";
    }

    if(group_list[groupid].request_list.find(user) != group_list[groupid].request_list.end())
    {
        return "Already requested. Please wait for the owner to accept the request.\n";
    }

    group_list[groupid].request_list.insert(user);
    
    return "Requested successfully. Please wait for owner to accept the request.\n";
}

char* handle_leave_group(std::string user, std::string groupid)
{
    if(group_list[groupid].group_members.find(user) == group_list[groupid].group_members.end())
    {
        return "Cannot leave group. You are not part of the group.\n";
    }

    group_list[groupid].group_members.erase(user);

    if(group_list[groupid].group_owner == user)
    {
        group_list[groupid].group_owner = *group_list[groupid].group_members.end();
    }

    return "You left the group.\n";
}

const char* handle_list_files(std::string groupid)
{
    std::string list_files="";
    for(auto &it: group_list[groupid].file_list)
    {
        list_files += it.first + "\n";
    }

    return list_files.c_str();
}

const char* handle_list_groups()
{
    std::string groups="";
    for(auto &it: group_list)
    {
        groups += it.first + "\n";
    }

    return groups.c_str();
}

const char* handle_list_requests(std::string user, std::string groupid)
{
    if(group_list[groupid].group_owner != user)
    {
        return "Only group owner can see group join request list.\n";
    }

    std::string requests="";
    for(auto &it: group_list[groupid].request_list)
    {
        requests += it + "\n";
    }

    return requests.c_str();
}

char* handle_login(std::string username, std::string password, std::string& logged_in_user)
{
    if(user_list.find(username) == user_list.end())
    {
        return "User not found.\n";
    }

    if(password != user_list[username].password)
    {
        return "Incorrect password.\n";
    }

    logged_in_user = username;

    return "User logged in successfully.\n";
}

char* handle_logout(std::string& logged_in_user)
{
    if(logged_in_user == "")
    {
        return "No user logged in.\n";
    }

    logged_in_user = "";

    return "User logged out successfully.\n";
}

void handle_show_downloads()
{

}

void handle_stop_share()
{

}

char* handle_upload_file(std::string user, std::string filepath, std::string groupid)
{
    File file = get_file(filepath);

    if(user == "")
    {
        return "Please login to upload files.\n";
    }

    if(group_list.find(groupid) == group_list.end())
    {
        return "Group specified does not exist.\n";
    }

    if(group_list[groupid].file_list.find(file.name) != group_list[groupid].file_list.end())
    {
        UploadedFiles new_file;
        new_file.owners[user] = filepath;

        // todo
        new_file.name = file.name;
        new_file.chunks = file.size/CHUNK_SIZE;
        new_file.size = file.size;

        group_list[groupid].file_list[file.name] = new_file;
    }
    else
    {
        group_list[groupid].file_list[file.name].owners[user] = filepath;
    }

    return "File uploaded successfully.\n";
}

void handle_incoming_request(int client)
{
    std::string logged_in_user="";
    char request[1024];

    while(true)
    {
        memset(request, 0, 1024);

        read(client, request, 1024);

        std::vector<std::string> tokens = tokenize_cmd(request);

        std::string cmd_type = tokens[0];

        if(cmd_type == EXIT)
        {
            write(client, "", 1024);
        }
        else if(cmd_type == CREATE_USER)
        {
            write(client, handle_create_user(tokens[1], tokens[2]), 1024);
        }
        else if(cmd_type == LOGIN)
        {
            write(client, handle_login(tokens[1], tokens[2], logged_in_user), 1024);
        }
        else if(cmd_type == CREATE_GROUP)
        {
            write(client, handle_create_group(tokens[1], logged_in_user), 1024);
        }
        else if(cmd_type == JOIN_GROUP)
        {
            write(client, handle_join_group(logged_in_user, tokens[1]), 1024);
        }
        else if(cmd_type == LEAVE_GROUP)
        {
            write(client, handle_leave_group(logged_in_user, tokens[1]), 1024);
        }
        else if(cmd_type == LIST_REQUESTS)
        {
            write(client, handle_list_requests(logged_in_user, tokens[1]), 1024);
        }
        else if(cmd_type == ACCEPT_REQUEST)
        {
            write(client, handle_accept_request(tokens[1], tokens[2]), 1024);
        }
        else if(cmd_type == LIST_GROUPS)
        {
            write(client, handle_list_groups(), 1024);
        }
        else if(cmd_type == LIST_FILES)
        {
            // send(client, handle_list_files(), 1024, 0);
        }
        else if(cmd_type == UPLOAD_FILE)
        {
            write(client, handle_upload_file(logged_in_user, tokens[1], tokens[2]), 1024);
        }
        else if(cmd_type == DOWNLOAD_FILE)
        {
            // handle_download_file();
        }
        else if(cmd_type == LOGOUT)
        {
            write(client, handle_logout(logged_in_user), 1024);
        }
        else if(cmd_type == SHOW_DOWNLOADS)
        {
            // handle_show_downloads();
        }
        else if(cmd_type == STOP_SHARE)
        {
            // handle_stop_share();
        }
        else
        {
            write(client, ("Invalid request: No command " + cmd_type + " found.\n").c_str(), 1024);
        }
    }

    close(client);
}

Server server_constructor(int domain, int service, int protocol, unsigned long interface, int port, int backlog)
{
    Server server;
    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;

    server.port = port;
    server.backlog = backlog;

    server.address.sin_addr.s_addr = htonl(interface);
    server.address.sin_family = domain;
    server.address.sin_port = htons(port);

    if((server.socket = socket(domain, service, protocol)) == -1)
    {
        perror("Error making server socket");
        exit(1);
    }

    if((bind(server.socket, (struct sockaddr*) &server.address, sizeof(server.address))) < 0)
    {
        perror("Error binding server socket");
        exit(1);
    }

    if((listen(server.socket, server.backlog)) < 0)
    {
        perror("Error listening from server socket");
        exit(1);
    }

    std::cout << "Listening at port " << port << std::endl;

    return server;
}

void start_server(int port, int backlog)
{
    Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, backlog);
    struct sockaddr *address = (struct sockaddr *)&server.address;
    socklen_t address_length = (socklen_t) sizeof(server.address);

    while(true)
    {
        int client = accept(server.socket, address, &address_length);
        std::cout << "connection accepted\n";

        std::thread t(handle_incoming_request, client);

        t.detach();
    }
}