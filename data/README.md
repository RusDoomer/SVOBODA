# Data Directory

The `data` directory contains all the necessary files for defining languages, corpora, layouts, and weights. This structure allows for easy management and extension of the data used in the analysis and optimization of keyboard layouts.

## Directory Structure

-   **`languages/`**
    -   Each language has its own subdirectory (e.g., `data/english/`).
    -   Contains a `.lang` file defining the language's character set.
    -   Includes `corpora/` subdirectory specific to the language.

## Languages

Each language directory (e.g., `data/english/`) contains the following:

-   **`<language>.lang`**: Defines the character set for the language.
    -   The first two characters must be spaces
    -   Shifted characters are treated as the same characters and are to be placed adjacent to their unshifted counterparts.
    -   The `@` symbol is reserved and cannot be part of the language.
    -   The file must not exceed 100 characters (49 unshifted characters + space).
    -   Example: `data/english/english.lang`
-   **`corpora/`**: Contains text files used as corpora for the language.
    -   Each file is a plain text file representing a corpus.
    -   The program analyzes these files to gather n-gram frequency data.
    -   The first time a corpus is used, a `.cache` file will be generated to speed up future processing.
    -   Example: `data/english/corpora/shai.txt`

## Creating and Modifying Data

### Adding a New Language

1. Create a new directory under `data/` named after the language.
2. Create a `<language>.lang` file defining the character set.
3. Add at least one corpus file in the `corpora/` subdirectory.

### Adding a New Corpus

1. Place a unicode text file in the `data/<language>/corpora/` directory.
2. Ensure that the language of the corpora matches the `.lang` file's character set.

## Notes

-   Ensure that all data files are correctly formatted to avoid errors during processing.
-   The `.cache` files are automatically generated and should not be manually edited.
-   When adding new statistics or modifying existing ones, ensure that the corresponding weight files are updated accordingly.
