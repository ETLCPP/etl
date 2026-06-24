# Viewing the documentation locally

The documentation for this project is built using [Hugo](https://gohugo.io/), a static site generator.
This guide will walk you through installing Hugo and running the documentation site on your machine.

---

## Prerequisites

- You must have already cloned this repository, including its submodules (the Hugo themes are stored as submodules).
- If you cloned without submodules, run this first:
```bash
  git submodule update --init --recursive
```

---

## Step 1: Install Hugo Extended

This project requires the **extended** version of Hugo, which includes support for additional features such as SCSS/Sass processing.  
Make sure you install the extended version, not the standard one.

### Windows

1. The easiest way to install Hugo on Windows is via [Chocolatey](https://chocolatey.org/). If you have it installed, open a command prompt as Administrator and run:
```bash
   choco install hugo-extended -y
```
2. If you don't have Chocolatey, you can download Hugo Extended directly from the [Hugo releases page](https://github.com/gohugoio/hugo/releases).  
Download the file named `hugo_extended_x.x.x_windows-amd64.zip`, extract it, and add the folder to your system PATH.
3. Verify the installation by opening a new command prompt and running:
```bash
   hugo version
```
   You should see the word **extended** in the output, for example: `hugo v0.x.x+extended`.

### macOS

1. The easiest way to install Hugo Extended on macOS is via [Homebrew](https://brew.sh/). Homebrew installs the extended version by default. If you have it installed, open a terminal and run:
```bash
   brew install hugo
```
2. If you don't have Homebrew, you can install it first by following the instructions at [brew.sh](https://brew.sh/), then run the command above.
3. Verify the installation by running:
```bash
   hugo version
```
   You should see the word **extended** in the output, for example: `hugo v0.x.x+extended`.

### Linux

1. Package managers often provide an older or non-extended version of Hugo, so the recommended approach is to download the latest extended release directly from the [Hugo releases page](https://github.com/gohugoio/hugo/releases).
   Download the file named `hugo_extended_x.x.x_linux-amd64.tar.gz`, then run:
```bash
   tar -xzf hugo_extended_x.x.x_linux-amd64.tar.gz
   sudo mv hugo /usr/local/bin/
```
2. Verify the installation by running:
```bash
   hugo version
```
   You should see the word **extended** in the output, for example: `hugo v0.x.x+extended`.

---

## Step 2: Run the documentation site

1. Open a terminal (or command prompt on Windows) and navigate to the `hugo` directory inside the project:
```bash
   cd hugo
```

2. Start the Hugo development server:
```bash
   hugo server
```

3. Hugo will start a local web server. You will see output similar to:
```
   Web Server is available at http://localhost:1313/
```

4. Open your browser and go to **http://localhost:1313/** to view the documentation.

---

## Stopping the server

To stop the Hugo server, go back to your terminal and press **Ctrl+C**.

---

## Editing the Documentation

The documentation source files are located in the `/docs` directory at the root of the repository —
**not** inside the `hugo` directory. Hugo is configured to mount this directory automatically via
`hugo.toml`, so any changes you make to files in `/docs` will be picked up by the Hugo development
server straight away.

Here is a quick overview of where things live:
```
hugo/
├── layouts/        # HTML templates (you probably won't need to touch these)
├── themes/         # The Hugo theme (managed as a git submodule)
└── hugo.toml       # Hugo configuration file (includes the /docs content mount)

docs/
└── section-name/   # Each subdirectory becomes a section in the documentation
    ├── _index.md   # The landing page for that section
    └── my-page.md  # Individual pages within the section
```

### Adding or Editing a Page

All documentation pages are written in **Markdown** (`.md` files). To edit an existing page, open
the relevant `.md` file in the `/docs` directory and make your changes.

The documentation is written in [Goldmark](https://github.com/teekennedy/goldmark-markdown) markdown.

Hugo uses [Goldmark](https://github.com/teekennedy/goldmark-markdown) as its default Markdown processor for versions 0.60.0 and newer.  
It is built into Hugo, providing high-performance and fully [CommonMark](https://commonmark.org/)-compliant rendering, along with support for [GitHub](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax) Markdown.

### Starting a New Page

A template file is provided at `docs/page-template.md` to use as a starting point. Copy it to the
appropriate section directory, rename it, and edit the front matter, in particular, change the
`title` and remove the `draft: true` line, otherwise Hugo will not include your page in the
documentation.

### Previewing Your Changes

If you have the Hugo development server running (see [Step 2](#step-2--run-the-documentation-site)),
your changes will appear in the browser automatically as soon as you save the file. There is no need
to restart the server.

### Images and Other Assets

If you need to include images in your documentation, place them in the `hugo/static/` directory and
reference them in your Markdown like so:
```markdown
![Alt text](/my-image.png)
```

The site will automatically refresh in your browser whenever you save changes to the documentation files.
