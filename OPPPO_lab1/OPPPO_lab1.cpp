#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "FilmContainer.h"
#include "GameFilm.h"
#include "CartoonFilm.h"
#include "SeriesFilm.h"

std::unique_ptr<Film> createFilm(const std::string& type, const std::string& title, const std::string& additionalData)
{
    if (type.empty() || title.empty()) {
        throw std::invalid_argument("Film type and title cannot be empty");
    }

    std::istringstream iss(additionalData);

    if (type == "game") {
        std::string director;
        std::getline(iss, director);
        if (director.empty()) {
            throw std::invalid_argument("Director cannot be empty for game film");
        }
        return std::make_unique<GameFilm>(title, director);
    }
    else if (type == "cartoon") {
        std::string animType;
        iss >> animType;
        if (animType.empty()) {
            throw std::invalid_argument("Animation type cannot be empty for cartoon film");
        }
        TypeCreation typeEnum;
        if (animType == "drawn") typeEnum = TypeCreation::Drawn;
        else if (animType == "puppet") typeEnum = TypeCreation::Doll;
        else if (animType == "plasticine") typeEnum = TypeCreation::Plasticine;
        else {
            throw std::invalid_argument("Invalid animation type: '" + animType + "'. Use: drawn, puppet, plasticine");
        }
        return std::make_unique<CartoonFilm>(title, typeEnum);
    }
    else if (type == "series") {
        std::string director;
        int episodes = 0;
        if (!std::getline(iss, director, '|')) {
            throw std::invalid_argument("Missing director for series film");
        }
        director.erase(0, director.find_first_not_of(" \t"));
        director.erase(director.find_last_not_of(" \t") + 1);
        if (director.empty()) {
            throw std::invalid_argument("Director cannot be empty for series film");
        }
        if (!(iss >> episodes)) {
            throw std::invalid_argument("Missing or invalid episode count for series film");
        }
        if (episodes <= 0) {
            throw std::invalid_argument("Episode count must be positive for series film");
        }
        return std::make_unique<SeriesFilm>(title, director, episodes);
    }
    else {
        throw std::invalid_argument("Unknown film type: '" + type + "'. Use: game, cartoon, or series");
    }
}

void processAddCommand(std::istringstream& iss, FilmContainer& container) {
    std::string type, title, additionalData;
    if (!(iss >> type)) {
        std::cout << "Error: Missing film type after ADD command" << std::endl;
        return;
    }
    iss >> std::ws;
    if (!std::getline(iss, title, '|')) {
        std::cout << "Error: Missing title for ADD command. Format: ADD type title|additional_data" << std::endl;
        return;
    }
    if (!title.empty() && title.front() == '"' && title.back() == '"') {
        title = title.substr(1, title.length() - 2);
    }
    std::getline(iss, additionalData);
    additionalData.erase(0, additionalData.find_first_not_of(" \t"));

    try {
        std::vector<std::string> params;
        params.push_back(title);
        if (type == "cartoon") {
            std::istringstream dataStream(additionalData);
            std::string animType;
            dataStream >> animType;
            params.push_back(animType);
        }
        else if (type == "game") {
            params.push_back(additionalData);
        }
        else if (type == "series") {
            std::istringstream dataStream(additionalData);
            std::string director;
            std::string episodesStr;
            std::getline(dataStream, director, '|');
            dataStream >> episodesStr;
            params.push_back(director);
            params.push_back(episodesStr);
        }
        if (!FilmContainer::validateAddCommand(type, params)) {
            return;
        }
        auto film = createFilm(type, title, additionalData);
        container.addFilm(std::move(film));
    }
    catch (const std::exception& e) {
        std::cout << "Error creating film: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Unknown error occurred while creating film" << std::endl;
    }
}

void processRemoveCommand(std::istringstream& iss, FilmContainer& container) {
    std::string condition;
    std::getline(iss >> std::ws, condition);
    if (condition.empty()) {
        std::cout << "Error: Missing condition for REM command" << std::endl;
        return;
    }
    container.removeFilms(condition);
}

void commandFromFile(const std::string& filename, FilmContainer& container) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Cannot open file '" << filename << "'" << std::endl;
        return;
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty() || line[0] == '#') {
            continue;
        }
        std::istringstream iss(line);
        std::string command;
        if (!(iss >> command)) {
            continue;
        }

        try {
            if (command == "ADD") {
                processAddCommand(iss, container);
            }
            else if (command == "REM") {
                processRemoveCommand(iss, container);
            }
            else if (command == "PRINT") {
                container.printAll();
            }
            else {
                std::cout << "Error: Unknown command '" << command << "' at line " << lineNumber << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error processing command at line " << lineNumber << ": " << e.what() << std::endl;
        }
        catch (...) {
            std::cout << "Unknown error processing command at line " << lineNumber << std::endl;
        }
    }
    std::cout << "Finished processing file. Total films in container: " << container.size() << std::endl;
}

int main()
{
    //C:\Ueba\7semak\opppo\lab1\test.txt
    std::string filename;
    std::cout << "Enter filename: ";
    std::getline(std::cin, filename);

    if (filename.empty()) {
        std::cout << "Error: Filename cannot be empty" << std::endl;
        return 1;
    }

    try {
        FilmContainer container;
        commandFromFile(filename, container);
    }
    catch (const std::exception& e) {
        std::cout << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cout << "Unknown fatal error occurred" << std::endl;
        return 1;
    }

    return 0;
}