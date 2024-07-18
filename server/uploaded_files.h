struct UploadedFiles
{
    std::string name;
    std::unordered_map<std::string, std::string> owners;    // username -> path
    unsigned long size;
    unsigned long chunks;
};