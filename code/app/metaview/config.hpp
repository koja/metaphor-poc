#pragma once

#include <string>

struct Config {
    Config(
        const std::string& _ast_list_filename,
        const std::string& _crmap_list_filename
    ) :
        ast_list_filename(_ast_list_filename),
        crmap_list_filename(_crmap_list_filename)
    { }

    const std::string ast_list_filename;
    const std::string crmap_list_filename;
};
