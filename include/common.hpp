#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#ifndef COMMON_READER_HPP
#define COMMON_READER_HPP

// Errorrs
static constexpr int SUCCESS = 0;
static constexpr int FILE_ERR = -1;
static constexpr int DEV_MEM_ERR = -2;
static constexpr int MMAP_ERR = -3;
static constexpr int MUNMAP_ERR = -4;

// Constants
static constexpr size_t MEM_START = 0x20'000'000; // 512 Mb
static constexpr size_t PRG_MEM_SIZE = 0x10'000;  // 64 Kb

#endif
