#include "DuplicateFinder.h"
#include <boost/filesystem.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/crc.hpp>
#include <boost/regex.hpp>
#include <fstream>

namespace fs = boost::filesystem;
namespace ud = boost::uuids::detail;

Files DuplicateFinder::Find(const ArgumentParser& args) noexcept
{
    return (args.GetScanType() == ScanType::Deep ?
        Scan<fs::recursive_directory_iterator>(args) :
        Scan<fs::directory_iterator>(args));
}

template <DirectoryIterator Iterator>
Files DuplicateFinder::Scan(const ArgumentParser& args) noexcept
{
    auto is_exclude_dir = [](const fs::directory_entry& entry, const StringList& exclude_dirs) noexcept
	{
        return std::ranges::any_of(exclude_dirs, [&entry](const std::string& exclude_dir) noexcept
        {
            return entry.path().string().find(exclude_dir) != std::string::npos;
        });
    };

    const boost::regex file_mask(args.GetFileMask());
    auto is_file_match_by_mask = [&](const fs::directory_entry& entry) noexcept
    {
        // return true is mask is empty
        // using method size()
        // method empty() don't help here
        if (file_mask.size() == 0)
        {
            return true;
        }

        return boost::regex_match(entry.path().filename().string(), file_mask);
    };

    Files files;
    
    for (const auto& scan_dir : args.GetDirectoriesScan())
    {
        if (fs::path scan_path{ scan_dir }; fs::exists(scan_path) && fs::is_directory(scan_path))
        {
            // direct or recursive iterator
            for (const auto& entry : Iterator(scan_path))
            {
                if (is_exclude_dir(entry, args.GetDirectoriesExclude()))
                {
                    // skip whole excluded dir
	                continue;
                }

                if (!fs::is_regular_file(entry.path()))
                {
                    // skip not files
                    continue;
                }

                const auto file_size_bt{ fs::file_size(entry) };
                if (file_size_bt < args.GetMinimumFileSizeBt())
                {
                    // skip files which size is less then minimum size
                    continue;
                }

                if (!is_file_match_by_mask(entry))
                {
	                // skip files that do not match the mask
                    continue;
                }

                // read file block by block
                if (std::ifstream input_file{ entry.path().string(), std::ios::binary }; input_file.is_open())
                {
                    String hash;
                    String block(args.GetBlockSize(), 0);

                    while (input_file)
                    {
                        input_file.read(block.data(), block.size());
                        if (input_file.gcount() == 0) 
                        {
                            break;
                        }

                        if (args.GetHashType() == HashType::MD5)
                        {
                            hash += CalcMD5(block);
                        }
                        else if (args.GetHashType() == HashType::CRC32)
                        {
                            hash += CalcCRC32(block);
                        }

                        // there is no another way to reset the string
                        memset(block.data(), 0, block.size());
                    }

                    files[hash].emplace_back(entry.path().string());
                    input_file.close();
                }
            }
        }
    }

    return files;
}

String DuplicateFinder::CalcMD5(std::string_view data) noexcept
{
    ud::md5 hash{};
    ud::md5::digest_type digest{};

    hash.process_bytes(data.data(), data.size());
    hash.get_digest(digest);

    const auto char_digest{ reinterpret_cast<const char*>(&digest) };
    return { char_digest, char_digest + sizeof(digest) };
}

String DuplicateFinder::CalcCRC32(std::string_view data) noexcept
{
    boost::crc_32_type result{};
    result.process_bytes(data.data(), data.size());
    return std::to_string(result.checksum());
}
