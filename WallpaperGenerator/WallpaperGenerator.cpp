#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <exception>

#include "ConfigParser.hpp"

extern "C" {
#include <log.h>
}

using YMC::WallpaperGenerator::ConfigParser::generateFromTOML;

int MainProcess(const string & toml_config_path, const string & output_path, const string & path_additional_script);

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        if (argc < 4)
            log_fatal("Missing command line parameter(s)!\n");
        else
            log_fatal("Too many command line parameters!\n");
        printf("Usage: %s <path_to_config.toml> <path_to_output> <path_to_additional_lua_script>\n", PROGRAM_NAME);
        return EXIT_FAILURE;
    }

    try
    {
        MainProcess(argv[1], argv[2], argv[3]);
    }
    catch (const std::exception & e)
    {
        fprintf(stderr, "\033[1;31;40mAN ERROR OCCURRED!\033[0m\n");
        fprintf(stderr, "%s\n", e.what());
        return EXIT_FAILURE;
    }
    return 0;
}

int MainProcess(const string & toml_config_path, const string & output_path, const string & path_additional_script)
{
    printf("%s\n", toml_config_path.c_str());
    printf("%s\n\n", output_path.c_str());

    cv::Mat output_img = generateFromTOML(toml_config_path, path_additional_script);
    cv::imwrite(output_path, output_img);
    return 0;
}

