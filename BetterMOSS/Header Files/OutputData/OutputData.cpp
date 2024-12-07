#include "OutputDataH.h"

void _outPut2Zip(const std::wstring& newData, const char* zipFile, const char* fileName) {
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));

    std::vector<std::pair<std::string, std::vector<char>>> files;

    if (mz_zip_reader_init_file(&zip_archive, zipFile, 0)) {
        mz_uint numFiles = mz_zip_reader_get_num_files(&zip_archive);

        for (mz_uint i = 0; i < numFiles; ++i) {
            mz_zip_archive_file_stat file_stat;
            if (mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
                size_t fileSize;
                char* fileData = (char*)mz_zip_reader_extract_file_to_heap(&zip_archive, file_stat.m_filename, &fileSize, 0);

                if (fileData) {
                    if (strcmp(file_stat.m_filename, fileName) == 0) {
                        std::string currentData(fileData, fileData + fileSize);
                        currentData.append(newData.begin(), newData.end()); 
                        files.emplace_back(file_stat.m_filename, std::vector<char>(currentData.begin(), currentData.end()));
                    } else {
                        files.emplace_back(file_stat.m_filename, std::vector<char>(fileData, fileData + fileSize));
                    }

                    mz_free(fileData);
                }
            }
        }

        mz_zip_reader_end(&zip_archive);
    }

    if (files.empty() || none_of(files.begin(), files.end(), [fileName](const auto& pair) {
        return pair.first == fileName;
    })) {
        std::string newDataStr(newData.begin(), newData.end());
        files.emplace_back(fileName, std::vector<char>(newDataStr.begin(), newDataStr.end()));
    }

    if (!mz_zip_writer_init_file(&zip_archive, zipFile, 0)) {
        std::cerr << "Failed to initialize ZIP file for writing. Error: "
                  << mz_zip_get_last_error(&zip_archive) << std::endl;
        return;
    }

    for (const auto& file : files) {
        if (!mz_zip_writer_add_mem(&zip_archive, file.first.c_str(), file.second.data(), file.second.size(), MZ_BEST_SPEED)) {
            std::cerr << "Failed to add file '" << file.first << "' to ZIP. Error: "
                      << mz_zip_get_last_error(&zip_archive) << std::endl;
        }
    }

    if (!mz_zip_writer_finalize_archive(&zip_archive)) {
        std::cerr << "Failed to finalize ZIP archive. Error: "
                  << mz_zip_get_last_error(&zip_archive) << std::endl;
    }

    if (!mz_zip_writer_end(&zip_archive)) {
        std::cerr << "Failed to close ZIP writer. Error: "
                  << mz_zip_get_last_error(&zip_archive) << std::endl;
    }
}
