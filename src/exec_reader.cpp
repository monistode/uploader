#include "exec_reader.hpp"

int ExecFileReader::readBytes(const std::string &fname) {
    std::ifstream programStream(fname, std::ios::binary);

    if (!programStream) {
        std::cerr << "Failed to open file \"" << fname << "\"" << std::endl;
        return FILE_ERR;
    }

    raw_bytes = std::vector<unsigned char>(
        (std::istreambuf_iterator<char>(programStream)),
        (std::istreambuf_iterator<char>()));

    programStream.close();

    return SUCCESS;
}

int ExecFileReader::readSegHeader(size_t &pc_idx) {
    size_t seg_start = raw_bytes[pc_idx] + (raw_bytes[pc_idx + 1] << BYTE);
    size_t seg_size = raw_bytes[pc_idx + 8] + (raw_bytes[pc_idx + 9] << BYTE);

    std::memcpy(mem.data() + seg_start,
                raw_bytes.data() + pc_idx + SEG_HEADER_SIZE, seg_size);

    pc_idx += SEG_HEADER_SIZE + seg_size;

    return SUCCESS;
}

int ExecFileReader::readFile(const std::string &fname) {
    int result = readBytes(fname);

    if (result != SUCCESS) {
        return result;
    }

    mem = std::vector<unsigned char>(PRG_MEM_SIZE, 0);
    size_t pc_idx = FIRST_SEG_IDX;

    while (pc_idx < raw_bytes.size()) {
        result = readSegHeader(pc_idx);

        if (result != SUCCESS) {
            return result;
        }
    }

    return SUCCESS;
}
