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
    void testConstruction() {
        Location l1;
        REQUIRE(l1.index() == 0);
        REQUIRE(l1.line() == 1);
        REQUIRE(l1.column() == 1);

        Location l2{5, 2, 3};
        REQUIRE(l2.index() == 5);
        REQUIRE(l2.line() == 2);
        REQUIRE(l2.column() == 3);

        Location l3{-1, -1, -1};
        REQUIRE(l3.index() == -1);
        REQUIRE(l3.line() == -1);
        REQUIRE(l3.column() == -1);
    }

    void testIncrement() {
        Location l1;
        l1.increment(false);
        REQUIRE(l1.index() == 1);
        REQUIRE(l1.line() == 1);
        REQUIRE(l1.column() == 2);

        l1.increment(true);
        REQUIRE(l1.index() == 2);
        REQUIRE(l1.line() == 2);
        REQUIRE(l1.column() == 1);
    }

    void testIsNegative() {
        Location l1;
        REQUIRE_FALSE(l1.isNegative());

        Location l2{-1, 0, 0};
        REQUIRE(l2.isNegative());

        Location l3{0, -1, 0};
        REQUIRE(l3.isNegative());

        Location l4{0, 0, -1};
        REQUIRE(l4.isNegative());
    }
};

