#include <gtest/gtest.h>
#include "FilmContainer.h"
#include "GameFilm.h"
#include "CartoonFilm.h"
#include "SeriesFilm.h"

TEST(FilmContainerTest, AddAndSize) {
    FilmContainer container;
    EXPECT_EQ(container.size(), 0);

    auto film1 = std::make_unique<GameFilm>("Film1", "Director1");
    container.addFilm(std::move(film1));
    EXPECT_EQ(container.size(), 1);

    auto film2 = std::make_unique<CartoonFilm>("Film2", TypeCreation::Drawn);
    container.addFilm(std::move(film2));
    EXPECT_EQ(container.size(), 2);
}

TEST(FilmContainerTest, AddNullFilm) {
    FilmContainer container;
    testing::internal::CaptureStdout();
    container.addFilm(nullptr);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Error: Cannot add null film") != std::string::npos);
}

TEST(FilmContainerTest, RemoveFilms) {
    FilmContainer container;

    container.addFilm(std::make_unique<GameFilm>("Matrix", "Wachowski"));
    container.addFilm(std::make_unique<GameFilm>("Inception", "Nolan"));
    container.addFilm(std::make_unique<CartoonFilm>("Shrek", TypeCreation::Drawn));

    EXPECT_EQ(container.size(), 3);

    testing::internal::CaptureStdout();
    container.removeFilms("type == game");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(container.size(), 1);
    EXPECT_TRUE(output.find("Successfully removed 2 film(s)") != std::string::npos);

    container.removeFilms("title == Shrek");
    EXPECT_EQ(container.size(), 0);
}

TEST(FilmContainerTest, RemoveWithEmptyCondition) {
    FilmContainer container;
    container.addFilm(std::make_unique<GameFilm>("Test", "Director"));

    testing::internal::CaptureStdout();
    container.removeFilms("");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Error: Empty condition provided") != std::string::npos);
}

TEST(FilmContainerTest, PrintAll) {
    FilmContainer container;

    testing::internal::CaptureStdout();
    container.printAll();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Container is empty") != std::string::npos);

    container.addFilm(std::make_unique<GameFilm>("Film1", "Director1"));
    container.addFilm(std::make_unique<CartoonFilm>("Film2", TypeCreation::Doll));
    testing::internal::CaptureStdout();
    container.printAll();
    output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Films in container (2 total)") != std::string::npos);
    EXPECT_TRUE(output.find("Game film: Film1") != std::string::npos);
    EXPECT_TRUE(output.find("Cartoon film: Film2") != std::string::npos);
}

TEST(FilmContainerTest, ValidateAddCommand) {

    std::vector<std::string> cartoonParams = {"Shrek", "drawn"};
    EXPECT_TRUE(FilmContainer::validateAddCommand("cartoon", cartoonParams));

    std::vector<std::string> gameParams = {"Matrix", "Wachowski"};
    EXPECT_TRUE(FilmContainer::validateAddCommand("game", gameParams));

    std::vector<std::string> seriesParams = {"Breaking Bad", "Vince Gilligan", "62"};
    EXPECT_TRUE(FilmContainer::validateAddCommand("series", seriesParams));

    std::vector<std::string> invalidCartoon = {"Shrek", "invalid_type"};
    EXPECT_FALSE(FilmContainer::validateAddCommand("cartoon", invalidCartoon));
    std::vector<std::string> invalidSeries = {"Breaking Bad", "Vince Gilligan", "not_a_number"};
    EXPECT_FALSE(FilmContainer::validateAddCommand("series", invalidSeries));

    std::vector<std::string> emptyParams;
    EXPECT_FALSE(FilmContainer::validateAddCommand("unknown_type", emptyParams));
}
