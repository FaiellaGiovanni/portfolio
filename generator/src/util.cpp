#include "util.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace util {

std::string readFile(const fs::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("cannot open " + path.string());

    std::ostringstream buffer;
    buffer << in.rdbuf();
    std::string text = buffer.str();

    // Files saved on Windows may start with a UTF-8 "byte order mark" and
    // use \r\n line endings. Normalise both so the parsers only see \n.
    if (startsWith(text, "\xEF\xBB\xBF")) text.erase(0, 3);
    text.erase(std::remove(text.begin(), text.end(), '\r'), text.end());
    return text;
}

void writeFile(const fs::path& path, const std::string& text) {
    if (path.has_parent_path()) fs::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::binary);
    if (!out) throw std::runtime_error("cannot write " + path.string());
    out << text;
}

std::string trim(const std::string& s) {
    const char* ws = " \t\n";
    const auto first = s.find_first_not_of(ws);
    if (first == std::string::npos) return "";
    const auto last = s.find_last_not_of(ws);
    return s.substr(first, last - first + 1);
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> parts;
    std::string current;
    for (char c : s) {
        if (c == delim) {
            parts.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    parts.push_back(current);
    return parts;
}

bool startsWith(const std::string& s, const std::string& prefix) {
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

std::string toLower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

std::string slugify(const std::string& s) {
    std::string slug;
    bool lastWasDash = true;  // avoids a leading dash
    for (unsigned char c : s) {
        if (std::isalnum(c)) {
            slug += static_cast<char>(std::tolower(c));
            lastWasDash = false;
        } else if (!lastWasDash) {
            slug += '-';
            lastWasDash = true;
        }
    }
    if (!slug.empty() && slug.back() == '-') slug.pop_back();
    return slug;
}

std::string htmlEscape(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        switch (c) {
            case '&': out += "&amp;"; break;
            case '<': out += "&lt;"; break;
            case '>': out += "&gt;"; break;
            case '"': out += "&quot;"; break;
            case '\'': out += "&#39;"; break;
            default: out += c;
        }
    }
    return out;
}

// Shared by parseConfig and parseFrontmatter: "key <sep> value".
static void parseKeyValueLines(const std::string& text, char sep, KeyValues& out) {
    for (const std::string& raw : split(text, '\n')) {
        const std::string line = trim(raw);
        if (line.empty() || line[0] == '#') continue;
        const auto pos = line.find(sep);
        if (pos == std::string::npos) continue;
        out[toLower(trim(line.substr(0, pos)))] = trim(line.substr(pos + 1));
    }
}

KeyValues parseConfig(const std::string& text) {
    KeyValues kv;
    parseKeyValueLines(text, '=', kv);
    return kv;
}

Document parseFrontmatter(const std::string& text) {
    Document doc;
    if (!startsWith(text, "---\n")) {
        doc.body = text;
        return doc;
    }
    const auto end = text.find("\n---", 3);
    if (end == std::string::npos) {
        doc.body = text;  // no closing fence: treat everything as body
        return doc;
    }
    parseKeyValueLines(text.substr(4, end - 4), ':', doc.meta);
    const auto bodyStart = text.find('\n', end + 1);
    doc.body = bodyStart == std::string::npos ? "" : text.substr(bodyStart + 1);
    return doc;
}

std::string get(const KeyValues& kv, const std::string& key, const std::string& fallback) {
    const auto it = kv.find(key);
    return (it == kv.end() || it->second.empty()) ? fallback : it->second;
}

}  // namespace util
