#pragma once
#include "Film.h"
#include <string>

enum class TypeCreation 
{
	Drawn,
	Doll,
	Plasticine,
};

class CartoonFilm : public Film
{
private:
	TypeCreation creation;

public:
	CartoonFilm(const std::string& title, TypeCreation creation);
	
	void print() const override;
	TypeCreation getCreation() const;

	bool matchesCondition(const std::string& condition) const override;
	std::string getType() const override;
};

