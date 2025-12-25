#include <gtest/gtest.h>
#include "Film.h"
#include "GameFilm.h"
#include "CartoonFilm.h"
#include "SeriesFilm.h"

TEST(GameFilmTest, Constructor) {
    GameFilm film("Game Title", "Director Name");
    EXPECT_EQ(film.getTitle(), "Game Title");
    EXPECT_EQ(film.getDirector(), "Director Name");
    EXPECT_EQ(film.getType(), "game");
}

TEST(GameFilmTest, InvalidConstructor) {
    EXPECT_THROW(GameFilm("", "Director"), std::invalid_argument);
    EXPECT_THROW(GameFilm("Title", ""), std::invalid_argument);
}

TEST(GameFilmTest, Print) {
    GameFilm film("Matrix", "Wachowski");
    testing::internal::CaptureStdout();
    film.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Game film: Matrix") != std::string::npos);
    EXPECT_TRUE(output.find("director: Wachowski") != std::string::npos);
}

TEST(GameFilmTest, MatchesCondition) {
    GameFilm film("Inception", "Christopher Nolan");
    EXPECT_TRUE(film.matchesCondition("title == Inception"));
    EXPECT_FALSE(film.matchesCondition("title == Titanic"));
    EXPECT_TRUE(film.matchesCondition("title contains Incep"));
    EXPECT_TRUE(film.matchesCondition("director == Christopher Nolan"));
    EXPECT_TRUE(film.matchesCondition("type == game"));
    EXPECT_FALSE(film.matchesCondition("type == cartoon"));
    EXPECT_THROW(film.matchesCondition("invalid_field == value"), std::exception);
}

// Тестирование CartoonFilm
TEST(CartoonFilmTest, Constructor) {
    CartoonFilm film("Shrek", TypeCreation::Drawn);
    EXPECT_EQ(film.getTitle(), "Shrek");
    EXPECT_EQ(film.getCreation(), TypeCreation::Drawn);
    EXPECT_EQ(film.getType(), "cartoon");
}

TEST(CartoonFilmTest, Print) {
    CartoonFilm film("Shrek", TypeCreation::Drawn);
    testing::internal::CaptureStdout();
    film.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Cartoon film: Shrek") != std::string::npos);
    EXPECT_TRUE(output.find("animation type: drawn") != std::string::npos);
}

TEST(CartoonFilmTest, MatchesCondition) {
    CartoonFilm film("Toy Story", TypeCreation::Plasticine);
    EXPECT_TRUE(film.matchesCondition("title == Toy Story"));
    EXPECT_TRUE(film.matchesCondition("animation_type == plasticine"));
    EXPECT_FALSE(film.matchesCondition("animation_type == drawn"));
    EXPECT_TRUE(film.matchesCondition("type == cartoon")); 
    EXPECT_THROW(film.matchesCondition("animation_type == invalid"), std::invalid_argument);
}


TEST(SeriesFilmTest, Constructor) {
    SeriesFilm film("Breaking Bad", "Vince Gilligan", 62);
    EXPECT_EQ(film.getTitle(), "Breaking Bad");
    EXPECT_EQ(film.getDirector(), "Vince Gilligan");
    EXPECT_EQ(film.getEpisode(), 62);
    EXPECT_EQ(film.getType(), "series");
}

TEST(SeriesFilmTest, InvalidConstructor) {
    EXPECT_THROW(SeriesFilm("", "Director", 10), std::invalid_argument);
    EXPECT_THROW(SeriesFilm("Title", "", 10), std::invalid_argument);
    EXPECT_THROW(SeriesFilm("Title", "Director", 0), std::invalid_argument);
    EXPECT_THROW(SeriesFilm("Title", "Director", -5), std::invalid_argument);
}

TEST(SeriesFilmTest, MatchesCondition) {
    SeriesFilm film("Game of Thrones", "David Benioff", 73);
    EXPECT_TRUE(film.matchesCondition("title == Game of Thrones"));
    EXPECT_TRUE(film.matchesCondition("director == David Benioff"));
    EXPECT_TRUE(film.matchesCondition("episodes == 73"));
    EXPECT_TRUE(film.matchesCondition("episodes > 50"));
    EXPECT_TRUE(film.matchesCondition("episodes < 100"));
    EXPECT_FALSE(film.matchesCondition("episodes < 50"));
    EXPECT_TRUE(film.matchesCondition("type == series"));
    EXPECT_THROW(film.matchesCondition("episodes == abc"), std::invalid_argument);
}
