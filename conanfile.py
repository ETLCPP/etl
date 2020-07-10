#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from conans import ConanFile, tools


def get_version_from_git_tag():
    """
    :returns: The git tag associated with the current commit, or None if it is not able to get the Git data.

    Returning None is necessary when the recipe is already in the Conan cache, and the Git repository may not be there.
    A value of None makes Conan get the version from the metadata.
    See: https://docs.conan.io/en/latest/howtos/capture_version.html
    """
    try:
        return tools.Git().get_tag()
    except:
        return None

class EmbeddedTemplateLibraryConan(ConanFile):
    name = "embedded-template-library"
    version = "17.4.2"
    license = "MIT"
    author = "John Wellbelove <john.wellbelove@etlcpp.com>"
    url = "https://github.com/ETLCPP/etl"
    description = "A C++ template library for embedded applications"
    topics = ("embedded", "template", "container")

    # Source info
    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto"
    }

    def package(self):
        self.copy("LICENSE", "licenses")
        self.copy("*.h", src="include", dst="include")
   
    def package_id(self):
        self.info.header_only()
