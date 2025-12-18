#pragma once
#include "Film.h"
#include <memory>
#include <vector>

class FilmContainer {
private:
    std::vector<std::unique_ptr<Film>> films;

public:
    void addFilm(std::unique_ptr<Film> film);
    void removeFilms(const std::string& condition);
    void printAll() const;
    size_t size() const;
    static bool validateAddCommand(const std::string& type, const std::vector<std::string>& params);
};

