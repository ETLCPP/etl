---
title: "Hugo commands"
---

Ensure your current working directory is `hugo/` before running these commands.

## Generate hugo for localhost
If you have previously generated for a different host then run `hugo --cleanDestinationDir`  

Run either of these two commands.

---

```bash
hugo server
```
This is quicker, as Hugo will only regenerate files it detects have changed.  

If you have moved files then the left-hand menu may not always regenerate correctly. If this occurs then run the following command.

---

```bash
hugo server --disableFastRender
```
Hugo will regenerate all of the output html files.  
This takes a few seconds to run, but ensures all output files are updated.

## Generate hugo for remote host
If you have previously generated for a different host then run `hugo --cleanDestinationDir`  

```bash
hugo --baseURL "https://www.your-web-site.com"
```
