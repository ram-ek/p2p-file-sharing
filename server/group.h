struct Group
{
    std::string id;
    std::string group_owner;
    std::unordered_set<std::string> group_members;
    std::unordered_set<std::string> request_list;
    std::unordered_map<std::string, UploadedFiles> file_list;   // file hash -> file attributes
};