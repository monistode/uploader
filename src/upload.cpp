#include "common.hpp"
#include "exec_reader.hpp"

class ObjectParser {
    std::vector<unsigned char> mem;

  public:
    ObjectParser() = default;

    [[nodiscard]] size_t size() const { return mem.size(); }
    [[nodiscard]] unsigned char *data() { return mem.data(); }

    int readFile(const std::string &fname) {
        std::ifstream programStream(fname, std::ios::binary);

        if (!programStream) {
            std::cerr << "Failed to open file \"" << fname << "\"" << std::endl;
            return FILE_ERR;
        }

        mem = std::vector<unsigned char>(
            (std::istreambuf_iterator<char>(programStream)),
            (std::istreambuf_iterator<char>()));

        programStream.close();

        return SUCCESS;
    }
};

template <class FileReader> class ProgramMemory {
    bool loaded = false;
    std::string fname;
    FileReader programMemory;

    int readFile() {
        int result = this->programMemory.readFile(this->fname);
        if (result != SUCCESS) {
            return result;
        }

        loaded = true;

        return SUCCESS;
    }

    int mapMemory() {
        int memFile = open("/dev/mem", O_RDWR);

        if (!static_cast<bool>(memFile)) {
            std::cerr << "Could not open file \"/dev/mem\"" << std::endl;
            return DEV_MEM_ERR;
        }

        auto *deviceMemory = static_cast<uint8_t *>(
            mmap(nullptr, PRG_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
                 memFile, MEM_START));

        if (deviceMemory == MAP_FAILED) {
            std::cerr << "Could not mmap memory at " << std::hex << MEM_START
                      << std::dec << std::endl;
            return MMAP_ERR;
        }

        std::memcpy(deviceMemory, this->programMemory.data(),
                    std::min(PRG_MEM_SIZE, this->programMemory.size()));

        int result = munmap(deviceMemory, PRG_MEM_SIZE);

        if (result < 0) {
            std::cerr << "Could not unmap memory at " << std::hex << MEM_START
                      << std::dec << std::endl;
            return MUNMAP_ERR;
        }

        close(memFile);

        return SUCCESS;
    }

  public:
    ProgramMemory(std::string filename) : fname{std::move(filename)} {}
    ProgramMemory(const char *filename) : fname{filename} {}

    int writeProgram() {
        int result = 0;

        if (!loaded) {
            result = this->readFile();
            if (result != SUCCESS) {
                return result;
            }
        }

        result = this->mapMemory();
        return result;
    }
};

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: uload <filename>" << std::endl;
    }

    ProgramMemory mapper = ProgramMemory<ExecReader>(argv[1]);
    int result = mapper.writeProgram();

    return result;
}
