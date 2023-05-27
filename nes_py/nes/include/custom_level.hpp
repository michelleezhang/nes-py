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

#define LEVEL_HEIGHT 13

namespace NES {

class CustomLevel {
  private:
    std::vector<char> tiles;
  public:
    const static NES_Address inject_instruction = 0x94DC;
    const static NES_Address metatile_buffer_addr = 0x06A1;
    const static NES_Address page_address = 0x0725;
    const static NES_Address column_address = 0x0726;

    CustomLevel(const std::string& level_file_path) {
        std::cout << level_file_path << std::endl;
        std::ifstream level_file(level_file_path, std::ios::binary);

        if(!level_file) {
            throw std::runtime_error("Failed to get level file!");
        }

        level_file.read(tiles.data(), tiles.size());

        level_file.close();
    }

    CustomLevel(const std::vector<char> &level_data) {
        int size = level_data.size();
        if (size % 13 != 0) {
            throw std::runtime_error("level data must be divisible by 13!");
        }
        tiles = level_data;
    }

    void inject_column(MainBus &bus) {
        // std::cout << "internal column position: " << internal_column_position << std::endl << std::flush;
        int page = bus.read(page_address);
        int page_column = bus.read(column_address);
        int absolute_column = 16*page+page_column;
        for (int i=0; i<LEVEL_HEIGHT; i++) {
            char tile = tiles[absolute_column*LEVEL_HEIGHT + i];
            bus.write(metatile_buffer_addr+i, tile);
        }
    }
};


}  // namespace NES

#endif  // CUSTOM_HPP
