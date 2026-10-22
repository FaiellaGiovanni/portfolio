#include "templates.hpp"

#include <iostream>

std::string TemplateSet::render(const std::string& name, const util::KeyValues& vars) {
    auto it = cache_.find(name);
    if (it == cache_.end()) {
        it = cache_.emplace(name, util::readFile(dir_ / (name + ".html"))).first;
    }
    const std::string& tpl = it->second;

    std::string out;
    std::size_t pos = 0;
    while (true) {
        const auto open = tpl.find("{{", pos);
        if (open == std::string::npos) break;
        const auto close = tpl.find("}}", open + 2);
        if (close == std::string::npos) break;

        out.append(tpl, pos, open - pos);
        const std::string key = util::trim(tpl.substr(open + 2, close - open - 2));

        const auto value = vars.find(key);
        if (value != vars.end()) {
            out += value->second;
        } else if (warned_.insert(name + ":" + key).second) {
            std::cerr << "warning: " << name << ".html uses {{" << key << "}} but no value was given\n";
        }
        pos = close + 2;
    }
    out.append(tpl, pos, std::string::npos);
    return out;
}
