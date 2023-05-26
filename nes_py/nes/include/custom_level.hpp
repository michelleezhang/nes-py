//  Program:      nes-py
//  File:         cpu.hpp
//  Description:  This class parses custom levels, feeds them to EMU
//
//  Copyright (c) 2023 Cameron Churchwell. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>

#include "common.hpp"
#include "main_bus.hpp"

#ifndef CUSTOM_HPP
#define CUSTOM_HPP

#define LEVEL_WIDTH 48
#define LEVEL_HEIGHT 13
#define LEVEL_SIZE LEVEL_WIDTH*LEVEL_HEIGHT

namespace NES {

class CustomLevel {
  private:
    std::array<char, LEVEL_SIZE> tiles;
    int internal_column_position;
  public:
    const static NES_Address inject_instruction = 0x94DC;
    // const static NES_Address reset_instruction = 0x0000;
    const static NES_Address metatile_buffer_addr = 0x06a1;

    CustomLevel(const std::string& level_file_path) {
        std::cout << level_file_path << std::endl;
        internal_column_position = 0;
        std::ifstream level_file(level_file_path, std::ios::binary);

        if(!level_file) {
            throw std::runtime_error("Failed to get level file!");
        }

        level_file.read(tiles.data(), tiles.size());

        level_file.close();
    }

    CustomLevel(const std::array<char, LEVEL_SIZE> &level_data) {
        for (int i=0; i<LEVEL_SIZE; i++) {
            tiles[i] = level_data[i];
        }
    }

    void inject_column(MainBus &bus) {
        // std::cout << "internal column position: " << internal_column_position << std::endl << std::flush;
        for (int i=0; i<LEVEL_HEIGHT; i++) {
            bus.write(metatile_buffer_addr+i, tiles[internal_column_position*LEVEL_HEIGHT + i]);
            // bus.write(metatile_buffer_addr+i, tiles[0]);
            // bus.write(metatile_buffer_addr+i, 0x54);
        }
        internal_column_position++;
        if (internal_column_position >= LEVEL_WIDTH) {
            internal_column_position = 0;
        }
    }
};


}  // namespace NES

#endif  // CUSTOM_HPP
