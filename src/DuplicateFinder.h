#pragma once

#include "ArgumentParser.h"
#include <unordered_map>

using Files = std::unordered_map<String, StringList>;

/**
 * @brief Concept for a directory iterator.
 */
template <typename Iterator>
concept DirectoryIterator = requires(Iterator it)
{
    //{ *it }              -> std::convertible_to<fs::directory_entry>;
    { ++it }             -> std::same_as<Iterator&>;
    { it != Iterator{} } -> std::convertible_to<bool>;
};

/**
 * @brief Class for finding duplicate files.
 */
class DuplicateFinder final
{
public:
    /**
     * @brief Find duplicate files based on the provided arguments.
     * @param args The ArgumentParser object containing scan parameters.
     * @return A map of duplicate files.
     */
    [[nodiscard]] Files Find(const ArgumentParser& args) noexcept;

private:
    /**
     * @brief Scan directories for duplicate files.
     * @tparam Iterator The type of directory iterator.
     * @param args The ArgumentParser object containing scan parameters.
     * @return A map of duplicate files.
     */
    template <DirectoryIterator Iterator>
    [[nodiscard]] Files Scan(const ArgumentParser& args) noexcept;

private:
    /**
     * @brief Calculate MD5 hash for data.
     * @param data The data to calculate the hash for.
     * @return The MD5 hash.
     */
    [[nodiscard]] String CalcMD5(std::string_view data) noexcept;

    /**
     * @brief Calculate CRC32 hash for data.
     * @param data The data to calculate the hash for.
     * @return The CRC32 hash.
     */
    [[nodiscard]] String CalcCRC32(std::string_view data) noexcept;
};
