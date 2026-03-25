---
title: "Header guards"
weight: 6
---

---

The ETL as a default uses `#include` headers guards.  
If you wish, you can convert them to using `#pragma once`.

## Install guardonce
`python3 -m pip install guardonce`

## Modify all of the headers
Navigate to `etl/scripts`

To change from `#include` guard to `#pragma once`
`./convert_headers_to_use_once.bat`

To change from `#pragma` once to `#include guard`
`./convert_headers_to_use_guards.bat`
