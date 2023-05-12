// Copyright © 2023 Tobias Erbsland. Web: https://erbsland.dev
// Copyright © 2023 EducateIT GmbH. Web: https://educateit.ch
//
// This program is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with this program.
// If not, see <https://www.gnu.org/licenses/>.
#pragma once


#include <erbsland/unittest/UnitTest.hpp>

#include <erbsland/qt/toml/Error.hpp>


using elqt::toml::Error;
using elqt::toml::Location;


namespace {
class DummyIODevice : public QIODevice {
public:
    QString errorMessage;
    explicit DummyIODevice(const QString &errorMessage) { setErrorString(errorMessage); }
    [[nodiscard]] auto isSequential() const -> bool override { return false; }
    auto readData(char *data, qint64 maxlen) -> qint64 override { return {}; }
    auto writeData(const char *data, qint64 len) -> qint64 override { return {}; }
};
}


class ErrorTest : public el::UnitTest {
public:
    void testGenericError() {
        // Test the default constructor
        Error errDefault;
        REQUIRE(errDefault.toString() == "Generic");

        // Test the constructor with a message.
        Error errMessage("Test message");
        REQUIRE(errMessage.toString() == "Generic: Test message");
    }

    void testIOError() {
        // Create a dummy QIODevice to simulate an IO error.
        DummyIODevice dummyIODevice("Test IO error");

        // Create an IO error and check its properties.
        auto err = Error::createIO("test.txt", dummyIODevice);
        REQUIRE(err.toString() == "IO in test.txt: Test IO error");
    }

    void testEncodingError() {
        // Create an encoding error and check its properties.
        auto err = Error::createEncoding("test.txt", Location(42, 2, 13));
        REQUIRE(err.toString() == "Encoding in test.txt at line 2, column 13 (index 42): Corrupted data in UTF-8 encoding.");
    }

    void testSyntaxError() {
        // Create a syntax error and check its properties.
        auto err = Error::createSyntax("test.txt", Location(42, 2, 13), "Unexpected character.");
        REQUIRE(err.toString() == "Syntax in test.txt at line 2, column 13 (index 42): Unexpected character.");
    }

    void testToString() {
        // Create a generic error and check its string representation.
        Error err("Test message");
        REQUIRE(err.toString() == "Generic: Test message");
    }

    void testWhat() {
        // Create a generic error and check the output of the what() method.
        Error err("Test message");
        REQUIRE(std::string(err.what()) == "Generic: Test message");
    }
};

