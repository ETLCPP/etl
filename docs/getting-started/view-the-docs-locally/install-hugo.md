---
title: "Install Hugo"
weight: 72
---

You need to install **Hugo** if you want to replicate the ETL online documentation.

You need to install the **extended version** of **Hugo** on each platform:

*The extended version includes support for Sass/SCSS compilation and WebP image processing, which is required by many **Hugo** themes.*

## Install Go

**Hugo** requires **Go** to support **Hugo Modules**.

Visit [here](https://go.dev/dl/) to install the version applicable to your OS.

Check that it is installed correctly with the command  
```bash
go version
```

## Windows
<div class="not-prose badges">
  <img src="/Windows.png" alt="Windows" align="left" style="margin-right: 15px;">
</div>

**Option 1: Winget (recommended)**
```powershell
winget install Hugo.Hugo.Extended
```

**Option 2: Chocolatey**
```powershell
choco install hugo-extended
```

**Option 3: Scoop**
```powershell
scoop install hugo-extended
```

**Option 4: Manual**
1. Download the `hugo_extended_x.x.x_windows-amd64.zip` from the [Hugo releases page](https://github.com/gohugoio/hugo/releases)

1. Extract the zip and place `hugo.exe` in a folder (e.g. `C:\Hugo\bin`)

1. Add that folder to your system `PATH`

## Linux
<div class="not-prose badges">
  <img src="/Linux.png" alt="Linux" align="left" style="margin-right: 15px;">
</div>

**Option 1: Snap**
```bash
sudo snap install hugo --channel=extended/stable --classic
```

**Option 2: Package manager (Debian/Ubuntu)**
```bash
sudo apt install hugo
```
> ⚠️ Apt may serve an older version. Prefer snap or manual install for the latest extended build.

**Option 3: Manual (always latest)**
```bash
# Replace x.x.x with the latest version number
wget https://github.com/gohugoio/hugo/releases/download/vx.x.x/hugo_extended_x.x.x_linux-amd64.tar.gz
tar -xzf hugo_extended_x.x.x_linux-amd64.tar.gz
sudo mv hugo /usr/local/bin/
```

## macOS
<div class="not-prose badges">
  <img src="/macOS.png" alt="macOS" align="left" style="margin-right: 15px;">
</div>

**Option 1: Homebrew (recommended)**
```bash
brew install hugo
```
> Homebrew installs the extended version by default.

**Option 2: MacPorts**
```bash
sudo port install hugo
```

**Option 3: Manual**
1. Download `hugo_extended_x.x.x_darwin-universal.tar.gz` from the [Hugo releases page](https://github.com/gohugoio/hugo/releases)
2. Extract and move the binary:
```bash
tar -xzf hugo_extended_*.tar.gz
sudo mv hugo /usr/local/bin/
```

## Verify the installation

On all platforms, confirm it's the **extended** version:
```bash
hugo version
```
You should see `+extended` in the output, e.g.:
```
hugo v0.147.0+extended linux/amd64 BuildDate=...
```
