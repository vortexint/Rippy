# Rippy

Rippy is a simple web scraper application with simple setup aimed at producing datasets for language models. It is designed to be easy to set up and use, and to produce high-quality datasets for training language models.

## Usage

To use Rippy, you need to create a project file (`project.yml`) containing the settings and HTML rules for scraping each domain. The project file contains the following settings:

* `userAgent` - The user agent to use when scraping, this is used to identify the scraper to the server.
* `threads` - The number of threads to use when scraping. Increasing this will increase the speed of the scraper, but will also increase the load on the server.
* `depth` - The maximum depth to which the scraper should crawl. Setting this to 0 will disable the depth limit.
* `domains` - The list of domains to start recursively scraping, and the rules for scraping each domain.
* `output` - The name of the output text file.

Example project file:

```yml
# Crawler configuration  (YAML)
# The user agent to use when scraping, this is used to identify the scraper to the server.
userAgent: Rippy/1.0
threads: 4 # Increasing this will increase the speed of the scraper, but will also increase the load on the server.
depth: 0 # disable depth limit, e.g. 10000 would limit the scraper to 10000 pages
domains:
  - domain: en.wikipedia.com
    start_page:
      - /wiki/Main_Page
    avoid:
      - /w/index.php?title=Special
    rules:
      - tag: span
        attribute: class
        has: mw-page-title-main
      - tag: div
        attribute: id
        has: mw-content-text
output: output.txt
```

This will get the title and contents of each article.

To run the project, use the command `rippy start` in the directory containing the project file.

## Contributing

We welcome any contributions to Rippy! If you have any ideas or suggestions, or if you'd like to help out with development, please open an issue or pull request.