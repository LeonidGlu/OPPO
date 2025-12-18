#pragma once
#include <string>
#include <iostream>

class Film
{
protected:
	std::string title;

public:
	Film(const std::string& title);
	virtual ~Film() = default;

	const std::string& getTitle() const;

	virtual void print() const = 0;
	virtual bool matchesCondition(const std::string& consition) const = 0;
	virtual std::string getType() const = 0;
};

