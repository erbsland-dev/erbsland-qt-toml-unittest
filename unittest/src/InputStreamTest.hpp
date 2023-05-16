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

#include <erbsland/qt/toml/InputStream.hpp>

#include <QtCore/QString>


using elqt::toml::InputStream;


class InputStreamTest : public el::UnitTest {
public:
    void testString() {
        auto text = QString::fromUtf8("abc⚫");
        auto inputStream = InputStream::createFromString(text);
        REQUIRE(inputStream != nullptr);
        REQUIRE_FALSE(inputStream->atEnd());
        REQUIRE(inputStream->type() == InputStream::Type::String);
        REQUIRE(inputStream->document() == QStringLiteral("[string]"));
        auto c = inputStream->readOrThrow();
        REQUIRE(c == 'a');
        REQUIRE_FALSE(inputStream->atEnd());
        c = inputStream->readOrThrow();
        REQUIRE(c == 'b');
        REQUIRE_FALSE(inputStream->atEnd());
        c = inputStream->readOrThrow();
        REQUIRE(c == 'c');
        REQUIRE_FALSE(inputStream->atEnd());
        c = inputStream->readOrThrow();
        REQUIRE(c == 0x26ab);
        REQUIRE(inputStream->atEnd());
        c = inputStream->readOrThrow(); // undefined, but should not crash.
        REQUIRE(inputStream->atEnd()); // still at end.
    }

    void testByteArray() {
        auto data = QByteArray{};
        data.append('a');
        data.append('b');
        data.append('c');
        data.append('\xe2'); // utf-8 for => ⚫ (0x26ab)
        data.append('\x9a');
        data.append('\xab');
        auto inputStream = InputStream::createFromString(data);
        REQUIRE(inputStream != nullptr);
        REQUIRE_FALSE(inputStream->atEnd());
        REQUIRE(inputStream->type() == InputStream::Type::String);
        REQUIRE(inputStream->document() == QStringLiteral("[string]"));
        auto c = inputStream->readOrThrow();
        REQUIRE(c == 'a');
        REQUIRE_FALSE(inputStream->atEnd());
        c = inputStream->readOrThrow();
        REQUIRE(c == 'b');
        REQUIRE_FALSE(inputStream->atEnd());
        c = inputStream->readOrThrow();
        REQUIRE(c == 'c');
        REQUIRE_FALSE(inputStream->atEnd());
        c = inputStream->readOrThrow();
        REQUIRE(c == 0x26abU);
        REQUIRE(inputStream->atEnd());
        c = inputStream->readOrThrow(); // undefined, but should not crash.
        REQUIRE(inputStream->atEnd()); // still at end.
    }

    void testFile() {
        auto filePath = unitTestExecutablePath().parent_path() / "data" / "input-stream" / "minimal_read.txt";
        auto filePathQt = QString::fromStdString(filePath.string());
        auto inputStream = InputStream::createFromFileOrThrow(filePathQt);
        REQUIRE(inputStream != nullptr);
        REQUIRE_FALSE(inputStream->atEnd());
        REQUIRE(inputStream->type() == InputStream::Type::File);
        REQUIRE(inputStream->document() == filePathQt);
        auto c = inputStream->readOrThrow();
        REQUIRE(c == 'a');
        REQUIRE_FALSE(inputStream->atEnd());
        c = inputStream->readOrThrow();
        REQUIRE(c == 'b');
        REQUIRE_FALSE(inputStream->atEnd());
        c = inputStream->readOrThrow();
        REQUIRE(c == 'c');
        REQUIRE_FALSE(inputStream->atEnd());
        c = inputStream->readOrThrow();
        REQUIRE(c == 0x26abU);
        REQUIRE(inputStream->atEnd());
        c = inputStream->readOrThrow(); // undefined, but should not crash.
        REQUIRE(inputStream->atEnd()); // still at end.
    }
};


