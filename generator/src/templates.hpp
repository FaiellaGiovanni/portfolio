// templates.hpp — fills HTML templates that contain {{placeholders}}.
//
// A template like  <h1>{{title}}</h1>  rendered with {"title": "Hello"}
// becomes  <h1>Hello</h1>. Values are inserted as-is (they may contain
// HTML), so escape plain text with util::htmlEscape before passing it in.
#pragma once

#include <map>
#include <set>
#include <string>

#include "util.hpp"

class TemplateSet {
public:
    explicit TemplateSet(fs::path dir) : dir_(std::move(dir)) {}

    // Loads templates/<name>.html (cached) and fills its placeholders.
    std::string render(const std::string& name, const util::KeyValues& vars);

private:
    fs::path dir_;
    std::map<std::string, std::string> cache_;
    std::set<std::string> warned_;  // so each missing placeholder is reported once
};
