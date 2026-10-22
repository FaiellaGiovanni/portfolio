// markdown.hpp — a small Markdown-to-HTML converter.
//
// Supports the subset a portfolio needs:
//   # Headings (1–6)        **bold**   *italic*   `code`
//   - bullet lists          1. numbered lists
//   > quotes                [link](url)   ![image](src)
//   ``` code blocks ```     --- horizontal rule
//   Lines starting with "<" are passed through as raw HTML
//   (handy for embedding a simulation later).
//   <!-- private notes --> are removed and never published.
#pragma once

#include <string>

std::string markdownToHtml(const std::string& markdown);
