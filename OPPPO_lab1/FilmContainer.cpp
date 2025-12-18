#include "FilmContainer.h"
#include <algorithm>
#include <iostream>
#include <sstream>

void FilmContainer::addFilm(std::unique_ptr<Film> film) {
    if (!film) {
        std::cout << "Error: Cannot add null film" << std::endl;
        return;
    }
    films.push_back(std::move(film));
    std::cout << "Film added successfully" << std::endl;
}

void FilmContainer::removeFilms(const std::string& condition) {
    if (condition.empty()) {
        std::cout << "Error: Empty condition provided" << std::endl;
        return;
    }

    if (films.empty()) {
        std::cout << "No films to remove - container is empty" << std::endl;
        return;
    }

    try {
        auto newEnd = std::remove_if(films.begin(), films.end(),
            [&condition](const std::unique_ptr<Film>& film) {
                return film->matchesCondition(condition);
            });

        int removedCount = films.end() - newEnd;
        films.erase(newEnd, films.end());
        std::cout << "Successfully removed " << removedCount << " film(s)" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error removing films: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Unknown error occurred while removing films" << std::endl;
    }
}

void FilmContainer::printAll() const {
    if (films.empty()) {
        std::cout << "Container is empty" << std::endl;
        return;
    }

    std::cout << "Films in container (" << films.size() << " total):" << std::endl;
    for (size_t i = 0; i < films.size(); ++i) {
        std::cout << i + 1 << ". ";
        films[i]->print();
    }
}

size_t FilmContainer::size() const {
    return films.size();
}

bool FilmContainer::validateAddCommand(const std::string& type, const std::vector<std::string>& params) {
    if (type == "cartoon") {
        if (params.size() < 2) {
            std::cout << "Error: Cartoon film requires title and animation type (drawn/puppet/plasticine)" << std::endl;
            return false;
        }
        if (params[1] != "drawn" && params[1] != "puppet" && params[1] != "plasticine") {
            std::cout << "Error: Invalid animation type. Use: drawn, puppet, or plasticine" << std::endl;
            return false;
        }
    }
    else if (type == "game") {
        if (params.size() < 2) {
            std::cout << "Error: Game film requires title and director" << std::endl;
            return false;
        }
    }
    else if (type == "series") {
        if (params.size() < 3) {
            std::cout << "Error: Series film requires title, director, and episode count" << std::endl;
            return false;
        }
        try {
            int episodes = std::stoi(params[2]);
            if (episodes <= 0) {
                std::cout << "Error: Episode count must be positive" << std::endl;
                return false;
            }
        }
        catch (...) {
            std::cout << "Error: Invalid episode count '" << params[2] << "'. Must be a number" << std::endl;
            return false;
        }
    }
    else {
        std::cout << "Error: Unknown film type '" << type << "'. Use: cartoon, game, or series" << std::endl;
        return false;
    }
    return true;
}