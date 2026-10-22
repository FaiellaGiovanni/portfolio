#include "markdown.hpp"

#include <cctype>
#include <vector>

#include "util.hpp"

namespace {

// ---------------------------------------------------------------------------
// Inline formatting: **bold**, *italic*, `code`, [links](url), ![img](src)
// ---------------------------------------------------------------------------

struct Link {
    std::string text;
    std::string url;
    std::size_t end = 0;  // index just past the closing ")"
};

// Tries to read "[text](url)" starting at s[open] == '['.
bool parseLink(const std::string& s, std::size_t open, Link& link) {
    const auto close = s.find(']', open + 1);
    if (close == std::string::npos || close + 1 >= s.size() || s[close + 1] != '(') return false;
    const auto paren = s.find(')', close + 2);
    if (paren == std::string::npos) return false;
    link.text = s.substr(open + 1, close - open - 1);
    link.url = util::trim(s.substr(close + 2, paren - close - 2));
    link.end = paren + 1;
    return true;
}

bool isExternal(const std::string& url) {
    return util::startsWith(url, "http://") || util::startsWith(url, "https://");
}

std::string renderInline(const std::string& s) {
    std::string out;
    std::size_t i = 0;

    while (i < s.size()) {
        const char c = s[i];
        const bool hasNext = i + 1 < s.size();

        if (c == '`') {
            const auto end = s.find('`', i + 1);
            if (end != std::string::npos) {
                out += "<code>" + util::htmlEscape(s.substr(i + 1, end - i - 1)) + "</code>";
                i = end + 1;
                continue;
            }
        }

        if (c == '!' && hasNext && s[i + 1] == '[') {
            Link link;
            if (parseLink(s, i + 1, link)) {
                out += "<img src=\"" + util::htmlEscape(link.url) + "\" alt=\"" +
                       util::htmlEscape(link.text) + "\" loading=\"lazy\">";
                i = link.end;
                continue;
            }
        }

        if (c == '[') {
            Link link;
            if (parseLink(s, i, link)) {
                const std::string target = isExternal(link.url) ? " target=\"_blank\" rel=\"noopener\"" : "";
                out += "<a href=\"" + util::htmlEscape(link.url) + "\"" + target + ">" +
                       renderInline(link.text) + "</a>";
                i = link.end;
                continue;
            }
        }

        if (c == '*' && hasNext && s[i + 1] == '*') {
            const auto end = s.find("**", i + 2);
            if (end != std::string::npos && end > i + 2) {
                out += "<strong>" + renderInline(s.substr(i + 2, end - i - 2)) + "</strong>";
                i = end + 2;
                continue;
            }
        }

        if (c == '*') {
            const auto end = s.find('*', i + 1);
            if (end != std::string::npos && end > i + 1) {
                out += "<em>" + renderInline(s.substr(i + 1, end - i - 1)) + "</em>";
                i = end + 1;
                continue;
            }
        }

        // Plain character (escaped so it can't be mistaken for HTML).
        out += util::htmlEscape(std::string(1, c));
        ++i;
    }
    return out;
}

// ---------------------------------------------------------------------------
// Block structure: headings, paragraphs, lists, quotes, code, raw HTML
// ---------------------------------------------------------------------------

// If `line` looks like "12. text", returns the text after the number.
bool orderedItem(const std::string& line, std::string& rest) {
    std::size_t i = 0;
    while (i < line.size() && std::isdigit(static_cast<unsigned char>(line[i]))) ++i;
    if (i == 0 || i + 1 >= line.size() || line[i] != '.' || line[i + 1] != ' ') return false;
    rest = util::trim(line.substr(i + 2));
    return true;
}

class BlockRenderer {
public:
    std::string render(const std::string& markdown) {
        for (const std::string& line : util::split(markdown, '\n')) handleLine(line);
        if (inCode_) out_ += "</code></pre>\n";
        closeAll();
        return out_;
    }

private:
    enum class List { None, Bullets, Numbers };

    std::string out_;
    std::vector<std::string> paragraph_;
    std::vector<std::string> quote_;
    List list_ = List::None;
    bool inCode_ = false;
    bool inRawHtml_ = false;
    bool inNote_ = false;

    void flushParagraph() {
        if (paragraph_.empty()) return;
        std::string joined;
        for (const auto& p : paragraph_) joined += (joined.empty() ? "" : " ") + p;
        out_ += "<p>" + renderInline(joined) + "</p>\n";
        paragraph_.clear();
    }

    void flushQuote() {
        if (quote_.empty()) return;
        std::string joined;
        for (const auto& q : quote_) joined += (joined.empty() ? "" : " ") + q;
        out_ += "<blockquote><p>" + renderInline(joined) + "</p></blockquote>\n";
        quote_.clear();
    }

    void closeList() {
        if (list_ == List::Bullets) out_ += "</ul>\n";
        if (list_ == List::Numbers) out_ += "</ol>\n";
        list_ = List::None;
    }

    void closeAll() {
        flushParagraph();
        flushQuote();
        closeList();
    }

    void openList(List type) {
        if (list_ == type) return;
        closeAll();
        out_ += (type == List::Bullets) ? "<ul>\n" : "<ol>\n";
        list_ = type;
    }

    void handleLine(const std::string& line) {
        const std::string t = util::trim(line);

        // Inside ``` fences everything is literal until the closing fence.
        if (inCode_) {
            if (util::startsWith(t, "```")) {
                out_ += "</code></pre>\n";
                inCode_ = false;
            } else {
                out_ += util::htmlEscape(line) + "\n";
            }
            return;
        }

        // Private notes: anything between <!-- and --> is skipped entirely,
        // so it stays in the .md file but never reaches the website.
        if (inNote_) {
            if (line.find("-->") != std::string::npos) inNote_ = false;
            return;
        }
        if (!inRawHtml_ && util::startsWith(t, "<!--")) {
            if (t.find("-->", 4) == std::string::npos) inNote_ = true;
            return;
        }

        // A raw HTML block lasts until the next blank line.
        if (inRawHtml_) {
            if (t.empty()) {
                inRawHtml_ = false;
            } else {
                out_ += line + "\n";
            }
            return;
        }

        if (t.empty()) {
            closeAll();
            return;
        }

        if (util::startsWith(t, "```")) {
            closeAll();
            const std::string lang = util::trim(t.substr(3));
            out_ += "<pre><code";
            if (!lang.empty()) out_ += " class=\"language-" + util::htmlEscape(lang) + "\"";
            out_ += ">";
            inCode_ = true;
            return;
        }

        if (t == "---" || t == "***") {
            closeAll();
            out_ += "<hr>\n";
            return;
        }

        if (t[0] == '#') {
            std::size_t level = 0;
            while (level < t.size() && t[level] == '#') ++level;
            if (level <= 6 && level < t.size() && t[level] == ' ') {
                closeAll();
                const std::string text = util::trim(t.substr(level));
                const std::string tag = "h" + std::to_string(level);
                out_ += "<" + tag + " id=\"" + util::slugify(text) + "\">" + renderInline(text) + "</" + tag + ">\n";
                return;
            }
        }

        if (t[0] == '<') {
            closeAll();
            inRawHtml_ = true;
            out_ += line + "\n";
            return;
        }

        if (t[0] == '>') {
            flushParagraph();
            closeList();
            quote_.push_back(util::trim(t.substr(1)));
            return;
        }

        if (util::startsWith(t, "- ") || util::startsWith(t, "* ")) {
            openList(List::Bullets);
            out_ += "<li>" + renderInline(util::trim(t.substr(2))) + "</li>\n";
            return;
        }

        std::string itemText;
        if (orderedItem(t, itemText)) {
            openList(List::Numbers);
            out_ += "<li>" + renderInline(itemText) + "</li>\n";
            return;
        }

        // Ordinary text: part of the current paragraph.
        flushQuote();
        closeList();
        paragraph_.push_back(t);
    }
};

}  // namespace

std::string markdownToHtml(const std::string& markdown) {
    return BlockRenderer().render(markdown);
}
