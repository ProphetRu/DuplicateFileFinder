#include <iostream>
#include <ranges>
#include <stdexcept>
#include "ArgumentParser.h"
#include "DuplicateFinder.h"

int main(int argc, char* argv[]) noexcept
{
    try
    {
	    if (ArgumentParser ap; ap.Parse(argc, argv))
        {
            //std::cout << "Duplicate File Finder\n";
            //print_string_list("Directories to scan", ap.GetDirectoriesScan());
            //print_string_list("Directories to exclude", ap.GetDirectoriesExclude());
            //std::cout << "Deep scan    : " << (ap.GetScanType() == ScanType::Deep ? "deep" : "regular") << std::endl;
            //std::cout << "Min file size: " << ap.GetMinimumFileSizeBt() << std::endl;
            //std::cout << "File mask    : " << ap.GetFileMask()  << std::endl;
            //std::cout << "Block size   : " << ap.GetBlockSize() << std::endl;
            //std::cout << "Hashing type : " << (ap.GetHashType() == HashType::MD5 ? "md5" : "crc32") << std::endl << std::endl;

            DuplicateFinder df;
            if (const auto finded_files{ df.Find(ap) }; !finded_files.empty())
            {
                for (const auto& paths : finded_files | std::views::values)
                {
                    if (paths.size() > 1)
                    {
                        for (const auto& file_path : paths)
                        {
                            std::cout << file_path << std::endl;
                        }

                        std::cout << std::endl;
                    }
                }
            }
        }
    }
    catch (const std::invalid_argument& ex)
    {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}
