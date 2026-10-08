# Formatting

The repository uses [pre-commit](https://pre-commit.com/) to keep source and configuration files
consistently formatted. The hooks run automatically before each commit and in CI.

## Setup

The VS Code development container installs pre-commit and all system formatters. After opening the
repository in the container, install the Git hook once:

```sh
pre-commit install
```

To install the hook in another environment, install pre-commit, clang-format, cmake-format,
clang-tidy, and Qt's QML formatter (`qmllformat` or `qmlformat`). Prettier and Ruff are installed
in isolated environments by pre-commit.

## Run formatting

Format all tracked files and verify the result with:

```sh
pre-commit run --all-files
```

The hooks use clang-format for C and C++, cmake-format for CMake, Ruff for Python, Prettier for JSON,
YAML, and Markdown, and the custom `qmllformat` hook for QML. The QML hook prefers the
`qmllformat` executable and falls back to Qt's `qmlformat` executable.

Project-wide style is defined in `.clang-format`, `.clang-tidy`, `.cmake-format.yaml`,
`.qmlformat.ini`, `.prettierrc.json`, and `pyproject.toml`. Update those settings rather than adding
per-file formatting exceptions.
