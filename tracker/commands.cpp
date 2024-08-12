#include "headers.h"
#include "cmdcall.h"

// handle panic statements

int create_user(Command* cmd) {
    string username = cmd->argv[0], password = cmd->argv[1];
    if(user_list.find(username) != user_list.end()) {
        if(send(cmd->sock_out, "409 User already exists.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    user_list[username] = password;
    if(send(cmd->sock_out, "200 User created successfully.\n", SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.\n");
        return -1;
    }
    
    return 0;
}

int login(Command* cmd) {
    string user = get_user_from_sock(cmd->sock_out);
    if(user != NO_USER) {
        if(send(cmd->sock_out, ("400 You are already logged in as " + user + ". Please logout first before logging back in.\n").c_str(), SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    string username = cmd->argv[0], password = cmd->argv[1];
    if(user_list.find(username) == user_list.end()) {
        if(send(cmd->sock_out, "404 No user exists.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    if(password != user_list[username]) {
        if(send(cmd->sock_out, "401 Invalid credentials.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }
        
        return 0;
    }

    if(send(cmd->sock_out, ("200 "+username).c_str(), SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.\n");
        return -1;
    }
    
    bind_user_to_sock(cmd->sock_out, username);

    return 0;
}

// get_current_user

int create_group(Command* cmd) {
    string user = get_user_from_sock(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    string group_name = cmd->argv[0];
    if(group_list.find(group_name) != group_list.end()) {
        if(send(cmd->sock_out, "409 Group with same name already exists.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    Group* g = new Group();
    g->group_name = group_name;
    g->group_owner = user;
    g->group_members.insert(g->group_owner);
    group_list[g->group_name] = g;

    if(send(cmd->sock_out, "200 Group created.\n", SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.\n");
        return -1;
    }

    return 0;
}

int join_group(Command* cmd) {
    string user = get_user_from_sock(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    string group_name = cmd->argv[0];
    if(group_list.find(group_name) == group_list.end()) {
        if(send(cmd->sock_out, "404 No group found.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    Group* g = group_list[group_name];
    if(g->group_members.find(user) != g->group_members.end()) {
        if(send(cmd->sock_out, "400 You are already a member of this group.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    if(g->request_list.find(user) != g->request_list.end()) {
        if(send(cmd->sock_out, "409 You have already requested to join this group.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    g->request_list.insert(user);
    if(send(cmd->sock_out, "200 Request to join group sent.\n", SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.\n");
        return -1;
    }

    return 0;
}

int leave_group(Command* cmd) {
    string user = get_user_from_sock(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    string group_name = cmd->argv[0];
    if(group_list.find(group_name) == group_list.end()) {
        if(send(cmd->sock_out, "404 No group found.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    Group* g = group_list[group_name];
    if(g->group_members.find(user) == g->group_members.end()) {
        if(send(cmd->sock_out, "400 You are not a member of this group.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    g->group_members.erase(user);
    if(g->group_members.size() == 0)
        group_list.erase(group_name);

    else if(g->group_owner == user)
        g->group_owner = *g->group_members.begin();

    if(send(cmd->sock_out, "200 Group left successfully.\n", SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.\n");
        return -1;
    }

    return 0;
}

int list_requests(Command* cmd) {
    string user = get_user_from_sock(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    string group_name = cmd->argv[0];
    if(group_list.find(group_name) == group_list.end()) {
        if(send(cmd->sock_out, "404 No group found.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    Group* g = group_list[group_name];
    if(g->group_owner != user) {
        if(send(cmd->sock_out, "403 You don't have permission to execute this command.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    if(g->request_list.size() == 0) {
        if(send(cmd->sock_out, "200 No pending requests.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    string resp = "";
    for(string req: g->request_list)
        resp += req + '\n';

    if(send(cmd->sock_out, ("200 "+resp).c_str(), SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.\n");
        return -1;
    }

    return 0;
}

int accept_request(Command* cmd) {
    string user = get_user_from_sock(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    string group_name = cmd->argv[0], request_user = cmd->argv[1];
    if(group_list.find(group_name) == group_list.end()) {
        if(send(cmd->sock_out, "404 No group found.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    Group* g = group_list[group_name];
    if(g->group_owner != user) {
        if(send(cmd->sock_out, "403 You don't have permission to execute this command.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    if(g->request_list.find(request_user) == g->request_list.end()) {
        if(send(cmd->sock_out, "404 No request from user found.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    g->group_members.insert(request_user);
    g->request_list.erase(request_user);
    if(send(cmd->sock_out, "200 User added to group successfully.\n", SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.\n");
        return -1;
    }

    return 0;
}

int reject_request(Command* cmd) {
    string user = get_user_from_sock(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    string group_name = cmd->argv[0], request_user = cmd->argv[1];
    if(group_list.find(group_name) == group_list.end()) {
        if(send(cmd->sock_out, "404 No group found.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    Group* g = group_list[group_name];
    if(g->group_owner != user) {
        if(send(cmd->sock_out, "403 You don't have permission to execute this command.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    if(g->request_list.find(request_user) == g->request_list.end()) {
        if(send(cmd->sock_out, "404 No request from user found.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    g->request_list.erase(request_user);
    if(send(cmd->sock_out, "200 User request rejected.\n", SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.\n");
        return -1;
    }

    return 0;
}

int list_groups(Command* cmd) {
    string user = get_user_from_sock(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    if(group_list.size() == 0) {
        if(send(cmd->sock_out, "200 No groups present.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    string resp = "";
    for(auto& [group_name, group]: group_list)
        resp += group_name + '\n';

    if(send(cmd->sock_out, ("200 "+resp).c_str(), SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.\n");
        return -1;
    }

    return 0;
}

int list_files(Command* cmd) {
    string user = get_user_from_sock(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    string group_name = cmd->argv[0];
    if(group_list.find(group_name) == group_list.end()) {
        if(send(cmd->sock_out, "404 No group found.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    Group* g = group_list[group_name];
    if(g->file_list.size() == 0) {
        if(send(cmd->sock_out, "200 No files present in group.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    string resp = "";
    for(auto& [file_name, file]: g->file_list)
        resp += file_name + '\n';

    if(send(cmd->sock_out, ("200 "+resp).c_str(), SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.\n");
        return -1;
    }

    return 0;
}

int upload_file(Command* cmd) {
    string user = get_user_from_sock(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    // only get filename from peer, no need for path here
    string file_path = cmd->argv[0], group_name = cmd->argv[1];
    long long num_chunks = stoll(cmd->argv[2]);
    if(group_list.find(group_name) == group_list.end()) {
        if(send(cmd->sock_out, "404 No group found.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    Group* g = group_list[group_name];
    if(g->group_members.find(user) == g->group_members.end()) {
        if(send(cmd->sock_out, "403 You are not part of this group.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    string file_name = file_path.substr(file_path.find_last_of('/') + 1);
    string peerid = get_peerid_from_sock(cmd->sock_out);
    if(g->file_list.find(file_name) == g->file_list.end()) {
        File* f = new File();
        f->file_name = file_name;
        f->chunks.resize(num_chunks);
        g->file_list[file_name] = f;
    }

    File* f = g->file_list[file_name];
    if(f->share_list.find(peerid) != f->share_list.end()) {
        if(send(cmd->sock_out, "409 You have already uploaded a file with same name.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    f->share_list.insert(peerid);
    for(auto& chunk: f->chunks)
        chunk.insert(peerid);

    if(send(cmd->sock_out, "200 File uploaded successfully.\n", SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.\n");
        return -1;
    }

    return 0;
}

int download_file(Command* cmd) {
    string user = get_user_from_sock(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    // no need for destination path here
    string group_name = cmd->argv[0], file_name = cmd->argv[1], dest_path = cmd->argv[2];
    if(group_list.find(group_name) == group_list.end()) {
        if(send(cmd->sock_out, "404 No group found.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    Group* g = group_list[group_name];
    if(g->group_members.find(user) == g->group_members.end()) {
        if(send(cmd->sock_out, "403 You are not part of this group.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    if(g->file_list.find(file_name) == g->file_list.end()) {
        if(send(cmd->sock_out, "404 File not found.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    string peerid = get_peerid_from_sock(cmd->sock_out);
    File* f = g->file_list[file_name];
    f->share_list.insert(peerid);

    vector<pair<int, int>> chunk_order(f->chunks.size());
    for(int i=0; i<chunk_order.size(); i++)
        chunk_order[i] = make_pair(f->chunks[i].size(), i);
    
    sort(chunk_order.begin(), chunk_order.end());

    string resp = file_name + " ";
    for(auto& [size, idx]: chunk_order) {
        // getting random peer from the list. (may implement ordered_set : O(n) -> O(logn))
        auto it = f->chunks[idx].begin();
        advance(it, rand() % size);
        resp += *it + ' ';
    }

    if(send(cmd->sock_out, ("200 " + resp).c_str(), SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.\n");
        return -1;
    }

    return 0;
}

int got_chunk(Command* cmd) {
    string user = get_user_from_sock(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    string group_name = cmd->argv[0], file_name = cmd->argv[1];
    long long chunk_index = stoll(cmd->argv[2]);
    if(group_list.find(group_name) == group_list.end()) {
        if(send(cmd->sock_out, "404 No group found.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    Group* g = group_list[group_name];
    // TODO - need to check how to handle this
    if(g->file_list.find(file_name) == g->file_list.end()) {
        if(send(cmd->sock_out, "404 File not found.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    File* f = g->file_list[file_name];
    if(f->chunks.size() <= chunk_index) {
        if(send(cmd->sock_out, "400 Invalid index for chunk.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    // store that user also has this chunk
    string peerid = get_peerid_from_sock(cmd->sock_out);
    f->share_list.insert(user);
    f->chunks[chunk_index].insert(peerid);
    if(send(cmd->sock_out, "200 Acknowledged.\n", SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.\n");
        return -1;
    }

    return 0;
}

int logout(Command* cmd) {
    string user = get_user_from_sock(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "400 You are already logged out.\n", SIZE_1024, 0) < 0) {
            console_write("Error sending response to peer.\n");
            return -1;
        }

        return 0;
    }

    if(send(cmd->sock_out, "200 User logged out successfully.\n", SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.\n");
        return -1;
    }

    bind_user_to_sock(cmd->sock_out, NO_USER);

    return 0;
}

int show_downloads(Command* cmd) {

}

int stop_share(Command* cmd) {

}

int invalid(Command* cmd) {
    if(send(cmd->sock_out, ("422 " + cmd->msg).c_str(), SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.");
        return -1;
    }

    return 0;
}

int quit(Command* cmd) {
    if(send(cmd->sock_out, "200", SIZE_1024, 0) < 0) {
        console_write("Error sending response to peer.");
        return -1;
    }

    return 1;
}