# Rippy

Rippy is a simple web scraper application with simple setup mostly aimed at producing high-quality datasets for language models. It is designed to be easy to set up and use.

Rippy operates differently than most other web scrapers; please read the Usage section for more information. 

## Usage

To use Rippy, you need to download the latest installer, which will enable you to use the command `Rippy create <projectname>` to
generate project file (`project.yml`) containing the settings and HTML rules for scraping each domain. The project file contains the following settings:

* `userAgent` - The user agent to use when scraping, this is used to identify the scraper to the server.
* `threads` - The number of threads to use when scraping. Increasing this will increase the speed of the scraper, but will also increase the load on the server.
* `depth` - The maximum depth to which the scraper should crawl. Setting this to 0 will disable the depth limit.
* `saveSession` - Allows you to resume an interrupted crawl without having to reload the pages by storing all visited subpages to a separate file.
* `domains` - The list of domains to start recursively scraping, and the rules for scraping each domain.
* `filter_mode` - The mode to use when filtering pages. `whitelist` will only scrape pages that match the filter, while `blacklist` will scrape all pages except those that match the filter.
* `start_pages ` - The list of pages to start scraping from on the domain.
* `rules` the HTML elements to parse text out of. For example, to get the text of `<span class="mw-page-title-main"></span>` rule: `tag: span, attribute: class, has: mw-page-title-main`.
* `output` - The name of the output text file.

Example project file:

```yml
# Crawler configuration  (YAML)
# The user agent to use when scraping, this is used to identify the scraper to the server.
userAgent: Rippy/1.0
threads: 4
depth: 0
saveSession: true
domains:
  - domain: en.wikipedia.com
    filter_mode: blacklist
    start_pages:
      - /wiki/Main_Page
    filter:
      - /w/index.php?title=Special
    rules:
      - tag: span
        attribute: class
        value:
        - mw-page-title-main
      - tag: div
        attribute: id
        value:
        - mw-content-text
output: output.txt
```

This will get the title and contents of each article.

To run the project, use the command `rippy start` in the directory containing the project file.

If any mistakes were made in the project file, most will be reported when you run the project.

## Contributing

We welcome any contributions to Rippy! If you have any ideas or suggestions, or if you'd like to help out with development, please open an issue or pull request.
