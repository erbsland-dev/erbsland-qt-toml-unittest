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

#include <erbsland/qt/toml/Char.hpp>

#include <QtCore/QVector>


using elqt::toml::Char;


class CharTest : public el::UnitTest {
public:
    Char c{};

    void testDefault() {
        REQUIRE(c.isNull());
        REQUIRE(c.isValidUnicode());
        REQUIRE(c == 0U);
    }

    void testCopy() {
        Char c2 = c;
        REQUIRE(c2 == c);
        REQUIRE(c2.isNull());
        REQUIRE(c2.isValidUnicode());

        Char c3{'A'};
        c2 = c3;
        REQUIRE(c2 == c3);
        REQUIRE(!c2.isNull());
        REQUIRE(c2.isValidUnicode());
        REQUIRE(c2 == 'A');
    }

    void testComparison() {
        c = Char{'A'};

        REQUIRE(c == 'A');
        REQUIRE(c != 'B');
        REQUIRE(c > '9');
        REQUIRE(c >= 'A');
        REQUIRE(c < 'Z');
        REQUIRE(c <= 'A');

        REQUIRE(c == 65);
        REQUIRE(c != 66);
        REQUIRE(c > 57);
        REQUIRE(c >= 65);
        REQUIRE(c < 90);
        REQUIRE(c <= 65);

        Char c2{'Z'};
        REQUIRE(c < c2);
        REQUIRE(c <= c2);
        REQUIRE(c != c2);
        REQUIRE(c2 > c);
        REQUIRE(c2 >= c);
        REQUIRE(c2 != c);
    }

    void testNegativeValues() {
        // Initialize Char instance with a low number
        c = Char{0x40U};

        // Check the comparison operations with a negative number
        REQUIRE_FALSE(c == -1);
        REQUIRE(c != -1);
        REQUIRE(c > -1);
        REQUIRE(c >= -1);
        REQUIRE_FALSE(c < -1);
        REQUIRE_FALSE(c <= -1);
    }

    void testValidity() {
        c = Char{'A'};
        REQUIRE(!c.isNull());
        REQUIRE(c.isValidUnicode());

        // Test for values just outside the invalid Unicode range.
        c = Char{0xD7FFU};
        REQUIRE(c.isValidUnicode());

        c = Char{0xd800U};
        REQUIRE_FALSE(c.isValidUnicode());

        c = Char{0xE000U};
        REQUIRE(c.isValidUnicode());

        // Test for the maximum valid Unicode value.
        c = Char{0x10FFFFU};
        REQUIRE(c.isValidUnicode());

        // Test for values just outside the valid Unicode range.
        c = Char{0x110000U};
        REQUIRE_FALSE(c.isValidUnicode());

        c = Char{0x11FFFFU};
        REQUIRE_FALSE(c.isValidUnicode());
    }

    void testConversion() {
        c = Char{'A'};
        REQUIRE(c.toAscii() == 'A');

        QString str;
        c.appendToString(str);
        REQUIRE(str == "A");

        // Test for a value outside ASCII range.
        c = Char{0x80U};
        REQUIRE(c.toAscii() == '\0');
    }

    void testInvalidUnicode() {
        // Test for values in the range 0xD800 to 0xDFFF.
        for (char32_t i = 0xD800U; i <= 0xDFFFU; ++i) {
            c = Char{i};
            REQUIRE_FALSE(c.isValidUnicode());
        }

        // Test for values greater than 0x10FFFF.
        c = Char{0x110000U};
        REQUIRE_FALSE(c.isValidUnicode());
    }

    void testAppendHighUnicode() {
        // Test for a value greater than 0x10000U.
        // 0x10437 is the code point for the DESERET CAPITAL LETTER TSE, a valid Unicode character.
        c = Char{0x10437U};
        REQUIRE(c.isValidUnicode());

        QString str;
        c.appendToString(str);
        auto ucs4str = str.toUcs4();
        REQUIRE(ucs4str.size() == 1);
        REQUIRE(ucs4str[0] == 0x10437U);
    }
};


