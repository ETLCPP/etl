#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from conans import ConanFile


class EmbeddedTemplateLibraryConan(ConanFile):
    name = "embedded-template-library"
    version = "14.11.2"
    license = "MIT"
    author = "John Wellbelove <smartgit@wellbelove.co.uk>"
    url = "https://github.com/ETLCPP/etl"
    description = "A C++ template library for embedded applications"
    topics = ("embedded", "template", "container")
    no_copy_source = True
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
