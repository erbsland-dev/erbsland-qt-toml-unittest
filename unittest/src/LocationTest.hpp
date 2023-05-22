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

#include <erbsland/qt/toml/Location.hpp>


using elqt::toml::Location;


class LocationTest : public el::UnitTest {
public:
    Location location;

    void testConstruction() {
        location = {};
        REQUIRE(location.index() == 0);
        REQUIRE(location.line() == 1);
        REQUIRE(location.column() == 1);

        location = Location{5, 2, 3};
        REQUIRE(location.index() == 5);
        REQUIRE(location.line() == 2);
        REQUIRE(location.column() == 3);

        location = Location{-1, -1, -1};
        REQUIRE(location.index() == -1);
        REQUIRE(location.line() == -1);
        REQUIRE(location.column() == -1);
    }

    void testIncrement() {
        location = {};
        location.increment(false);
        REQUIRE(location.index() == 1);
        REQUIRE(location.line() == 1);
        REQUIRE(location.column() == 2);

        location.increment(true);
        REQUIRE(location.index() == 2);
        REQUIRE(location.line() == 2);
        REQUIRE(location.column() == 1);
    }

    void testIsNotSet() {
        location = {};
        REQUIRE_FALSE(location.isNotSet());

        location = Location{-1, 0, 0};
        REQUIRE(location.isNotSet());

        location = Location{0, -1, 0};
        REQUIRE(location.isNotSet());

        location = Location{0, 0, -1};
        REQUIRE(location.isNotSet());
    }

    void testToString() {
        location = {};
        REQUIRE(location.toString(Location::Format::Compact) == "1:1");
        REQUIRE(location.toString(Location::Format::CompactWithIndex) == "1:1(0)");
        REQUIRE(location.toString(Location::Format::Long) == "line 1, column 1");
        REQUIRE(location.toString(Location::Format::LongWithIndex) == "line 1, column 1 (index 0)");

        location = Location{5, 2, 3};
        REQUIRE(location.toString(Location::Format::Compact) == "2:3");
        REQUIRE(location.toString(Location::Format::CompactWithIndex) == "2:3(5)");
        REQUIRE(location.toString(Location::Format::Long) == "line 2, column 3");
        REQUIRE(location.toString(Location::Format::LongWithIndex) == "line 2, column 3 (index 5)");

        location = Location{-1, -1, -1};
        REQUIRE(location.toString(Location::Format::Compact) == "?:?");
        REQUIRE(location.toString(Location::Format::CompactWithIndex) == "?:?(?)");
        REQUIRE(location.toString(Location::Format::Long) == "unknown location");
        REQUIRE(location.toString(Location::Format::LongWithIndex) == "unknown location");
    }
};

