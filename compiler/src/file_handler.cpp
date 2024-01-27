#include "../inc/file_handler.h"

auto valid_extension(const std::string &file_name) -> bool
{
    size_t dpos = file_name.find_last_of('.');
    if (dpos != file_name.npos)
    {
        std::string ext(file_name, dpos + 1);
        return ext == "ro";
    }

    return false;
}

auto read_file(const std::string &file_path) -> std::unique_ptr<std::string>
{
    report(file_path.empty(), "No input file specified");
    report(!valid_extension(file_path), "Invalid file extension type");

    std::ifstream ifs(file_path);

    report(!ifs.is_open(), "Failed to open file: " + file_path);

    return std::make_unique<std::string>(std::istreambuf_iterator<char>(ifs),
                                         std::istreambuf_iterator<char>());
}
