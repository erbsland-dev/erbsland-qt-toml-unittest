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

#include <erbsland/qt/toml/Parser.hpp>
#include <erbsland/qt/toml/Error.hpp>
#include <erbsland/qt/toml/Specification.hpp>

#include <QtCore/QString>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include <vector>
#include <map>
#include <sstream>
#include <filesystem>


using namespace elqt::toml;
using namespace elqt::toml::impl;
namespace fs = std::filesystem;

#include <QtCore/QTextStream>


class ParserTest : public el::UnitTest {
public:
    /// The map with exceptions from the file "versions.go" in toml-test
    ///
    std::map<Specification, std::vector<std::string>> _excludedFiles = {
        {
            Specification::Version_1_0,
            {
                "valid/string/escape-esc",
                "valid/string/hex-escape",
                "invalid/string/bad-hex-esc",
                "valid/datetime/no-seconds",
                "valid/inline-table/newline",
                "valid/key/unicode",
            }
        },
        {
            Specification::Version_1_1,
            {
                "invalid/datetime/no-secs",
                "invalid/string/basic-byte-escapes",
                "invalid/inline-table/trailing-comma",
                "invalid/inline-table/linebreak-1",
                "invalid/inline-table/linebreak-2",
                "invalid/inline-table/linebreak-3",
                "invalid/inline-table/linebreak-4",
                "invalid/key/special-character",
            }
        }
    };

    /// A list of specifications to test.
    ///
    std::vector<Specification> _testedSpecifications = {
        Specification::Version_1_0,
        Specification::Version_1_1
    };

    /// Store the tested specification.
    ///
    Specification _testedSpecification{Specification::Version_1_0};

    fs::path _testPath; ///< The path of the current test.
    QStringList _keyPath; ///< The key path inside of the document that is tested.
    QString _actual; ///< The actual value.
    QString _expected; ///< The expected value.
    QJsonDocument _actualJson; ///< The actual JSON document.
    QJsonDocument _expectedJson; ///< The expected JSON document.

    auto additionalErrorMessages() -> std::string override {
        std::ostringstream str;
        str     << "Specification ...: ";
        switch (_testedSpecification) {
        case Specification::Version_1_0:
            str << "TOML 1.0\n";
            break;
        case Specification::Version_1_1:
            str << "TOML 1.1\n";
            break;
        }
        str     << "Test Path .......: " << _testPath << "\n";
        if (!_keyPath.isEmpty()) {
            str << "Key Path ........: " << _keyPath.join('/').toStdString() << "\n";
        }
        if (!_expected.isEmpty()) {
            str << "Expected ........: " << _expected.toStdString() << "\n";
        }
        if (!_actual.isEmpty()) {
            str << "Actual ..........: " << _actual.toStdString() << "\n";
        }
        if (!_actualJson.isNull()) {
            str << "--- Actual JSON ---\n" << QString::fromUtf8(_actualJson.toJson(QJsonDocument::Indented)).toStdString() << "\n";
        }
        if (!_expectedJson.isNull()) {
            str << "--- Expected JSON ---\n" << QString::fromUtf8(_expectedJson.toJson(QJsonDocument::Indented)).toStdString() << "\n";
        }
        return str.str();
    }

    /// Get a list of test files.
    ///
    auto getTestFiles(const std::string &name, Specification specification) -> std::vector<fs::path> {
        auto commonPath = unitTestExecutablePath().parent_path() / "data" / "parser";
        auto testFilesPath = commonPath / name;
        auto exclusions = _excludedFiles[specification];

        auto result = std::vector<fs::path>{};
        for (const auto &entry : fs::recursive_directory_iterator(testFilesPath)) {
            if (!entry.is_regular_file() || entry.path().extension() != ".toml") {
                continue;
            }
            auto relativePath = entry.path().lexically_relative(commonPath).generic_string();
            auto relativePathWithoutExtension = relativePath.substr(0, relativePath.size() - 5); // remove ".toml"
            if (std::find(exclusions.begin(), exclusions.end(), relativePathWithoutExtension) == exclusions.end()) {
                result.push_back(entry.path());
            }
        }

        return result;
    }

    static auto convertTypeToString(QJsonValue::Type type) -> QString {
        QString typeString;
        switch(type) {
        case QJsonValue::Null:
            typeString = "Null";
            break;
        case QJsonValue::Bool:
            typeString = "Bool";
            break;
        case QJsonValue::Double:
            typeString = "Double";
            break;
        case QJsonValue::String:
            typeString = "String";
            break;
        case QJsonValue::Array:
            typeString = "Array";
            break;
        case QJsonValue::Object:
            typeString = "Object";
            break;
        default:
            typeString = "Undefined";
            break;
        }
        return typeString;
    }

    void verifyAllValuesHaveLocation(const ValuePtr &value) {
        REQUIRE_FALSE(value->locationRange().begin().isNotSet());
        REQUIRE_FALSE(value->locationRange().end().isNotSet());
        if (value->isTable()) {
            for (const auto &[key, tableValue] : value->toTable()) {
                verifyAllValuesHaveLocation(tableValue);
            }
        } else if (value->isArray()) {
            for (const auto &arrayValue : value->toArray()) {
                verifyAllValuesHaveLocation(arrayValue);
            }
        }
    }

    /// Verify the given TOML file.
    ///
    /// For valid files, also expect a JSON file with the same name and `.json` suffix.
    ///
    /// @param tomlPath The path of the TOML file.
    /// @param isValid `true` if this file should be valid TOML.
    ///
    void verifyFile(const fs::path &tomlPath, Specification specification) {
        Parser parser{specification};
        auto value = parser.parseFileOrThrow(QString::fromStdString(tomlPath.string()));
        REQUIRE(value->isTable());
        WITH_CONTEXT(verifyAllValuesHaveLocation(value));
        // read the corresponding JSON file.
        auto jsonPath = tomlPath;
        jsonPath.replace_extension(fs::path(".json"));
        QFile jsonFile(QString::fromStdString(jsonPath.string()));
        if (!jsonFile.open(QIODevice::ReadOnly)) {
            throw std::logic_error(QStringLiteral("Can't open JSON file: %1").arg(jsonFile.fileName()).toStdString());
        }
        _expectedJson = QJsonDocument::fromJson(jsonFile.readAll());
        _actualJson.setObject(value->toUnitTestJson().toObject());
        _keyPath.clear();
        _keyPath.append(QStringLiteral("[root]"));
        verifyJsonObject(_actualJson.object(), _expectedJson.object());
    }

    void verifyJsonObject(const QJsonObject &actual, const QJsonObject &expected) {
        _actual = QStringLiteral("size = %1").arg(actual.size());
        _expected = QStringLiteral("size = %1").arg(expected.size());
        REQUIRE(actual.size() == expected.size());
        auto keys = expected.keys();
        keys.sort(); // sort the keys to have a predicted behaviour of the test.
        for (const auto &key : keys) {
            _keyPath.append(key);
            _actual = QStringLiteral("key = MISSING!");
            _expected = QStringLiteral("key = %1").arg(key);
            REQUIRE(actual.contains(key));
            auto actualValue = actual.value(key);
            auto expectedValue = expected.value(key);
            verifyValue(actualValue, expectedValue);
            _keyPath.removeLast();
        }
        // If this object is a value, also compare the strings.
        if (expected.size() == 2 && expected.contains(QStringLiteral("type")) && expected.contains(QStringLiteral("value"))) {
            auto expectedType = expected[QStringLiteral("type")].toString();
            auto actualType = actual[QStringLiteral("type")].toString();
            _actual = QStringLiteral("value type = %1").arg(actualType);
            _expected = QStringLiteral("value type = %1").arg(expectedType);
            REQUIRE(actualType == expectedType);
            auto expectedValue = expected[QStringLiteral("value")].toString();
            auto actualValue = actual[QStringLiteral("value")].toString();
            _actual = QStringLiteral("%1 value text = %2").arg(actualType, actualValue);
            _expected = QStringLiteral("%1 value text = %2").arg(expectedType, expectedValue);
            if (expectedType == QStringLiteral("string")) {
                REQUIRE(actualValue == expectedValue); // must match 1:1
            } else if (expectedType == QStringLiteral("integer")) {
                auto actualParsed = actualValue.toLongLong();
                auto expectedParsed = expectedValue.toLongLong();
                _actual.append(QStringLiteral(" (%1)").arg(actualParsed));
                _expected.append(QStringLiteral(" (%1)").arg(expectedParsed));
                REQUIRE(actualParsed == expectedParsed);
            } else if (expectedType == QStringLiteral("float")) {
                auto actualParsed = actualValue.toDouble();
                auto expectedParsed = expectedValue.toDouble();
                _actual.append(QStringLiteral(" (%1)").arg(actualParsed));
                _expected.append(QStringLiteral(" (%1)").arg(expectedParsed));
                if (std::isnan(expectedParsed)) {
                    REQUIRE(std::isnan(actualParsed));
                } else if (std::isinf(expectedParsed)) {
                    REQUIRE(std::isinf(actualParsed) && actualParsed == expectedParsed);
                } else {
                    REQUIRE(actualParsed == expectedParsed);
                }
            } else if (expectedType == QStringLiteral("bool")) {
                REQUIRE(actualValue == expectedValue); // must match 1:1
            } else if (expectedType == QStringLiteral("datetime")) {
                auto actualParsed = QDateTime::fromString(actualValue, Qt::ISODateWithMs);
                auto expectedParsed = QDateTime::fromString(expectedValue, Qt::ISODateWithMs);
                _actual.append(QStringLiteral(" (%1)").arg(actualParsed.toString(Qt::ISODateWithMs)));
                _expected.append(QStringLiteral(" (%1)").arg(expectedParsed.toString(Qt::ISODateWithMs)));
                REQUIRE(std::abs(actualParsed.msecsTo(expectedParsed)) <= 1); // ignore rounding errors
            } else if (expectedType == QStringLiteral("datetime-local")) {
                auto actualParsed = QDateTime::fromString(actualValue, Qt::ISODateWithMs);
                auto expectedParsed = QDateTime::fromString(expectedValue, Qt::ISODateWithMs);
                _actual.append(QStringLiteral(" (%1)").arg(actualParsed.toString(Qt::ISODateWithMs)));
                _expected.append(QStringLiteral(" (%1)").arg(expectedParsed.toString(Qt::ISODateWithMs)));
                REQUIRE(std::abs(actualParsed.msecsTo(expectedParsed)) <= 1);  // ignore rounding errors
            } else if (expectedType == QStringLiteral("date-local")) {
                auto actualParsed = QDate::fromString(actualValue, Qt::ISODate);
                auto expectedParsed = QDate::fromString(expectedValue, Qt::ISODate);
                _actual.append(QStringLiteral(" (%1)").arg(actualParsed.toString(Qt::ISODate)));
                _expected.append(QStringLiteral(" (%1)").arg(expectedParsed.toString(Qt::ISODate)));
                REQUIRE(actualParsed == expectedParsed);
            } else if (expectedType == QStringLiteral("time-local")) {
                auto actualParsed = QTime::fromString(actualValue, Qt::ISODateWithMs);
                auto expectedParsed = QTime::fromString(expectedValue, Qt::ISODateWithMs);
                _actual.append(QStringLiteral(" (%1)").arg(actualParsed.toString(Qt::ISODateWithMs)));
                _expected.append(QStringLiteral(" (%1)").arg(expectedParsed.toString(Qt::ISODateWithMs)));
                REQUIRE(std::abs(actualParsed.msecsTo(expectedParsed)) <= 1); // ignore rounding errors
            } else {
                throw std::logic_error("Unexpected type in test JSON.");
            }
        }
    }

    void verifyJsonArray(const QJsonArray &actual, const QJsonArray &expected) {
        _actual = QStringLiteral("size = %1").arg(actual.size());
        _expected = QStringLiteral("size = %1").arg(expected.size());
        REQUIRE(actual.size() == expected.size());
        for (int i = 0; i < expected.size(); ++i) {
            _keyPath.append(QStringLiteral("[%1]").arg(i));
            auto actualValue = actual[i];
            auto expectedValue = expected[i];
            verifyValue(actualValue, expectedValue);
            _keyPath.removeLast();
        }
    }

    void verifyValue(const QJsonValue &actualValue, const QJsonValue &expectedValue) {
        _actual = QStringLiteral("type = %1").arg(convertTypeToString(actualValue.type()));
        _expected = QStringLiteral("type = %1").arg(convertTypeToString(expectedValue.type()));
        REQUIRE(actualValue.type() == expectedValue.type());
        if (expectedValue.isObject()) {
            verifyJsonObject(actualValue.toObject(), expectedValue.toObject());
        } else if (expectedValue.isArray()) {
            verifyJsonArray(actualValue.toArray(), expectedValue.toArray());
        } else if (!expectedValue.isString()) {
            throw std::logic_error("Unexpected value type in test JSON file.");
        }
    }

    void testValidFiles() {
        for (auto specification : _testedSpecifications) {
            _testedSpecification = specification;
            auto testFiles = getTestFiles("valid", specification);
            REQUIRE(testFiles.size() > 100); // just make sure, the test files are accessible
            for (const auto &path : testFiles) {
                _testPath = path;
                WITH_CONTEXT(verifyFile(path, specification));
            }
        }
    }

    void testInvalidFiles() {
        _keyPath.clear();
        _actual.clear();
        _expected.clear();
        _actualJson = {};
        _expectedJson = {};
        for (auto specification : _testedSpecifications) {
            _testedSpecification = specification;
            auto testFiles = getTestFiles("invalid", specification);
            REQUIRE(testFiles.size() > 80); // just make sure, the test files are accessible
            for (const auto &path: testFiles) {
                _testPath = path;
                Parser parser{specification};
                REQUIRE_THROWS_AS(elqt::toml::Error, parser.parseFileOrThrow(QString::fromStdString(path.string())));
            }
        }
    }
};

