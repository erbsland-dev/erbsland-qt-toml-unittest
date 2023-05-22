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

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>


using namespace elqt::toml;


class ValueTest : public el::UnitTest {
public:
    ValuePtr value{};

    void testInteger() {
        value = Value::createInteger(42);
        REQUIRE(value->type() == Value::Type::Integer);
        REQUIRE(value->source() == Value::Source::Value);
        REQUIRE_FALSE(value->isTable());
        REQUIRE_FALSE(value->isArray());
        REQUIRE(value->locationRange().begin().isNotSet());
        REQUIRE(value->locationRange().end().isNotSet());
        REQUIRE(value->size() == 0);
        REQUIRE(value->hasValue("test") == false);
        REQUIRE(value->value(0) == nullptr);
        REQUIRE(value->value("test") == nullptr);
        REQUIRE(value->valueFromKey("test") == nullptr);
        value->makeExplicit();
        REQUIRE(value->source() == Value::Source::Value);
        REQUIRE(value->toInteger() == 42);
        REQUIRE(value->toString().isEmpty());
    }

    void testFloat() {
        value = Value::createFloat(42.0);
        REQUIRE(value->type() == Value::Type::Float);
        REQUIRE(value->source() == Value::Source::Value)
        REQUIRE(value->size() == 0);
        REQUIRE_FALSE(value->isTable());
        REQUIRE_FALSE(value->isArray());
        REQUIRE(value->locationRange().begin().isNotSet());
        REQUIRE(value->locationRange().end().isNotSet());
        REQUIRE(value->size() == 0);
        REQUIRE(value->hasValue("test") == false);
        REQUIRE(value->value(0) == nullptr);
        REQUIRE(value->value("test") == nullptr);
        REQUIRE(value->valueFromKey("test") == nullptr);
        value->makeExplicit();
        REQUIRE(value->source() == Value::Source::Value);
        REQUIRE(value->toFloat() == 42.0);
        REQUIRE(value->toString().isEmpty());
    }

    void testBool() {
        value = Value::createBoolean(true);
        REQUIRE(value->type() == Value::Type::Boolean);
        REQUIRE(value->source() == Value::Source::Value)
        REQUIRE(value->size() == 0);
        REQUIRE_FALSE(value->isTable());
        REQUIRE_FALSE(value->isArray());
        REQUIRE(value->locationRange().begin().isNotSet());
        REQUIRE(value->locationRange().end().isNotSet());
        REQUIRE(value->size() == 0);
        REQUIRE(value->hasValue("test") == false);
        REQUIRE(value->value(0) == nullptr);
        REQUIRE(value->value("test") == nullptr);
        REQUIRE(value->valueFromKey("test") == nullptr);
        value->makeExplicit();
        REQUIRE(value->source() == Value::Source::Value);
        REQUIRE(value->toBoolean() == true);
        REQUIRE(value->toFloat() == 0.0);
    }

    void testString() {
        value = Value::createString("test");
        REQUIRE(value->type() == Value::Type::String);
        REQUIRE(value->source() == Value::Source::Value)
        REQUIRE(value->size() == 0);
        REQUIRE_FALSE(value->isTable());
        REQUIRE_FALSE(value->isArray());
        REQUIRE(value->locationRange().begin().isNotSet());
        REQUIRE(value->locationRange().end().isNotSet());
        REQUIRE(value->size() == 0);
        REQUIRE(value->hasValue("test") == false);
        REQUIRE(value->value(0) == nullptr);
        REQUIRE(value->value("test") == nullptr);
        REQUIRE(value->valueFromKey("test") == nullptr);
        value->makeExplicit();
        REQUIRE(value->source() == Value::Source::Value);
        REQUIRE(value->toString() == "test");
        REQUIRE(value->toInteger() == 0);
    }

    void testTime() {
        value = Value::createTime(QTime{12, 5, 5});
        REQUIRE(value->type() == Value::Type::Time);
        REQUIRE(value->source() == Value::Source::Value)
        REQUIRE_FALSE(value->isTable());
        REQUIRE_FALSE(value->isArray());
        REQUIRE(value->locationRange().begin().isNotSet());
        REQUIRE(value->locationRange().end().isNotSet());
        REQUIRE(value->size() == 0);
        REQUIRE(value->hasValue("test") == false);
        REQUIRE(value->value(0) == nullptr);
        REQUIRE(value->value("test") == nullptr);
        REQUIRE(value->valueFromKey("test") == nullptr);
        value->makeExplicit();
        REQUIRE(value->source() == Value::Source::Value);
        REQUIRE(value->toTime() == QTime{12, 5, 5});
        REQUIRE(value->toBoolean() == false);
    }

    void testDate() {
        value = Value::createDate(QDate::currentDate());
        REQUIRE(value->type() == Value::Type::Date);
        REQUIRE(value->source() == Value::Source::Value)
        REQUIRE_FALSE(value->isTable());
        REQUIRE_FALSE(value->isArray());
        REQUIRE(value->locationRange().begin().isNotSet());
        REQUIRE(value->locationRange().end().isNotSet());
        REQUIRE(value->size() == 0);
        REQUIRE(value->hasValue("test") == false);
        REQUIRE(value->value(0) == nullptr);
        REQUIRE(value->value("test") == nullptr);
        REQUIRE(value->valueFromKey("test") == nullptr);
        value->makeExplicit();
        REQUIRE(value->source() == Value::Source::Value);
    }

    void testDateTime() {
        value = Value::createDateTime(QDateTime::currentDateTime());
        REQUIRE(value->type() == Value::Type::DateTime);
        REQUIRE(value->source() == Value::Source::Value)
        REQUIRE_FALSE(value->isTable());
        REQUIRE_FALSE(value->isArray());
        REQUIRE(value->locationRange().begin().isNotSet());
        REQUIRE(value->locationRange().end().isNotSet());
        REQUIRE(value->size() == 0);
        REQUIRE(value->hasValue("test") == false);
        REQUIRE(value->value(0) == nullptr);
        REQUIRE(value->value("test") == nullptr);
        REQUIRE(value->valueFromKey("test") == nullptr);
        value->makeExplicit();
        REQUIRE(value->source() == Value::Source::Value);
    }

    void testTable() {
        value = Value::createTable(Value::Source::ExplicitTable);
        REQUIRE(value->type() == Value::Type::Table);
        REQUIRE(value->source() == Value::Source::ExplicitTable)
        REQUIRE(value->isTable());
        REQUIRE_FALSE(value->isArray());
        REQUIRE(value->locationRange().begin().isNotSet());
        REQUIRE(value->locationRange().end().isNotSet());
        REQUIRE(value->size() == 0);
        REQUIRE(value->hasValue("test") == false);
        REQUIRE(value->value(0) == nullptr);
        REQUIRE(value->value("test") == nullptr);
        REQUIRE(value->valueFromKey("test") == nullptr);
    }

    void testArray() {
        value = Value::createArray(Value::Source::ExplicitValue);
        REQUIRE(value->type() == Value::Type::Array);
        REQUIRE(value->source() == Value::Source::ExplicitValue)
        REQUIRE_FALSE(value->isTable());
        REQUIRE(value->isArray());
        REQUIRE(value->locationRange().begin().isNotSet());
        REQUIRE(value->locationRange().end().isNotSet());
        REQUIRE(value->size() == 0);
        REQUIRE(value->hasValue("test") == false);
        REQUIRE(value->value(0) == nullptr);
        REQUIRE(value->value("test") == nullptr);
        REQUIRE(value->valueFromKey("test") == nullptr);
    }

    void testSourceAndMakeExplicit() {
        // Implicit Table
        value = Value::createTable(Value::Source::ImplicitTable);
        REQUIRE(value->source() == Value::Source::ImplicitTable);
        value->makeExplicit();
        REQUIRE(value->source() == Value::Source::ExplicitTable);

        // Explicit Table
        value = Value::createTable(Value::Source::ExplicitTable);
        REQUIRE(value->source() == Value::Source::ExplicitTable);
        value->makeExplicit();
        REQUIRE(value->source() == Value::Source::ExplicitTable); // should remain unchanged

        // Implicit Value
        value = Value::createArray(Value::Source::ImplicitValue);
        REQUIRE(value->source() == Value::Source::ImplicitValue);
        value->makeExplicit();
        REQUIRE(value->source() == Value::Source::ExplicitValue);

        // Explicit Value
        value = Value::createArray(Value::Source::ExplicitValue);
        REQUIRE(value->source() == Value::Source::ExplicitValue);
        value->makeExplicit();
        REQUIRE(value->source() == Value::Source::ExplicitValue); // should remain unchanged

        // Value
        value = Value::createInteger(31);
        REQUIRE(value->source() == Value::Source::Value);
        value->makeExplicit();
        REQUIRE(value->source() == Value::Source::Value); // should remain unchanged
    }

    void testArrayMethods() {
        value = Value::createArray(Value::Source::ExplicitValue);

        // Check size of newly created array (should be 0)
        REQUIRE(value->size() == 0);

        // Try to access a value at index 0 (should return nullptr as the array is empty)
        REQUIRE(value->value(0) == nullptr);

        // Add a new value to the array
        value->addValue(Value::createInteger(42));
        // Size should be 1 now
        REQUIRE(value->size() == 1);
        // Value at index 0 should not be nullptr now
        REQUIRE(value->value(0) != nullptr);
        // Value at index 0 should be the integer we added
        REQUIRE(value->value(0)->type() == Value::Type::Integer);

        // Add another value
        value->addValue(Value::createFloat(42.0));
        // Size should be 2 now
        REQUIRE(value->size() == 2);
        // Value at index 1 should not be nullptr
        REQUIRE(value->value(1) != nullptr);
        // Value at index 1 should be the float we added
        REQUIRE(value->value(1)->type() == Value::Type::Float);

        // Accessing beyond the size of the array should still return nullptr
        REQUIRE(value->value(2) == nullptr);

        // For non-array values, size() should return 0, value() should return nullptr, and addValue() should be ignored
        value = Value::createInteger(42);
        REQUIRE(value->size() == 0);
        REQUIRE(value->value(0) == nullptr);
        value->addValue(Value::createInteger(31));  // this should be ignored
        REQUIRE(value->size() == 0);  // size should still be 0
        REQUIRE(value->value(0) == nullptr);  // value() should still return nullptr
    }

    void testLocationRange() {
        value = Value::createString(QStringLiteral("Test"));
        REQUIRE(value->locationRange().begin().isNotSet());
        REQUIRE(value->locationRange().end().isNotSet());
        value->setLocationRange(LocationRange{Location{10, 2, 3}, Location{16, 3, 5}});
        REQUIRE_FALSE(value->locationRange().begin().isNotSet());
        REQUIRE_FALSE(value->locationRange().end().isNotSet());
        REQUIRE(value->locationRange() == LocationRange{Location{10, 2, 3}, Location{16, 3, 5}});
        value->setLocationRange(LocationRange::createNotSet());
        REQUIRE(value->locationRange().begin().isNotSet());
        REQUIRE(value->locationRange().end().isNotSet());
    }

    void testAddToValue() {
        value = Value::createInteger(1);
        value->setValue("test", Value::createInteger(1));
        value->addValue(Value::createInteger(1));
        REQUIRE(value->size() == 0);
        value = Value::createArray(Value::Source::ExplicitTable);
        value->setValue("test", Value::createInteger(1));
        REQUIRE(value->size() == 0);
        value = Value::createTable(Value::Source::ExplicitTable);
        value->addValue(Value::createInteger(1));
        REQUIRE(value->size() == 0);
        value = Value::createArray(Value::Source::ExplicitTable);
        value->addValue(Value::createInteger(1)); // must be table.
        REQUIRE(value->size() == 0);
    }

    auto createValueStructure() -> ValuePtr {
        auto tableValue = Value::createTable(Value::Source::ExplicitTable);
        tableValue->setValue("a", Value::createString("a"));
        tableValue->setValue("b", Value::createString("b"));
        tableValue->setValue("c", Value::createString("c"));
        tableValue->setValue("int50", Value::createInteger(50));
        auto subTable = Value::createTable(Value::Source::ExplicitTable);
        subTable->setValue("d", Value::createString("d"));
        subTable->setValue("e", Value::createString("e"));
        subTable->setValue("f", Value::createString("f"));
        subTable->setValue("int60", Value::createInteger(60));
        subTable->setValue("bool", Value::createBoolean(true));
        subTable->setValue("float", Value::createFloat(1.0));
        subTable->setValue("time", Value::createTime(QTime{8, 10, 12}));
        subTable->setValue("date", Value::createDate(QDate{2023, 5, 22}));
        subTable->setValue("datetime", Value::createDateTime(QDateTime{QDate{2023, 5, 21}, QTime{9, 11, 17}}));
        auto arrayValue = Value::createArray(Value::Source::Value);
        arrayValue->addValue(Value::createString("a"));
        arrayValue->addValue(Value::createString("b"));
        arrayValue->addValue(Value::createString("c"));
        subTable->setValue("array", arrayValue);
        tableValue->setValue("sub", subTable);
        auto subSubTable = Value::createTable(Value::Source::ExplicitTable);
        subSubTable->setValue("g", Value::createString("g"));
        subSubTable->setValue("h", Value::createString("h"));
        subSubTable->setValue("i", Value::createString("i"));
        subSubTable->setValue("int70", Value::createInteger(70));
        subTable->setValue("sub", subSubTable);
        return tableValue;
    }

    void testTableBasicValueMethods() {
        value = createValueStructure();
        REQUIRE(value->size() == 5);
        REQUIRE(value->hasValue("a"));
        REQUIRE(value->hasValue("b"));
        REQUIRE(value->hasValue("c"));
        REQUIRE(value->hasValue("int50"));
        REQUIRE(value->hasValue("sub"));
        REQUIRE(value->hasKey("a"));
        REQUIRE(value->hasKey("b"));
        REQUIRE(value->hasKey("c"));
        REQUIRE(value->hasKey("int50"));
        REQUIRE(value->hasKey("sub"));
        REQUIRE(value->value("a")->type() == Value::Type::String);
        REQUIRE(value->value("b")->type() == Value::Type::String);
        REQUIRE(value->value("c")->type() == Value::Type::String);
        REQUIRE(value->value("int50")->type() == Value::Type::Integer);
        REQUIRE(value->value("sub")->type() == Value::Type::Table);
        REQUIRE_FALSE(value->hasValue("sub.a"));
        REQUIRE_FALSE(value->hasValue("sub.b"));
        REQUIRE_FALSE(value->hasValue("sub.c"));
        REQUIRE_FALSE(value->hasValue("sub.int50"));
        REQUIRE(value->value("sub.a") == nullptr);
        REQUIRE(value->valueFromKey("sub.a") == nullptr);
        REQUIRE(value->hasValue("sub.d"));
        REQUIRE(value->hasValue("sub.e"));
        REQUIRE(value->hasValue("sub.f"));
        REQUIRE(value->hasValue("sub.int60"));
        REQUIRE(value->hasValue("sub.sub"));
        REQUIRE_FALSE(value->hasKey("sub.d"));
        REQUIRE_FALSE(value->hasKey("sub.e"));
        REQUIRE_FALSE(value->hasKey("sub.f"));
        REQUIRE_FALSE(value->hasKey("sub.int60"));
        REQUIRE_FALSE(value->hasKey("sub.sub"));
        REQUIRE(value->value("sub.d")->type() == Value::Type::String);
        REQUIRE(value->value("sub.e")->type() == Value::Type::String);
        REQUIRE(value->value("sub.f")->type() == Value::Type::String);
        REQUIRE(value->value("sub.int60")->type() == Value::Type::Integer);
        REQUIRE(value->value("sub.sub")->type() == Value::Type::Table);
        REQUIRE_FALSE(value->hasValue("sub.sub.a"));
        REQUIRE_FALSE(value->hasValue("sub.sub.b"));
        REQUIRE_FALSE(value->hasValue("sub.sub.c"));
        REQUIRE_FALSE(value->hasValue("sub.sub.int50"));
        REQUIRE_FALSE(value->hasValue("sub.sub.d"));
        REQUIRE_FALSE(value->hasValue("sub.sub.e"));
        REQUIRE_FALSE(value->hasValue("sub.sub.f"));
        REQUIRE_FALSE(value->hasValue("sub.sub.int60"));
        REQUIRE_FALSE(value->hasValue("sub.sub.sub"));
        REQUIRE(value->hasValue("sub.sub.g"));
        REQUIRE(value->hasValue("sub.sub.h"));
        REQUIRE(value->hasValue("sub.sub.i"));
        REQUIRE(value->hasValue("sub.sub.int70"));
        REQUIRE(value->value("sub.sub.g")->type() == Value::Type::String);
        REQUIRE(value->value("sub.sub.h")->type() == Value::Type::String);
        REQUIRE(value->value("sub.sub.i")->type() == Value::Type::String);
        REQUIRE(value->value("sub.sub.int70")->type() == Value::Type::Integer);
        REQUIRE_FALSE(value->hasValue("a.sub"));
        REQUIRE(value->value("a.sub") == nullptr);
    }

    void testTableKeys() {
        value = createValueStructure();
        auto actualTableKeys = value->tableKeys();
        auto expectedTableKeys = QStringList() << "a" << "b" << "c" << "int50" << "sub";
        actualTableKeys.sort();
        expectedTableKeys.sort();
        REQUIRE(actualTableKeys == expectedTableKeys);
        value = Value::createInteger(1);
        REQUIRE(value->tableKeys().isEmpty());
    }

    void testTableConvenienceAccess() {
        value = createValueStructure();
        REQUIRE(value->stringValue("somewhere").isEmpty());
        REQUIRE(value->stringValue("int50").isEmpty());
        REQUIRE(value->stringValue("sub").isEmpty());
        REQUIRE(value->stringValue("a") == "a");
        REQUIRE(value->stringValue("sub.d") == "d");
        REQUIRE(value->stringValue("sub.int60").isEmpty());
        REQUIRE(value->stringValue("sub.somewhere").isEmpty());
        REQUIRE(value->stringValue("somewhere", "alt") == "alt");

        REQUIRE(value->integerValue("somewhere") == 0);
        REQUIRE(value->integerValue("int50") == 50);
        REQUIRE(value->integerValue("sub") == 0);
        REQUIRE(value->integerValue("a") == 0);
        REQUIRE(value->integerValue("sub.d") == 0);
        REQUIRE(value->integerValue("sub.int60") == 60);
        REQUIRE(value->integerValue("sub.somewhere") == 0);
        REQUIRE(value->integerValue("sub.d", 900) == 900);

        REQUIRE(value->floatValue("somewhere") == 0.0);
        REQUIRE(value->floatValue("int50") == 0.0);
        REQUIRE(value->floatValue("sub") == 0.0);
        REQUIRE(value->floatValue("a") == 0.0);
        REQUIRE(value->floatValue("sub.float") == 1.0);
        REQUIRE(value->floatValue("sub.int60") == 0.0);
        REQUIRE(value->floatValue("sub.somewhere") == 0.0);
        REQUIRE(value->floatValue("sub.d", 23.8) == 23.8);

        REQUIRE(value->booleanValue("somewhere") == false);
        REQUIRE(value->booleanValue("int50") == false);
        REQUIRE(value->booleanValue("sub") == false);
        REQUIRE(value->booleanValue("a") == false);
        REQUIRE(value->booleanValue("sub.bool") == true);
        REQUIRE(value->booleanValue("sub.int60") == false);
        REQUIRE(value->booleanValue("sub.somewhere") == false);
        REQUIRE(value->booleanValue("sub.d", true) == true);

        REQUIRE(value->timeValue("somewhere") == QTime{});
        REQUIRE(value->timeValue("int50") == QTime{});
        REQUIRE(value->timeValue("sub") == QTime{});
        REQUIRE(value->timeValue("a") == QTime{});
        REQUIRE(value->timeValue("sub.time") == QTime{8, 10, 12});
        REQUIRE(value->timeValue("sub.int60") == QTime{});
        REQUIRE(value->timeValue("sub.somewhere") == QTime{});
        REQUIRE(value->timeValue("sub.d", QTime{10, 10, 10}) == QTime{10, 10, 10});

        REQUIRE(value->dateValue("somewhere") == QDate{});
        REQUIRE(value->dateValue("int50") == QDate{});
        REQUIRE(value->dateValue("sub") == QDate{});
        REQUIRE(value->dateValue("a") == QDate{});
        REQUIRE(value->dateValue("sub.date") == QDate{2023, 5, 22});
        REQUIRE(value->dateValue("sub.int60") == QDate{});
        REQUIRE(value->dateValue("sub.somewhere") == QDate{});
        REQUIRE(value->dateValue("sub.d", QDate{2023, 5, 2}) == QDate{2023, 5, 2});

        REQUIRE(value->dateTimeValue("somewhere") == QDateTime{});
        REQUIRE(value->dateTimeValue("int50") == QDateTime{});
        REQUIRE(value->dateTimeValue("sub") == QDateTime{});
        REQUIRE(value->dateTimeValue("a") == QDateTime{});
        REQUIRE(value->dateTimeValue("sub.datetime") == QDateTime{QDate{2023, 5, 21}, QTime{9, 11, 17}});
        REQUIRE(value->dateTimeValue("sub.int60") == QDateTime{});
        REQUIRE(value->dateTimeValue("sub.somewhere") == QDateTime{});
        REQUIRE(value->dateTimeValue("sub.d", QDateTime{QDate{2023, 5, 4}, QTime{9, 11, 2}}) == QDateTime{QDate{2023, 5, 4}, QTime{9, 11, 2}});

        REQUIRE(value->tableValue("somewhere")->size() == 0);
        REQUIRE(value->tableValue("int50")->size() == 0);
        REQUIRE(value->tableValue("sub")->size() == 11);
        REQUIRE(value->tableValue("a")->size() == 0);
        REQUIRE(value->tableValue("sub.d")->size() == 0);
        REQUIRE(value->tableValue("sub.int60")->size() == 0);
        REQUIRE(value->tableValue("sub.somewhere")->size() == 0);
        REQUIRE(value->tableValue("somewhere")->type() == Value::Type::Table);
        REQUIRE(value->tableValue("int50")->type() == Value::Type::Table);
        REQUIRE(value->tableValue("sub")->type() == Value::Type::Table);
        REQUIRE(value->tableValue("a")->type() == Value::Type::Table);
        REQUIRE(value->tableValue("sub.d")->type() == Value::Type::Table);
        REQUIRE(value->tableValue("sub.int60")->type() == Value::Type::Table);
        REQUIRE(value->tableValue("sub.somewhere")->type() == Value::Type::Table);

        REQUIRE(value->arrayValue("somewhere")->size() == 0);
        REQUIRE(value->arrayValue("int50")->size() == 0);
        REQUIRE(value->arrayValue("sub")->size() == 0);
        REQUIRE(value->arrayValue("a")->size() == 0);
        REQUIRE(value->arrayValue("sub.array")->size() == 3);
        REQUIRE(value->arrayValue("sub.int60")->size() == 0);
        REQUIRE(value->arrayValue("sub.somewhere")->size() == 0);
        REQUIRE(value->arrayValue("somewhere")->type() == Value::Type::Array);
        REQUIRE(value->arrayValue("int50")->type() == Value::Type::Array);
        REQUIRE(value->arrayValue("sub")->type() == Value::Type::Array);
        REQUIRE(value->arrayValue("a")->type() == Value::Type::Array);
        REQUIRE(value->arrayValue("sub.array")->type() == Value::Type::Array);
        REQUIRE(value->arrayValue("sub.int60")->type() == Value::Type::Array);
        REQUIRE(value->arrayValue("sub.somewhere")->type() == Value::Type::Array);
    }

    static auto flattenValueStructure(const ValuePtr &v) -> std::vector<ValuePtr> {
        std::vector<ValuePtr> result;
        if (v->isArray()) {
            for (const auto &sv : v->toArray()) {
                auto fv = flattenValueStructure(sv);
                result.insert(result.end(), fv.begin(), fv.end());
            }
        } else if (v->isTable()) {
            auto keys = v->tableKeys();
            keys.sort();
            for (const auto &key : keys) {
                auto fv = flattenValueStructure(v->valueFromKey(key));
                result.insert(result.end(), fv.begin(), fv.end());
            }
        } else {
            result.push_back(v);
        }
        return result;
    }

    void testClone() {
        value = createValueStructure();
        auto clonedValue = value->clone();
        auto vla = flattenValueStructure(value);
        auto vlb = flattenValueStructure(clonedValue);
        REQUIRE(vla.size() == vlb.size());
        for (const auto &a : vla) {
            auto containsPointer = std::any_of(vlb.begin(), vlb.end(), [a](const ValuePtr &b) -> bool {
                return a.get() == b.get(); // check if both point to the same object.
            });
            REQUIRE_FALSE(containsPointer);
        }
        for (std::size_t i = 0; i < vla.size(); ++i) {
            REQUIRE(vla[i]->type() == vlb[i]->type());
            REQUIRE(vla[i]->size() == vlb[i]->size());
            REQUIRE(vla[i]->locationRange() == vlb[i]->locationRange());
            REQUIRE(vla[i]->toVariant() == vlb[i]->toVariant());
        }
    }

    void testJson() {
        value = createValueStructure();
        auto jsonCompactText = R"({"a":"a","b":"b","c":"c","int50":50,"sub":{"array":["a","b","c"],)"
                               R"("bool":true,"d":"d","date":"2023-05-22","datetime":"2023-05-21T09:11:17.000",)"
                               R"("e":"e","f":"f","float":1,"int60":60,"sub":{"g":"g","h":"h","i":"i","int70":70},)"
                               R"("time":"08:10:12.000"}})";
        auto jsonA = QJsonDocument{value->toJson().toObject()};
        auto jsonB = QJsonDocument::fromJson(jsonCompactText);
        REQUIRE(jsonA == jsonB);
    }

    void testVariant() {
        value = Value::createInteger(90);
        REQUIRE(value->toVariant() == 90);
        value = Value::createFloat(100.9);
        REQUIRE(value->toVariant() == 100.9);
        value = Value::createBoolean(true);
        REQUIRE(value->toVariant() == true);
        value = Value::createString("test");
        REQUIRE(value->toVariant() == "test");
        value = Value::createTime(QTime{10, 11, 12});
        REQUIRE(value->toVariant() == QTime{10, 11, 12});
        value = Value::createDate(QDate{2023, 1, 2});
        REQUIRE(value->toVariant() == QDate{2023, 1, 2});
        value = Value::createDateTime(QDateTime{QDate{2023, 1, 2}, QTime{10, 11, 12}});
        REQUIRE(value->toVariant() == QDateTime{QDate{2023, 1, 2}, QTime{10, 11, 12}});
        value = createValueStructure();
        auto m = value->toVariant().toMap();
        REQUIRE(m.size() == 5);
        auto a = value->value("sub.array")->toVariant().toList();
        REQUIRE(a.size() == 3);
    }

    void testIterator() {
        value = Value::createArray(Value::Source::Value);
        for (std::size_t i = 0; i < 10; ++i) {
            value->addValue(Value::createInteger(i));
        }
        std::size_t i = 0;
        for (const auto &v : *value) {
            REQUIRE(v->type() == Value::Type::Integer);
            REQUIRE(v->toInteger() == i);
            i += 1;
        }
        i = 0;
        for (auto it = value->begin(); it != value->end(); ++it) {
            REQUIRE(it->type() == Value::Type::Integer);
            REQUIRE(it->toInteger() == i);
            i += 1;
        }
        i = 0;
        for (auto it = value->begin(); it != value->end(); it++) {
            REQUIRE(it->type() == Value::Type::Integer);
            REQUIRE(it->toInteger() == i);
            i += 1;
        }
        value = Value::createInteger(10);
        REQUIRE(value->begin() == ValueIterator{});
        REQUIRE(value->end() == ValueIterator{});
        auto it = ValueIterator{};
        REQUIRE(it == ValueIterator{});
        REQUIRE(*it == nullptr);
        REQUIRE(it.operator->() == nullptr);
        ++it;
        REQUIRE(it == ValueIterator{});
        it++;
        REQUIRE(it == ValueIterator{});
    }

    void testValueSourceToString() {
        REQUIRE(valueSourceToString(ValueSource::ImplicitTable) == "Implicit Table");
        REQUIRE(valueSourceToString(ValueSource::ExplicitTable) == "Explicit Table");
        REQUIRE(valueSourceToString(ValueSource::ImplicitValue) == "Implicit Value");
        REQUIRE(valueSourceToString(ValueSource::ExplicitValue) == "Explicit Value");
        REQUIRE(valueSourceToString(ValueSource::Value) == "Value");
    }
};

