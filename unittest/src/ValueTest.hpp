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

#include <erbsland/qt/toml/Value.hpp>


using elqt::toml::Value;


class ValueTest : public el::UnitTest {
public:
    void testConstruction() {
        // Integer creation
        auto integerValue = Value::createInteger(42);
        REQUIRE(integerValue->type() == Value::Type::Integer);
        REQUIRE(integerValue->source() == Value::Source::Value)
        REQUIRE_FALSE(integerValue->isTable());
        REQUIRE_FALSE(integerValue->isArray());

        // Float creation
        auto floatValue = Value::createFloat(42.0);
        REQUIRE(floatValue->type() == Value::Type::Float);
        REQUIRE(floatValue->source() == Value::Source::Value)
        REQUIRE_FALSE(floatValue->isTable());
        REQUIRE_FALSE(floatValue->isArray());

        // Boolean creation
        auto booleanValue = Value::createBoolean(true);
        REQUIRE(booleanValue->type() == Value::Type::Boolean);
        REQUIRE(booleanValue->source() == Value::Source::Value)
        REQUIRE_FALSE(booleanValue->isTable());
        REQUIRE_FALSE(booleanValue->isArray());

        // String creation
        auto stringValue = Value::createString("test");
        REQUIRE(stringValue->type() == Value::Type::String);
        REQUIRE(stringValue->source() == Value::Source::Value)
        REQUIRE_FALSE(stringValue->isTable());
        REQUIRE_FALSE(stringValue->isArray());

        // Time creation
        auto timeValue = Value::createTime(QTime::currentTime());
        REQUIRE(timeValue->type() == Value::Type::Time);
        REQUIRE(timeValue->source() == Value::Source::Value)
        REQUIRE_FALSE(timeValue->isTable());
        REQUIRE_FALSE(timeValue->isArray());

        // Date creation
        auto dateValue = Value::createDate(QDate::currentDate());
        REQUIRE(dateValue->type() == Value::Type::Date);
        REQUIRE(dateValue->source() == Value::Source::Value)
        REQUIRE_FALSE(dateValue->isTable());
        REQUIRE_FALSE(dateValue->isArray());

        // DateTime creation
        auto dateTimeValue = Value::createDateTime(QDateTime::currentDateTime());
        REQUIRE(dateTimeValue->type() == Value::Type::DateTime);
        REQUIRE(dateTimeValue->source() == Value::Source::Value)
        REQUIRE_FALSE(dateTimeValue->isTable());
        REQUIRE_FALSE(dateTimeValue->isArray());

        // Table creation
        auto tableValue = Value::createTable(Value::Source::ExplicitTable);
        REQUIRE(tableValue->type() == Value::Type::Table);
        REQUIRE(tableValue->source() == Value::Source::ExplicitTable)
        REQUIRE(tableValue->isTable());
        REQUIRE_FALSE(tableValue->isArray());

        // Array creation
        auto arrayValue = Value::createArray(Value::Source::ExplicitValue);
        REQUIRE(arrayValue->type() == Value::Type::Array);
        REQUIRE(arrayValue->source() == Value::Source::ExplicitValue)
        REQUIRE_FALSE(arrayValue->isTable());
        REQUIRE(arrayValue->isArray());
    }

    void testSourceAndMakeExplicit() {
        // Implicit Table
        auto implicitTableValue = Value::createTable(Value::Source::ImplicitTable);
        REQUIRE(implicitTableValue->source() == Value::Source::ImplicitTable);
        implicitTableValue->makeExplicit();
        REQUIRE(implicitTableValue->source() == Value::Source::ExplicitTable);

        // Explicit Table
        auto explicitTableValue = Value::createTable(Value::Source::ExplicitTable);
        REQUIRE(explicitTableValue->source() == Value::Source::ExplicitTable);
        explicitTableValue->makeExplicit();
        REQUIRE(explicitTableValue->source() == Value::Source::ExplicitTable); // should remain unchanged

        // Implicit Value
        auto implicitArrayValue = Value::createArray(Value::Source::ImplicitValue);
        REQUIRE(implicitArrayValue->source() == Value::Source::ImplicitValue);
        implicitArrayValue->makeExplicit();
        REQUIRE(implicitArrayValue->source() == Value::Source::ExplicitValue);

        // Explicit Value
        auto explicitArrayValue = Value::createArray(Value::Source::ExplicitValue);
        REQUIRE(explicitArrayValue->source() == Value::Source::ExplicitValue);
        explicitArrayValue->makeExplicit();
        REQUIRE(explicitArrayValue->source() == Value::Source::ExplicitValue); // should remain unchanged

        // Value
        auto simpleValue = Value::createInteger(31);
        REQUIRE(simpleValue->source() == Value::Source::Value);
        simpleValue->makeExplicit();
        REQUIRE(simpleValue->source() == Value::Source::Value); // should remain unchanged
    }

    void testArrayMethods() {
        // Create an array Value
        auto arrayValue = Value::createArray(Value::Source::ExplicitValue);

        // Check size of newly created array (should be 0)
        REQUIRE(arrayValue->size() == 0);

        // Try to access a value at index 0 (should return nullptr as the array is empty)
        REQUIRE(arrayValue->value(0) == nullptr);

        // Add a new value to the array
        arrayValue->addValue(Value::createInteger(42));
        // Size should be 1 now
        REQUIRE(arrayValue->size() == 1);
        // Value at index 0 should not be nullptr now
        REQUIRE(arrayValue->value(0) != nullptr);
        // Value at index 0 should be the integer we added
        REQUIRE(arrayValue->value(0)->type() == Value::Type::Integer);

        // Add another value
        arrayValue->addValue(Value::createFloat(42.0));
        // Size should be 2 now
        REQUIRE(arrayValue->size() == 2);
        // Value at index 1 should not be nullptr
        REQUIRE(arrayValue->value(1) != nullptr);
        // Value at index 1 should be the float we added
        REQUIRE(arrayValue->value(1)->type() == Value::Type::Float);

        // Accessing beyond the size of the array should still return nullptr
        REQUIRE(arrayValue->value(2) == nullptr);

        // For non-array values, size() should return 0, value() should return nullptr, and addValue() should be ignored
        auto nonArrayValue = Value::createInteger(42);
        REQUIRE(nonArrayValue->size() == 0);
        REQUIRE(nonArrayValue->value(0) == nullptr);
        nonArrayValue->addValue(Value::createInteger(31));  // this should be ignored
        REQUIRE(nonArrayValue->size() == 0);  // size should still be 0
        REQUIRE(nonArrayValue->value(0) == nullptr);  // value() should still return nullptr
    }

    // ...
};

