#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../common/Protocol.h"

class FileParser {
private:
    std::ifstream file;
    std::string currentLine;

public:
    FileParser(const std::string& filename) {
        file.open(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            exit(1);
        }
    }

    bool parseNext(MarketUpdate& update) {
        if (!std::getline(file, currentLine)) {
            return false;
        }

        if (currentLine.empty()) return parseNext(update);

        std::stringstream ss(currentLine);
        std::string symbolStr, sideStr, typeStr;

        ss >> update.epoch >> update.orderId >> symbolStr >> sideStr >> typeStr >> update.price >> update.quantity;

        strncpy(update.symbol, symbolStr.c_str(), 7);

        update.side = (sideStr == "BUY") ? 'B' : 'S';

        if (typeStr == "NEW") update.type = 'N';
        else if (typeStr == "TRADE") update.type = 'T';
        else if (typeStr == "CANCEL") update.type = 'C';
        else update.type = '?';

        return true;
    }
};

#endif
