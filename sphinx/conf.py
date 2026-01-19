import os
import sys

# Get the absolute path of the 'sphinx' directory
current_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, current_dir)

from generate_groups import generate_group_rst

# Path to xml/ folder (just a direct child of 'sphinx')
xml_dir = os.path.join(current_dir, 'xml')
output_dir = os.path.join(current_dir, 'api_generated')

# Run it
if os.path.exists(xml_dir):
    generate_group_rst(xml_dir, output_dir)
else:
    print(f"!!! ERROR: XML folder not found at {xml_dir}")

# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Embedded Template Library'
copyright = '2026, John Wellbelove'
author = 'John Wellbelove'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'breathe',
    # 'sphinx.ext.autodoc', # Optional
]

# Point to the XML directory
# Since conf.py is in 'sphinx/', and xml is in 'sphinx/xml/', use './xml'
breathe_projects = {
    "ETL_Project": "./xml" 
}

# Set the default project so you don't have to type it every time
breathe_default_project = "ETL_Project"

# (Optional but Recommended) Map file extensions to a domain
# This ensures .h files are treated as C++ (or C) correctly
breathe_domain_by_extension = {
    "h": "cpp",
    "hpp": "cpp",
    "cpp": "cpp"
}

# Use the relative path from conf.py to the xml folder
breathe_projects = {
    "ETL_Project": "xml"  # If xml is inside sphinx/
}

# Optional: Set the theme
html_theme = 'sphinx_rtd_theme'

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'alabaster'
html_static_path = ['_static']


