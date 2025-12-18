#pragma once
#include "Film.h"
#include <string>

class SeriesFilm : public Film
{
private:
	std::string director;
	int episodeCount;

public:
	SeriesFilm(const std::string& title, const std::string& director, int episodeCount);

	const std::string& getDirector() const;
	int getEpisode() const;

	void print() const override;

	bool matchesCondition(const std::string& condition) const override;
	std::string getType() const override;
};

