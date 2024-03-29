#pragma once

#include <string>
#include <vector>

using String     = std::string;
using StringList = std::vector<String>;

/**
 * @brief Enum defining the type of scan.
 */
enum class ScanType
{
    Deep = 0, ///< Deep scan type.
    Regular,  ///< Regular scan type.
};

/**
 * @brief Enum defining the type of hash.
 */
enum class HashType
{
    MD5 = 0,  ///< MD5 hash type.
    CRC32,    ///< CRC32 hash type.
};

/**
 * @brief Class for parsing command line arguments.
 */
class ArgumentParser final
{
public:
    /**
     * @brief Parse command line arguments.
     * @param argc Number of arguments.
     * @param argv Array of arguments.
     * @return True if parsing was successful, false otherwise.
     * @throw std::invalid_argument if parsing fails.
     */
    bool Parse(int argc, char* argv[]);

    /**
     * @brief Get directories to scan.
     * @return List of directories to scan.
     */
    [[nodiscard]] StringList GetDirectoriesScan() const noexcept;

    /**
     * @brief Get directories to exclude from scan.
     * @return List of directories to exclude.
     */
    [[nodiscard]] StringList GetDirectoriesExclude() const noexcept;

    /**
     * @brief Get the type of scan.
     * @return The scan type.
     */
    [[nodiscard]] ScanType GetScanType() const noexcept;

    /**
     * @brief Get the minimum file size in bytes.
     * @return The minimum file size.
     */
    [[nodiscard]] size_t GetMinimumFileSizeBt() const noexcept;

    /**
     * @brief Get the file mask to filter files.
     * @return The file mask.
     */
    [[nodiscard]] String GetFileMask() const noexcept;

    /**
     * @brief Get the block size for file reading.
     * @return The block size.
     */
    [[nodiscard]] size_t GetBlockSize() const noexcept;

    /**
     * @brief Get the type of hash to use.
     * @return The hash type.
     */
    [[nodiscard]] HashType GetHashType() const noexcept;

private:
    StringList m_DirectoriesScan; ///< Directories to scan.
    StringList m_DirectoriesExclude; ///< Directories to exclude from scan.
    ScanType m_ScanType{ ScanType::Deep }; ///< Type of scan.
    size_t m_MinimumFileSizeBt{}; ///< Minimum file size in bytes.
    String m_FileMask; ///< File mask to filter files.
    size_t m_BlockSize{}; ///< Block size for file reading.
    HashType m_HashType{ HashType::MD5 }; ///< Type of hash to use.
};
