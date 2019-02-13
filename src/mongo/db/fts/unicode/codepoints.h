/**
 *    Copyright (C) 2018-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#pragma once

#include <string>

namespace mongo {
namespace unicode {

/**
 * There are currently two options supported for the delimiter list. The only difference between the
 * English and NotEnglish modes is that in English, the apostrophe is considered a delimiter, while
 * in NotEnglish, it is not.
 */
enum class DelimiterListLanguage {
    kEnglish,
    kNotEnglish,
};

/**
 * There are currently two Case folding modes supported. The only difference between the Normal mode
 * and the Turkish mode is that in the Turkish mode, the letter I is lowercased to ı, and the letter
 * İ is lowercased to i. In the normal mode, the letter I is lowercased to i, and there is no
 * mapping for İ.
 */
enum class CaseFoldMode {
    kNormal,
    kTurkish,
};

/**
 * Returns whether or not the given codepoint is a diacritic. In 'D' normalized Unicode text,
 * diacritics are removed by removing characters with these codepoints.
 */
bool codepointIsDiacritic(char32_t codepoint);

/**
 * Returns whether or not the given codepoint is considered a delimiter in the language 'lang'.
 * Currently, there is only a difference between English and non-English languages (the apostrophe).
 * To see which Unicode character categories were considered delimiters, see gen_delimiter_list.py.
 */
bool codepointIsDelimiter(char32_t codepoint, DelimiterListLanguage lang);

/**
 * Return a version of the given codepoint without any diacritics. These mappings are generated by
 * taking all of the characters within a set of Unicode code blocks (see gen_diacritic_map.py to see
 * which code blocks are used), decomposing them to the NFD normalization form, removing any
 * combining marks, and renormalizing them to the NFC form. The result is a mapping from original
 * codepoint to a codepoint with no diacritics.
 *
 * Returns 0 if codepoint is a pure diacritic mark (ie if codepointIsDiacritic() would return true).
 * You will need to distinguish this case from the input codepoint being 0 either by explicit
 * checking or avoiding a call to this function if codepoint is in the ASCII range (<0x80).
 */
char32_t codepointRemoveDiacritics(char32_t codepoint);

/**
 * Return the lowercased version of the given codepoint, applying the special Turkish version of
 * case folding if specified.
 */
char32_t codepointToLower(char32_t codepoint, CaseFoldMode mode = CaseFoldMode::kNormal);

}  // namespace unicode
}  // namespace mongo
