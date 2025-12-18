#pragma once
#include "Film.h"
#include <string>

class GameFilm : public Film
{
private:
	std::string director;

public: 
	GameFilm(const std::string& title, const std::string& director);
	
	const std::string& getDirector() const;

	void print() const override;
	bool matchesCondition(const std::string& condition) const override;
	std::string getType() const override;
};

