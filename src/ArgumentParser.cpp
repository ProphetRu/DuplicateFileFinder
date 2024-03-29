#include "ArgumentParser.h"
#include <iostream>
#include <stdexcept>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

bool ArgumentParser::Parse(int argc, char* argv[])
{
    try
    {
        po::options_description desc("Arguments");
        desc.add_options()
            ("help",                                                         "produce help message")
            ("directories,D",   po::value<StringList>()->multitoken(),       "directories to scan")
            ("exclude,E",       po::value<StringList>()->multitoken(),       "directories to exclude")
            ("deep-scan",       po::value<size_t>()->default_value(1),       "1 - include subdirectories, 0 - only directory")
            ("min-file-size,F", po::value<size_t>()->default_value(1),       "minimum file size in bytes")
            ("file-mask,M",     po::value<std::string>()->default_value(""), "mask names of files allowed for comparison")
            ("block-size,S",    po::value<size_t>()->default_value(4096),    "block size for reading files")
            ("hashing-type,H",  po::value<size_t>()->default_value(1),       "hashing type 1 - md5, 0 - crc32");

        po::variables_map vm{};
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            desc.print(std::cout);
            return false;
        }

        if (vm.count("directories") == 0)
        {
            throw std::invalid_argument("No directories specified");
        }
        m_DirectoriesScan = vm["directories"].as<StringList>();

        if (vm.count("exclude"))
        {
            m_DirectoriesExclude = vm["exclude"].as<StringList>();
        }

        if (const auto deep_scan{ vm["deep-scan"].as<size_t>() }; deep_scan == 1)
        {
            m_ScanType = ScanType::Deep;
        }
        else if (deep_scan == 0)
        {
            m_ScanType = ScanType::Regular;
        }
        else
        {
            throw std::invalid_argument("Invalid deep scan");
        }

        m_MinimumFileSizeBt = vm["min-file-size"].as<size_t>();
        m_FileMask          = vm["file-mask"].as<std::string>();
        m_BlockSize         = vm["block-size"].as<size_t>();

        if (const auto hashing_type{ vm["hashing-type"].as<size_t>() }; hashing_type == 1)
        {
            m_HashType = HashType::MD5;
        }
        else if (hashing_type == 0)
        {
            m_HashType = HashType::CRC32;
        }
        else
        {
            throw std::invalid_argument("Invalid hashing type");
        }

        return true;
    }
    catch (const po::unknown_option& ex)
    {
        // rethrow
        throw std::invalid_argument(ex.what());
    }
    catch (const po::invalid_command_line_syntax& ex)
    {
        // rethrow
        throw std::invalid_argument(ex.what());
    }
}

StringList ArgumentParser::GetDirectoriesScan() const noexcept
{
    return m_DirectoriesScan;
}

StringList ArgumentParser::GetDirectoriesExclude() const noexcept
{
    return m_DirectoriesExclude;
}

ScanType ArgumentParser::GetScanType() const noexcept
{
    return m_ScanType;
}

size_t ArgumentParser::GetMinimumFileSizeBt() const noexcept
{
    return m_MinimumFileSizeBt;
}

String ArgumentParser::GetFileMask() const noexcept
{
    return m_FileMask;
}

size_t ArgumentParser::GetBlockSize() const noexcept
{
    return m_BlockSize;
}

HashType ArgumentParser::GetHashType() const noexcept
{
    return m_HashType;
}
