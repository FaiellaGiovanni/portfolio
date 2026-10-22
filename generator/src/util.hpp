// util.hpp — small helpers shared by the rest of the generator:
// reading/writing files, trimming strings, escaping HTML, and parsing
// the simple "key = value" / frontmatter formats used in content/.
#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace util {

using KeyValues = std::map<std::string, std::string>;

// A content file split into its frontmatter (the block between the two
// "---" lines at the top) and the Markdown body that follows.
struct Document {
    KeyValues meta;
    std::string body;
};

std::string readFile(const fs::path& path);
void writeFile(const fs::path& path, const std::string& text);

std::string trim(const std::string& s);
std::vector<std::string> split(const std::string& s, char delim);
bool startsWith(const std::string& s, const std::string& prefix);
std::string toLower(std::string s);

// Turns "Spring-Mass Damper!" into "spring-mass-damper" (safe for URLs).
std::string slugify(const std::string& s);

// Replaces & < > " ' with HTML entities so text can't break the page.
std::string htmlEscape(const std::string& s);

// Parses lines like "name = Jane Doe". Blank lines and # comments are skipped.
KeyValues parseConfig(const std::string& text);

// Splits a Markdown file into frontmatter ("title: ...") and body.
Document parseFrontmatter(const std::string& text);

// Looks up a key, returning `fallback` if it is missing or empty.
std::string get(const KeyValues& kv, const std::string& key, const std::string& fallback = "");

}  // namespace util
