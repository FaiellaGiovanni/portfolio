---
title: This Website
summary: A static site generator written from scratch in C++17 that turns Markdown files into this portfolio.
date: 2026
status: Live
tags: C++17, std::filesystem, Markdown parser
order: 11
---
## The idea

Instead of using a website builder, I wrote a program in C++ that builds the site. I write each page as a plain text file, and the generator converts it into HTML, applies a shared layout, and writes the finished website to a folder that gets published.

## How it works

1. **Reads content:** every `.md` file in the `content/` folder, plus site settings from `site.cfg`.
2. **Parses frontmatter:** the `title:`, `summary:` and `tags:` lines at the top of each file.
3. **Converts Markdown to HTML:** a small hand-written parser handles headings, lists, links, images, bold, italics and code.
4. **Fills templates:** layout files with `{{placeholders}}` get replaced by the page's content.
5. **Writes the site:** the result lands in `public/`, ready to upload.

## What I learned

- File and folder handling with `std::filesystem`
- Writing a parser one character at a time, and the edge cases that come with it
- Keeping a program organized across several source files
