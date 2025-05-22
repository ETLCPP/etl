#!/usr/bin/env bash

# For more detailed debugging, uncomment the next line
# set -x

# Explicitly set CMAKE_VERSION from first argument, default to "none"
CMAKE_VERSION=${1:-"none"}

# --- Configuration ---
DOWNLOAD_ATTEMPTS=3
DOWNLOAD_RETRY_DELAY=5 # seconds

# --- Global Variables ---
TMP_DIR="" # Initialize TMP_DIR, will be set by mktemp
# Filenames used within TMP_DIR
CMAKE_INSTALLER_SCRIPT_LOCAL_NAME="cmake-installer.sh"
CMAKE_CHECKSUM_FILE_LOCAL_NAME="cmake-checksums.txt"
# This will be the actual name of the CMake binary, derived from version and arch
# It's important for matching against the checksum file.
DERIVED_CMAKE_BINARY_FILENAME=""

# --- Cleanup Function ---
# This trap will execute on EXIT, ERR, SIGINT, SIGTERM
# It's crucial for debugging to see the state of TMP_DIR if things go wrong.
cleanup() {
    # $? is the exit code of the last command before the trap was triggered
    # or the argument to exit if the script called exit explicitly.
    LAST_EXIT_CODE=$?
    echo # Newline for readability

    # Only proceed with detailed cleanup if TMP_DIR was actually created
    if [[ -n "${TMP_DIR}" && -d "${TMP_DIR}" ]]; then
        echo "--- Cleanup: Temporary Directory Inspector (${TMP_DIR}) ---"
        echo "Listing contents of TMP_DIR:"
        ls -la "${TMP_DIR}"

        # Check and display checksum file content
        if [[ -f "${TMP_DIR}/${CMAKE_CHECKSUM_FILE_LOCAL_NAME}" ]]; then
            echo "--- Content of downloaded checksum file (${CMAKE_CHECKSUM_FILE_LOCAL_NAME}) ---"
            cat "${TMP_DIR}/${CMAKE_CHECKSUM_FILE_LOCAL_NAME}"
            echo "--- End of checksum file ---"
        else
            echo "Checksum file (${CMAKE_CHECKSUM_FILE_LOCAL_NAME}) not found in TMP_DIR."
        fi

        # Check and display head of (potentially) installer script
        # Useful to see if it's an HTML error page
        if [[ -f "${TMP_DIR}/${CMAKE_INSTALLER_SCRIPT_LOCAL_NAME}" ]]; then
            echo "--- First 10 lines of downloaded installer script (${CMAKE_INSTALLER_SCRIPT_LOCAL_NAME}) ---"
            head -n 10 "${TMP_DIR}/${CMAKE_INSTALLER_SCRIPT_LOCAL_NAME}"
            echo "--- End of installer script head ---"
        elif [[ -f "${TMP_DIR}/${DERIVED_CMAKE_BINARY_FILENAME}" ]]; then
             # If it was renamed
            echo "--- First 10 lines of downloaded installer script (${DERIVED_CMAKE_BINARY_FILENAME}) ---"
            head -n 10 "${TMP_DIR}/${DERIVED_CMAKE_BINARY_FILENAME}"
            echo "--- End of installer script head ---"
        else
            echo "Installer script not found in TMP_DIR (checked for ${CMAKE_INSTALLER_SCRIPT_LOCAL_NAME} and ${DERIVED_CMAKE_BINARY_FILENAME})."
        fi

        echo "Attempting to remove temporary directory: ${TMP_DIR}"
        rm -Rf "${TMP_DIR}"
        echo "Temporary directory removed."
        echo "--- End of Cleanup ---"
    else
        echo "--- Cleanup: TMP_DIR was not set or not a directory, no temp files to inspect or clean. ---"
    fi

    # Ensure the script exits with the LAST_EXIT_CODE observed by the trap
    # unless it was 0 and the script is exiting due to an explicit non-zero exit.
    # The 'exit' command within the trap will override the script's natural exit code.
    # So, if the script was going to exit 0, but cleanup had an issue, this could change it.
    # However, for debugging an exit code 8, we want to preserve the code that *caused* the trap.
    echo "Script finished with exit code: ${LAST_EXIT_CODE}."
    exit "${LAST_EXIT_CODE}"
}
trap cleanup EXIT ERR SIGINT SIGTERM

# Immediately turn on `set -e` after trap setup
set -e

# --- Helper Functions ---
# Function to download a file with retries and basic validation
download_file() {
    local url="$1"
    local output_filename="$2"
    local attempts_left=$DOWNLOAD_ATTEMPTS
    local wget_exit_code=0

    while [ $attempts_left -gt 0 ]; do
        echo "Downloading: ${url}"
        echo "Saving to: ${TMP_DIR}/${output_filename}"
        echo "Attempt $((DOWNLOAD_ATTEMPTS - attempts_left + 1)) of ${DOWNLOAD_ATTEMPTS}..."

        # Use wget with:
        # -O: specify output file
        # --timeout: connection/read timeout
        # --tries: number of retries (wget's own retry, distinct from this loop)
        # --quiet: suppress normal output, but errors still go to stderr
        # --show-progress: if not quiet, shows a progress bar (optional)
        # Using -q for less verbose logs, but on failure, we need to know.
        wget -O "${TMP_DIR}/${output_filename}" --timeout=30 --tries=1 "${url}"
        wget_exit_code=$?

        if [ ${wget_exit_code} -eq 0 ]; then
            echo "Download command successful for ${output_filename}."
            if [ -s "${TMP_DIR}/${output_filename}" ]; then # -s: file exists and has a size greater than 0
                # Basic check for common HTML error page indicators
                # This is a heuristic and might not catch all error pages.
                if head -n 5 "${TMP_DIR}/${output_filename}" | grep -Eiq '<html|<head|<!doctype html|404 Not Found|Error'; then
                    echo "WARNING: Downloaded file '${output_filename}' might be an HTML error page."
                    echo "First 5 lines of ${output_filename}:"
                    head -n 5 "${TMP_DIR}/${output_filename}"
                    # Consider this a failure for critical files
                    return 1 # Treat as failure
                fi
                echo "File ${output_filename} downloaded and is not empty."
                return 0 # Success
            else
                echo "ERROR: Downloaded file ${output_filename} is empty."
                # No need to rm here, loop will retry or fail
            fi
        else
            echo "ERROR: wget failed to download ${url} with exit code ${wget_exit_code}."
        fi

        attempts_left=$((attempts_left - 1))
        if [ $attempts_left -gt 0 ]; then
            echo "Retrying in ${DOWNLOAD_RETRY_DELAY} seconds..."
            sleep $DOWNLOAD_RETRY_DELAY
        else
            echo "ERROR: Failed to download ${url} after ${DOWNLOAD_ATTEMPTS} attempts."
            return 1 # Explicit failure
        fi
    done
    return 1 # Should be unreachable if loop logic is correct, but as a fallback
}

# --- Main Script Logic ---

if [ "${CMAKE_VERSION}" = "none" ]; then
    echo "No CMake version specified by argument, skipping CMake reinstallation."
    exit 0
fi
echo "CMake version to install: ${CMAKE_VERSION}"

# 1. Ensure wget is available
echo "Checking for wget..."
if ! command -v wget > /dev/null; then
    echo "wget not found. Attempting to install wget via apt-get..."
    if command -v apt-get > /dev/null; then
        apt-get update -y
        apt-get install -y --no-install-recommends wget
        echo "wget installed."
    else
        echo "ERROR: apt-get not found. Cannot install wget. Please install wget manually."
        exit 1
    fi
else
    echo "wget is available."
fi

# 2. (Optional) Remove existing CMake installed via apt
echo "Attempting to remove any existing CMake installed via apt..."
if command -v apt-get > /dev/null; then
    if dpkg -s cmake &> /dev/null; then # Check if cmake package is actually installed
        apt-get -y purge --auto-remove cmake
        echo "cmake package purged."
    else
        echo "cmake package not found via dpkg, skipping purge."
    fi
else
    echo "apt-get not found, skipping removal of CMake via apt."
fi

# 3. Create installation and temporary directories
echo "Creating CMake installation directory /opt/cmake..."
mkdir -p /opt/cmake

TMP_DIR=$(mktemp -d -t cmake-install-XXXXXXXXXX)
echo "Temporary directory created: ${TMP_DIR}"
# Crucial: subsequent operations needing temp files should happen in or relative to TMP_DIR
# We will cd into TMP_DIR later, or use full paths like ${TMP_DIR}/filename

# 4. Determine system architecture
echo "Determining system architecture..."
architecture=$(dpkg --print-architecture)
case "${architecture}" in
    arm64) ARCH="aarch64" ;;
    amd64) ARCH="x86_64" ;;
    *)
        echo "ERROR: Unsupported architecture '${architecture}' reported by dpkg."
        exit 1
        ;;
esac
echo "Detected architecture: ${architecture} (mapped to CMake arch: ${ARCH})"

# 5. Define CMake download URLs and the filename expected by checksum
DERIVED_CMAKE_BINARY_FILENAME="cmake-${CMAKE_VERSION}-linux-${ARCH}.sh"
CMAKE_BINARY_URL="https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/${DERIVED_CMAKE_BINARY_FILENAME}"
CMAKE_CHECKSUM_FILE_URL="https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-SHA-256.txt"

# 6. Download CMake binary and checksum file
echo "--- Downloading Files ---"
if ! download_file "${CMAKE_BINARY_URL}" "${CMAKE_INSTALLER_SCRIPT_LOCAL_NAME}"; then
    echo "ERROR: Failed to download CMake binary installer. See messages above."
    exit 1
fi

if ! download_file "${CMAKE_CHECKSUM_FILE_URL}" "${CMAKE_CHECKSUM_FILE_LOCAL_NAME}"; then
    echo "ERROR: Failed to download CMake checksum file. See messages above."
    exit 1
fi
echo "Downloads complete."
echo "--- End of Downloading Files ---"
echo # Newline for readability

# Before checksum, rename the downloaded installer to its derived name,
# as the checksum file refers to this specific name.
echo "Renaming downloaded installer from '${CMAKE_INSTALLER_SCRIPT_LOCAL_NAME}' to '${DERIVED_CMAKE_BINARY_FILENAME}' for checksum verification."
mv "${TMP_DIR}/${CMAKE_INSTALLER_SCRIPT_LOCAL_NAME}" "${TMP_DIR}/${DERIVED_CMAKE_BINARY_FILENAME}"
if [ ! -f "${TMP_DIR}/${DERIVED_CMAKE_BINARY_FILENAME}" ]; then
    echo "ERROR: Failed to rename installer script for checksum. File '${TMP_DIR}/${DERIVED_CMAKE_BINARY_FILENAME}' does not exist after move."
    exit 1
fi

# 7. Verify checksum
echo "--- Verifying Checksum ---"
echo "Checksum file is: ${TMP_DIR}/${CMAKE_CHECKSUM_FILE_LOCAL_NAME}"
echo "Binary file to check is: ${TMP_DIR}/${DERIVED_CMAKE_BINARY_FILENAME}"

# Ensure the checksum file actually contains an entry for our binary
# This is important because the SHA-256.txt file contains checksums for *all* release assets
echo "Checking if checksum file contains entry for '${DERIVED_CMAKE_BINARY_FILENAME}'..."
if ! grep -q "${DERIVED_CMAKE_BINARY_FILENAME}" "${TMP_DIR}/${CMAKE_CHECKSUM_FILE_LOCAL_NAME}"; then
    echo "ERROR: The downloaded checksum file '${CMAKE_CHECKSUM_FILE_LOCAL_NAME}' does NOT contain an entry for '${DERIVED_CMAKE_BINARY_FILENAME}'."
    echo "This strongly suggests that the CMAKE_VERSION ('${CMAKE_VERSION}') or ARCH ('${ARCH}') is incorrect, or the specified version does not provide a .sh installer for this architecture."
    echo "Please verify the version and available files at https://github.com/Kitware/CMake/releases/tag/v${CMAKE_VERSION}"
    exit 1
fi
echo "Checksum file contains an entry for '${DERIVED_CMAKE_BINARY_FILENAME}'."

# Perform the checksum. We need to be in the directory where the files are.
echo "Changing directory to ${TMP_DIR} for checksum verification."
cd "${TMP_DIR}" # <<<<<<< IMPORTANT: sha256sum -c needs to find files

echo "Verifying checksum of '${DERIVED_CMAKE_BINARY_FILENAME}' using '${CMAKE_CHECKSUM_FILE_LOCAL_NAME}'..."
# The --ignore-missing flag is good, as the .txt file has many checksums.
# The --strict flag would cause it to error if there are improperly formatted lines.
# We rely on the grep check above to ensure our specific file is mentioned.
if sha256sum -c --ignore-missing "${CMAKE_CHECKSUM_FILE_LOCAL_NAME}"; then
    echo "Checksum verification successful for '${DERIVED_CMAKE_BINARY_FILENAME}'."
else
    SHA_EXIT_CODE=$?
    echo "ERROR: Checksum verification FAILED for '${DERIVED_CMAKE_BINARY_FILENAME}' with exit code ${SHA_EXIT_CODE}."
    # Cleanup trap will show file contents.
    exit 1 # Critical failure
fi
echo "--- End of Verifying Checksum ---"
echo # Newline for readability

# 8. Install CMake
echo "--- Installing CMake ---"
echo "Making the CMake installer script '${DERIVED_CMAKE_BINARY_FILENAME}' executable..."
chmod +x "${DERIVED_CMAKE_BINARY_FILENAME}" # Still in TMP_DIR

echo "Executing CMake installer script: ./${DERIVED_CMAKE_BINARY_FILENAME} --prefix=/opt/cmake --skip-license"
# Execute the script. If this script exits with 8, this is our culprit.
if ./"${DERIVED_CMAKE_BINARY_FILENAME}" --prefix=/opt/cmake --skip-license; then
    echo "CMake installer script executed successfully."
else
    INSTALLER_EXIT_CODE=$?
    echo "ERROR: CMake installer script FAILED with exit code ${INSTALLER_EXIT_CODE}."
    # This is the most likely place for an exit code 8 if downloads and checksums were okay.
    # The trap will handle cleanup. The script will exit with INSTALLER_EXIT_CODE due to the trap.
    exit ${INSTALLER_EXIT_CODE} # Explicitly exit with the installer's code
fi
echo "--- End of Installing CMake ---"
echo # Newline for readability

# 9. Create symlinks
echo "Creating symbolic links for cmake and ctest in /usr/local/bin/..."
ln -sf /opt/cmake/bin/cmake /usr/local/bin/cmake
ln -sf /opt/cmake/bin/ctest /usr/local/bin/ctest
echo "Symbolic links created."

echo # Newline for readability
echo "SUCCESS: CMake ${CMAKE_VERSION} installation and setup complete."

# The script will exit with 0 here. The trap will run, see $? is 0, and then exit 0.
exit 0