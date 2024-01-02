#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

const char LINE_DELIMITER = '\n';

void cut(std::string filename, std::vector<int> columns, char delimiter) {
    std::ifstream file{filename};
    if (!file.good()) {
        std::cout << "\nfile not found\n";
        return;
    }

    if (columns.size() == 0) {
        std::string line2;
        std::getline(file, line2);
        for (auto ch : line2) {
            //std::cout << ch;
        }
    }
    std::sort(columns.begin(), columns.end());

    std::vector<int> skips{};
    for (int i = columns.size() - 1; i >= 0; --i) {
        int distance = 0;
        if (i == 0) {
            distance = columns.at(0) - 1;
        } else {
            distance = columns.at(i) - columns.at(i-1) - 1;
        }

        skips.push_back(distance);
    }

    std::reverse(skips.begin(), skips.end());

    while (!file.eof()) {
        std::vector<std::vector<char>> output{};
        for (int i = 0; i < skips.size(); ++i) {

            int toSkip = skips.at(i);
            for (int j = 0; j < toSkip; ++j) {
                file.ignore(256, delimiter);
            }

            // take chars from the stream until delimiter
            char ch = file.get();
            std::vector<char> word = {};
            while( ch != delimiter ) {
                word.push_back(ch);
                ch = file.get();
            }

            output.push_back(word);
        }
        for (int i = 0; i < output.size(); ++i) {
            for (auto ch : output.at(i)) {
                std::cout << ch;
            }
            if (i != output.size() - 1) {
                std::cout << delimiter;
            }
        }
        std::cout << std::endl;
        file.ignore(1024, LINE_DELIMITER);
    }
};

int main (int argc, char *argv[]) {
    using namespace std::literals;
    std::string command = argv[1];
    std::vector<std::string> args(argv + 1, argv + argc);
    std::vector<int> columns{};

    char delimiter = ',';

    for (auto arg : args) {
        int index = arg.find('-');
        if (index != -1) {
            char command_type = arg.at(index + 1);
            std::string command_value = arg.substr(index + 2);

            if (command_type == 'f') {
                size_t index = 0;
                std::string command;
                while ((index = command_value.find(',')) != std::string::npos) {
                    command = command_value.substr(0, index);
                    columns.push_back(std::stoi(command));
                    command_value.erase(0, index + 1);
                }

                if (index == std::string::npos) {
                    columns.push_back(std::stoi(command_value));
                }
            }
            if (command_type == 'd' && command_value.length()) {

                delimiter = command_value.at(0);
                if (delimiter == 't') {
                    delimiter = '\t';
                }
            }
        }
    }

    std::string filename = args.back();
    if (command == "-h"sv || command == "--help"sv) {
        std::cout << "help text here" << std::endl;
        return 0;
    }
    cut(filename, columns, delimiter);

    return 0;
}
