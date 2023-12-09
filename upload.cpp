#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

// Errorrs
static constexpr int SUCCESS = 0;
static constexpr int FILE_ERR = -1;
static constexpr int DEV_MEM_ERR = -2;
static constexpr int MMAP_ERR = -3;
static constexpr int MUNMAP_ERR = -4;

// Constants
static constexpr size_t MEM_START = 0x20'000'000; // 512 Mb
static constexpr size_t PRG_MEM_SIZE = 0x10'000;  // 64 Kb

class ProgramMemory {
    bool loaded = false;
    std::string fname;
    std::vector<uint8_t> programMemory;

    int readFile() {
        std::ifstream programStream(this->fname, std::ios::binary);

        if (!programStream) {
            std::cerr << "Failed to open file \"" << this->fname << "\""
                      << std::endl;
            return FILE_ERR;
        }

        this->programMemory = std::vector<uint8_t>(
            (std::istreambuf_iterator<char>(programStream)),
            (std::istreambuf_iterator<char>()));

        programStream.close();

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

    ProgramMemory mapper = ProgramMemory(argv[1]);
    int result = mapper.writeProgram();

    return result;
}
