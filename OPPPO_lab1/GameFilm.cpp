#include "GameFilm.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

GameFilm::GameFilm(const std::string& title, const std::string& director)
    : Film(title), director(director) {
    if (title.empty()) {
        throw std::invalid_argument("Title cannot be empty");
    }
    if (director.empty()) {
        throw std::invalid_argument("Director cannot be empty");
    }
}

const std::string& GameFilm::getDirector() const {
    return director;
}

std::string GameFilm::getType() const {
    return "game";
}

void GameFilm::print() const {
    std::cout << "Game film: " << title << ", director: " << director << std::endl;
}

bool GameFilm::matchesCondition(const std::string& condition) const {
    if (condition.empty()) {
        return false;
    }

    std::istringstream iss(condition);
    std::string field, op;
    iss >> field >> op;

    std::string value;
    std::getline(iss, value);

    value.erase(0, value.find_first_not_of(" \t"));
    value.erase(value.find_last_not_of(" \t") + 1);

    if (field == "title") {
        if (op == "==") return title == value;
        if (op == "!=") return title != value;
        if (op == "contains") return title.find(value) != std::string::npos;
    }
    else if (field == "director") {
        if (op == "==") return director == value;
        if (op == "!=") return director != value;
        if (op == "contains") return director.find(value) != std::string::npos;
    }
    else if (field == "type" && op == "==") {
        return value == "game";
    }

    return false;
}
