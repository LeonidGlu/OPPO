#include <gtest/gtest.h>
#include <sstream>
#include <memory>
#include <string>
#include <fstream>
#include "FilmContainer.h"
#include "Film.h"
#include "GameFilm.h"
#include "CartoonFilm.h"
#include "SeriesFilm.h"

std::unique_ptr<Film> createFilm(const std::string& type, const std::string& title, const std::string& additionalData);
void processAddCommand(std::istringstream& iss, FilmContainer& container);
void processRemoveCommand(std::istringstream& iss, FilmContainer& container);
void commandFromFile(const std::string& filename, FilmContainer& container);

std::unique_ptr<Film> createFilm(const std::string& type, const std::string& title, const std::string& additionalData)
{
    if (type.empty() || title.empty()) {
        throw std::invalid_argument("Film type and title cannot be empty");
    }

    std::istringstream iss(additionalData);

    if (type == "game") {
        std::string director;
        std::getline(iss, director);
        if (director.empty()) {
            throw std::invalid_argument("Director cannot be empty for game film");
        }
        return std::make_unique<GameFilm>(title, director);
    }
    else if (type == "cartoon") {
        std::string animType;
        iss >> animType;
        if (animType.empty()) {
            throw std::invalid_argument("Animation type cannot be empty for cartoon film");
        }
        TypeCreation typeEnum;
        if (animType == "drawn") typeEnum = TypeCreation::Drawn;
        else if (animType == "puppet") typeEnum = TypeCreation::Doll;
        else if (animType == "plasticine") typeEnum = TypeCreation::Plasticine;
        else {
            throw std::invalid_argument("Invalid animation type: '" + animType + "'. Use: drawn, puppet, plasticine");
        }
        return std::make_unique<CartoonFilm>(title, typeEnum);
    }
    else if (type == "series") {
        std::string director;
        int episodes = 0;
        if (!std::getline(iss, director, '|')) {
            throw std::invalid_argument("Missing director for series film");
        }
        director.erase(0, director.find_first_not_of(" \t"));
        director.erase(director.find_last_not_of(" \t") + 1);
        if (director.empty()) {
            throw std::invalid_argument("Director cannot be empty for series film");
        }
        if (!(iss >> episodes)) {
            throw std::invalid_argument("Missing or invalid episode count for series film");
        }
        if (episodes <= 0) {
            throw std::invalid_argument("Episode count must be positive for series film");
        }
        return std::make_unique<SeriesFilm>(title, director, episodes);
    }
    else {
        throw std::invalid_argument("Unknown film type: '" + type + "'. Use: game, cartoon, or series");
    }
}

void processAddCommand(std::istringstream& iss, FilmContainer& container) {
    std::string type, title, additionalData;
    if (!(iss >> type)) {
        std::cout << "Error: Missing film type after ADD command" << std::endl;
        return;
    }
    iss >> std::ws;
    if (!std::getline(iss, title, '|')) {
        std::cout << "Error: Missing title for ADD command. Format: ADD type title|additional_data" << std::endl;
        return;
    }
    if (!title.empty() && title.front() == '"' && title.back() == '"') {
        title = title.substr(1, title.length() - 2);
    }
    std::getline(iss, additionalData);
    additionalData.erase(0, additionalData.find_first_not_of(" \t"));

    try {
        std::vector<std::string> params;
        params.push_back(title);
        if (type == "cartoon") {
            std::istringstream dataStream(additionalData);
            std::string animType;
            dataStream >> animType;
            params.push_back(animType);
        }
        else if (type == "game") {
            params.push_back(additionalData);
        }
        else if (type == "series") {
            std::istringstream dataStream(additionalData);
            std::string director;
            std::string episodesStr;
            std::getline(dataStream, director, '|');
            dataStream >> episodesStr;
            params.push_back(director);
            params.push_back(episodesStr);
        }
        if (!FilmContainer::validateAddCommand(type, params)) {
            return;
        }
        auto film = createFilm(type, title, additionalData);
        container.addFilm(std::move(film));
    }
    catch (const std::exception& e) {
        std::cout << "Error creating film: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Unknown error occurred while creating film" << std::endl;
    }
}

void processRemoveCommand(std::istringstream& iss, FilmContainer& container) {
    std::string condition;
    std::getline(iss >> std::ws, condition);
    if (condition.empty()) {
        std::cout << "Error: Missing condition for REM command" << std::endl;
        return;
    }
    container.removeFilms(condition);
}

void commandFromFile(const std::string& filename, FilmContainer& container) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Cannot open file '" << filename << "'" << std::endl;
        return;
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty() || line[0] == '#') {
            continue;
        }
        std::istringstream iss(line);
        std::string command;
        if (!(iss >> command)) {
            continue;
        }

        try {
            if (command == "ADD") {
                processAddCommand(iss, container);
            }
            else if (command == "REM") {
                processRemoveCommand(iss, container);
            }
            else if (command == "PRINT") {
                container.printAll();
            }
            else {
                std::cout << "Error: Unknown command '" << command << "' at line " << lineNumber << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error processing command at line " << lineNumber << ": " << e.what() << std::endl;
        }
        catch (...) {
            std::cout << "Unknown error processing command at line " << lineNumber << std::endl;
        }
    }
    std::cout << "Finished processing file. Total films in container: " << container.size() << std::endl;
}


TEST(CreateFilmTest, CreateGameFilm) {
    auto film = createFilm("game", "The Matrix", "Wachowski");
    EXPECT_EQ(film->getTitle(), "The Matrix");
    EXPECT_EQ(film->getType(), "game");

    testing::internal::CaptureStdout();
    film->print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Game film: The Matrix") != std::string::npos);
}

TEST(CreateFilmTest, CreateCartoonFilm) {
    auto film = createFilm("cartoon", "Shrek", "drawn");
    EXPECT_EQ(film->getTitle(), "Shrek");
    EXPECT_EQ(film->getType(), "cartoon");
}

TEST(CreateFilmTest, CreateSeriesFilm) {
    auto film = createFilm("series", "Breaking Bad", "Vince Gilligan|62");
    EXPECT_EQ(film->getTitle(), "Breaking Bad");
    EXPECT_EQ(film->getType(), "series");
}

TEST(CreateFilmTest, InvalidInputs) {
    // Пустой тип или название
    EXPECT_THROW(createFilm("", "Title", "data"), std::invalid_argument);
    EXPECT_THROW(createFilm("game", "", "Director"), std::invalid_argument);

    // Неизвестный тип
    EXPECT_THROW(createFilm("unknown", "Title", "data"), std::invalid_argument);

    // Неправильные данные для game film
    EXPECT_THROW(createFilm("game", "Title", ""), std::invalid_argument);

    // Неправильные данные для cartoon film
    EXPECT_THROW(createFilm("cartoon", "Title", ""), std::invalid_argument);
    EXPECT_THROW(createFilm("cartoon", "Title", "invalid_type"), std::invalid_argument);

    // Неправильные данные для series film
    EXPECT_THROW(createFilm("series", "Title", ""), std::invalid_argument);
    EXPECT_THROW(createFilm("series", "Title", "Director|"), std::invalid_argument);
    EXPECT_THROW(createFilm("series", "Title", "|10"), std::invalid_argument);
    EXPECT_THROW(createFilm("series", "Title", "Director|0"), std::invalid_argument);
    EXPECT_THROW(createFilm("series", "Title", "Director|-5"), std::invalid_argument);
    EXPECT_THROW(createFilm("series", "Title", "Director|abc"), std::invalid_argument);
}

TEST(ProcessCommandsTest, ProcessAddCommand) {
    FilmContainer container;

    // Тест успешного добавления
    std::istringstream iss1("game \"The Matrix\"|Wachowski");
    testing::internal::CaptureStdout();
    processAddCommand(iss1, container);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Film added successfully") != std::string::npos);
    EXPECT_EQ(container.size(), 1);

    // Тест с ошибкой - недостаточно параметров
    std::istringstream iss2("game");
    testing::internal::CaptureStdout();
    processAddCommand(iss2, container);
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Error: Missing film type after ADD command") != std::string::npos);

    // Тест с ошибкой - неправильный формат
    std::istringstream iss3("cartoon Shrek");
    testing::internal::CaptureStdout();
    processAddCommand(iss3, container);
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Error: Missing title for ADD command") != std::string::npos);
}

TEST(ProcessCommandsTest, ProcessRemoveCommand) {
    FilmContainer container;
    container.addFilm(std::make_unique<GameFilm>("Film1", "Director1"));
    container.addFilm(std::make_unique<GameFilm>("Film2", "Director2"));
    container.addFilm(std::make_unique<CartoonFilm>("Cartoon1", TypeCreation::Drawn));

    // Удаление по условию
    std::istringstream iss("type == game");
    testing::internal::CaptureStdout();
    processRemoveCommand(iss, container);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Successfully removed 2 film(s)") != std::string::npos);
    EXPECT_EQ(container.size(), 1);

    // Пустое условие
    std::istringstream iss2("");
    testing::internal::CaptureStdout();
    processRemoveCommand(iss2, container);
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Error: Missing condition for REM command") != std::string::npos);
}

TEST(FileProcessingTest, CommandFromFile) {
    FilmContainer container;

    // Создание тестового файла
    const std::string testFilename = "test_commands.txt";
    std::ofstream testFile(testFilename);
    testFile << "# This is a comment\n";
    testFile << "ADD game \"The Matrix\"|Wachowski\n";
    testFile << "ADD cartoon Shrek|drawn\n";
    testFile << "PRINT\n";
    testFile << "REM type == game\n";
    testFile << "PRINT\n";
    testFile.close();

    testing::internal::CaptureStdout();
    commandFromFile(testFilename, container);
    std::string output = testing::internal::GetCapturedStdout();

    // Проверяем вывод
    EXPECT_TRUE(output.find("Film added successfully") != std::string::npos);
    EXPECT_TRUE(output.find("Films in container") != std::string::npos);
    EXPECT_TRUE(output.find("Successfully removed") != std::string::npos);
    EXPECT_TRUE(output.find("Finished processing file") != std::string::npos);

    // Удаляем тестовый файл
    std::remove(testFilename.c_str());
}

TEST(FileProcessingTest, InvalidFile) {
    FilmContainer container;

    testing::internal::CaptureStdout();
    commandFromFile("non_existent_file.txt", container);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Error: Cannot open file") != std::string::npos);
}
