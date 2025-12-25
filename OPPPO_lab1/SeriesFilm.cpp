#include "SeriesFilm.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

SeriesFilm::SeriesFilm(const std::string& title, const std::string& director, int episodeCount)
    : Film(title), director(director), episodeCount(episodeCount) {
    if (episodeCount <= 0) {
        throw std::invalid_argument("Episode count must be positive");
    }
    if (director.empty()) {
        throw std::invalid_argument("Director cannot be empty");
    }
    if (title.empty()) {
        throw std::invalid_argument("Title cannot be empty");
    }
}

const std::string& SeriesFilm::getDirector() const {
    return director;
}

int SeriesFilm::getEpisode() const {
    return episodeCount;
}

void SeriesFilm::print() const {
    std::cout << "Series film: " << title << ", director: " << director << ", episodes: " << episodeCount << std::endl;
}

std::string SeriesFilm::getType() const {
    return "series";
}

bool SeriesFilm::matchesCondition(const std::string& condition) const {
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
    else if (field == "episodes") {
        try {
            int numValue = std::stoi(value);
            if (op == "==") return episodeCount == numValue;
            if (op == "!=") return episodeCount != numValue;
            if (op == ">") return episodeCount > numValue;
            if (op == "<") return episodeCount < numValue;
            if (op == ">=") return episodeCount >= numValue;
            if (op == "<=") return episodeCount <= numValue;
        }
        catch (const std::invalid_argument&) {
            return false;
        }
        catch (const std::out_of_range&) {
            return false;
        }
    }
    else if (field == "type" && op == "==") {
        return value == "series";
    }

    return false;
}
