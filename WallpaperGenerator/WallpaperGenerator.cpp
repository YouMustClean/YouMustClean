#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "ConfigParser.hpp"

extern "C" {
#include <log.h>
}


int MainProcess(string toml_config_path, string output_path);

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        log_fatal("Missing command line parameter(s)!\n");
        printf("Usage: %s <path_to_config.toml> <path_to_output>\n", PROGRAM_NAME);
        return EXIT_FAILURE;
    }

    MainProcess(argv[1], argv[2]);
    return 0;
}

int MainProcess(string toml_config_path, string output_path)
{
    printf("%s\n", toml_config_path.c_str());
    printf("%s\n\n", output_path.c_str());

    cv::Mat output_img = YMC::WallpaperGenerator::ConfigParser::generateFromTOML(toml_config_path);
    cv::imwrite(output_path, output_img);
    return 0;
}

