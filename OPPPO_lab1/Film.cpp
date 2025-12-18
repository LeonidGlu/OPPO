#include "Film.h"

Film::Film(const std::string& title) : title(title) {}

const std::string& Film::getTitle() const {
	return title;
}