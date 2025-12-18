#include "CartoonFilm.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

CartoonFilm::CartoonFilm(const std::string& title, TypeCreation creation)
    : Film(title), creation(creation) {
    if (title.empty()) {
        throw std::invalid_argument("Title cannot be empty");
    }
}

void CartoonFilm::print() const {
    std::string typeStr;
    switch (creation) {
    case TypeCreation::Drawn: typeStr = "drawn"; break;
    case TypeCreation::Doll: typeStr = "doll"; break;
    case TypeCreation::Plasticine: typeStr = "plasticine"; break;
    default: typeStr = "unknown"; break;
    }
    std::cout << "Cartoon film: " << title << ", animation type: " << typeStr << std::endl;
}

TypeCreation CartoonFilm::getCreation() const {
    return creation;
}

std::string CartoonFilm::getType() const {
    return "cartoon";
}

bool CartoonFilm::matchesCondition(const std::string& condition) const {
    if (condition.empty()) {
        return false;
    }

    std::istringstream iss(condition);
    std::string field, op, value;
    iss >> field >> op >> value;

    if (field == "title") {
        if (op == "==") return title == value;
        if (op == "!=") return title != value;
        if (op == "contains") return title.find(value) != std::string::npos;
    }
    else if (field == "animation_type") {
        TypeCreation condType;
        if (value == "drawn") condType = TypeCreation::Drawn;
        else if (value == "doll") condType = TypeCreation::Doll;
        else if (value == "plasticine") condType = TypeCreation::Plasticine;
        else {
            throw std::invalid_argument("Invalid animation type: '" + value + "'. Use: drawn, puppet, plasticine");
        }

        if (op == "==") return creation == condType;
        if (op == "!=") return creation != condType;
    }
    else if (field == "type" && op == "==") {
        return value == "cartoon";
    }

    return false;
}