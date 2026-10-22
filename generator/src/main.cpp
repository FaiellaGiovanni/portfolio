// main.cpp — the portfolio site generator.
//
// Reads everything in content/, pours it into the HTML files in templates/,
// copies static/ (CSS, images, later WebAssembly) and writes the finished
// website to public/. Run it from the project folder:
//
//     build\sitegen.exe            (Windows)
//     ./build/sitegen              (macOS / Linux)
//
// Optionally pass the project folder as the first argument.

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "markdown.hpp"
#include "templates.hpp"
#include "util.hpp"

namespace {

struct Project {
    std::string slug;   // file name without .md, used in the URL
    util::KeyValues meta;
    std::string html;   // body converted from Markdown
    int order = 100;
};

struct Page {
    std::string slug;
    util::KeyValues meta;
    std::string html;
    int navOrder = -1;  // -1 = not shown in the navigation bar
};

int toInt(const std::string& s, int fallback) {
    try {
        return s.empty() ? fallback : std::stoi(s);
    } catch (...) {
        return fallback;
    }
}

std::string currentYear() {
    const std::time_t now = std::time(nullptr);
    const std::tm* local = std::localtime(&now);
    return std::to_string(local->tm_year + 1900);
}

// Reads every .md file in a folder, sorted by file name.
std::vector<std::pair<std::string, util::Document>> readMarkdownFolder(const fs::path& dir) {
    std::vector<std::pair<std::string, util::Document>> docs;
    if (!fs::exists(dir)) return docs;
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".md") {
            docs.emplace_back(entry.path().stem().string(), util::parseFrontmatter(util::readFile(entry.path())));
        }
    }
    std::sort(docs.begin(), docs.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    return docs;
}

class SiteBuilder {
public:
    explicit SiteBuilder(fs::path root)
        : root_(std::move(root)), out_(root_ / "public"), templates_(root_ / "templates") {}

    int build() {
        for (const char* required : {"content", "templates"}) {
            if (!fs::is_directory(root_ / required)) {
                std::cerr << "error: no '" << required << "' folder in " << fs::absolute(root_).string()
                          << "\n       Run the generator from the portfolio project folder.\n";
                return 1;
            }
        }

        site_ = util::parseConfig(util::readFile(root_ / "content" / "site.cfg"));
        loadProjects();
        loadPages();

        cleanOutput();
        if (fs::exists(root_ / "static")) {
            fs::copy(root_ / "static", out_, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        }
        util::writeFile(out_ / ".nojekyll", "");  // tells GitHub Pages to serve files as-is

        buildHome();
        buildProjectIndex();
        for (const Project& p : projects_) buildProject(p);
        for (const Page& p : pages_) buildPage(p);
        return 0;
    }

    int pagesWritten() const { return pagesWritten_; }

private:
    fs::path root_;
    fs::path out_;
    TemplateSet templates_;
    util::KeyValues site_;
    std::vector<Project> projects_;
    std::vector<Page> pages_;
    int pagesWritten_ = 0;

    // Empties public/ so pages you deleted don't linger. On Windows, a folder
    // that Live Server or OneDrive is using can't be removed; that's fine,
    // so failures are ignored and its files are simply overwritten.
    void cleanOutput() {
        fs::create_directories(out_);
        for (const auto& entry : fs::directory_iterator(out_)) {
            std::error_code ignored;
            fs::remove_all(entry.path(), ignored);
        }
    }

    // ----- loading content ------------------------------------------------

    void loadProjects() {
        for (auto& [slug, doc] : readMarkdownFolder(root_ / "content" / "projects")) {
            Project p;
            p.slug = util::slugify(slug);
            p.order = toInt(util::get(doc.meta, "order"), 100);
            p.html = markdownToHtml(doc.body);
            p.meta = std::move(doc.meta);
            projects_.push_back(std::move(p));
        }
        std::stable_sort(projects_.begin(), projects_.end(),
                         [](const Project& a, const Project& b) { return a.order < b.order; });
    }

    void loadPages() {
        for (auto& [slug, doc] : readMarkdownFolder(root_ / "content")) {
            if (slug == "index") continue;  // the home page is built separately
            Page p;
            p.slug = util::slugify(slug);
            p.navOrder = toInt(util::get(doc.meta, "nav_order"), -1);
            p.html = markdownToHtml(doc.body);
            p.meta = std::move(doc.meta);
            pages_.push_back(std::move(p));
        }
    }

    // ----- shared pieces --------------------------------------------------

    std::string navHtml(const std::string& root, const std::string& active) const {
        struct Item { std::string id, label, href; int order; };
        std::vector<Item> items = {
            {"home", "Home", root + "index.html", 0},
            {"projects", "Projects", root + "projects/index.html", 1},
        };
        for (const Page& p : pages_) {
            if (p.navOrder >= 0) {
                items.push_back({p.slug, util::get(p.meta, "title", p.slug), root + p.slug + ".html", p.navOrder});
            }
        }
        std::stable_sort(items.begin(), items.end(), [](const Item& a, const Item& b) { return a.order < b.order; });

        std::string html;
        for (const Item& item : items) {
            const std::string current = item.id == active ? " aria-current=\"page\"" : "";
            html += "<li><a href=\"" + item.href + "\"" + current + ">" + util::htmlEscape(item.label) + "</a></li>";
        }
        return html;
    }

    std::string contactLinksHtml() const {
        std::string html;
        const std::string email = util::get(site_, "email");
        if (!email.empty()) {
            html += "<li><a href=\"mailto:" + util::htmlEscape(email) + "\">Email</a></li>";
        }
        for (const auto& [key, label] : std::vector<std::pair<std::string, std::string>>{
                 {"linkedin", "LinkedIn"}, {"github", "GitHub"}}) {
            const std::string url = util::get(site_, key);
            if (!url.empty()) {
                html += "<li><a href=\"" + util::htmlEscape(url) + "\" target=\"_blank\" rel=\"noopener\">" + label + "</a></li>";
            }
        }
        return html;
    }

    // The status label ("Complete", "In progress"). When there is no date
    // in front of it, the "solo" class drops the divider line.
    std::string statusHtml(const util::KeyValues& meta) const {
        const std::string status = util::get(meta, "status");
        if (status.empty()) return "";
        const std::string cls = util::get(meta, "date").empty() ? "status solo" : "status";
        return "<span class=\"" + cls + "\">" + util::htmlEscape(status) + "</span>";
    }

    std::string tagsHtml(const std::string& csv) const {
        std::string html;
        for (const std::string& raw : util::split(csv, ',')) {
            const std::string tag = util::trim(raw);
            if (!tag.empty()) html += "<li>" + util::htmlEscape(tag) + "</li>";
        }
        return html.empty() ? "" : "<ul class=\"tags\">" + html + "</ul>";
    }

    std::string projectCard(const Project& p, const std::string& root) {
        return templates_.render("project-card", {
            {"url", root + "projects/" + p.slug + ".html"},
            {"title", util::htmlEscape(util::get(p.meta, "title", p.slug))},
            {"summary", util::htmlEscape(util::get(p.meta, "summary"))},
            {"date", util::htmlEscape(util::get(p.meta, "date"))},
            {"status", statusHtml(p.meta)},
            {"tags", tagsHtml(util::get(p.meta, "tags"))},
        });
    }

    // Wraps page content in templates/base.html and writes it to public/.
    // `root` is the relative path back to the site root ("" or "../"),
    // which keeps links working both on GitHub Pages and from your disk.
    void writePage(const fs::path& relPath, const std::string& root, const std::string& title,
                   const std::string& description, const std::string& active, const std::string& content) {
        util::KeyValues vars;
        for (const auto& [key, value] : site_) vars["site." + key] = util::htmlEscape(value);

        const std::string siteName = util::get(site_, "name", "My Portfolio");
        vars["title"] = util::htmlEscape(title == siteName ? siteName : title + " · " + siteName);
        vars["description"] = util::htmlEscape(description.empty() ? util::get(site_, "tagline") : description);
        vars["root"] = root;
        vars["nav"] = navHtml(root, active);
        vars["contact"] = contactLinksHtml();
        vars["year"] = currentYear();
        vars["content"] = content;

        util::writeFile(out_ / relPath, templates_.render("base", vars));
        ++pagesWritten_;
    }

    // ----- the pages ------------------------------------------------------

    void buildHome() {
        const auto home = util::parseFrontmatter(util::readFile(root_ / "content" / "index.md"));
        const int featuredCount = toInt(util::get(home.meta, "featured_count"), 3);

        std::string featured;
        for (int i = 0; i < featuredCount && i < static_cast<int>(projects_.size()); ++i) {
            featured += projectCard(projects_[i], "");
        }

        util::KeyValues vars;
        for (const auto& [key, value] : site_) vars["site." + key] = util::htmlEscape(value);
        vars["intro"] = markdownToHtml(home.body);
        vars["featured"] = featured;
        vars["contact"] = contactLinksHtml();

        const std::string name = util::get(site_, "name", "My Portfolio");
        writePage("index.html", "", name, "", "home", templates_.render("home", vars));
    }

    void buildProjectIndex() {
        std::string cards;
        for (const Project& p : projects_) cards += projectCard(p, "../");
        writePage("projects/index.html", "../", "Projects", "", "projects",
                  templates_.render("projects", {{"cards", cards}}));
    }

    void buildProject(const Project& p) {
        const std::string title = util::get(p.meta, "title", p.slug);
        const std::string content = templates_.render("project", {
            {"root", "../"},
            {"title", util::htmlEscape(title)},
            {"summary", util::htmlEscape(util::get(p.meta, "summary"))},
            {"date", util::htmlEscape(util::get(p.meta, "date"))},
            {"status", statusHtml(p.meta)},
            {"tags", tagsHtml(util::get(p.meta, "tags"))},
            {"body", p.html},
        });
        writePage("projects/" + p.slug + ".html", "../", title, util::get(p.meta, "summary"), "projects", content);
    }

    void buildPage(const Page& p) {
        const std::string title = util::get(p.meta, "title", p.slug);
        const std::string content = templates_.render("page", {{"title", util::htmlEscape(title)}, {"body", p.html}});
        writePage(p.slug + ".html", "", title, util::get(p.meta, "description"), p.slug, content);
    }
};

}  // namespace

int main(int argc, char* argv[]) {
    const auto start = std::chrono::steady_clock::now();
    const fs::path root = argc > 1 ? fs::path(argv[1]) : fs::current_path();

    try {
        SiteBuilder builder(root);
        if (const int code = builder.build(); code != 0) return code;

        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now() - start).count();
        std::cout << "Built " << builder.pagesWritten() << " pages into public/ in " << ms << " ms\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
}
