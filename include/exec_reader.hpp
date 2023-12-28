#pragma once

#include "common.hpp"

#ifndef EXEC_READER_HPP
#define EXEC_READER_HPP

class ExecFileReader {
  private:
    std::vector<unsigned char> raw_bytes;
    std::vector<unsigned char> mem;

    static constexpr unsigned int BYTE = 8;
    static constexpr size_t FIRST_SEG_IDX = 5;
    static constexpr size_t SEG_HEADER_SIZE = 17;

  public:
    ExecFileReader() = default;

    [[nodiscard]] size_t size() const { return mem.size(); }
    [[nodiscard]] unsigned char *data() { return mem.data(); }

    int readBytes(const std::string &fname);
    int readSegHeader(size_t &pc_idx);
    int readFile(const std::string &fname);
};

#endif
