#!/bin/bash
#
# Create docker image for development environment and enter container
#
# Run from project root directory
#

set -e

# Verify script is running from project root
if [ ! -d ".devcontainer" ]; then
    echo "Error: This script must be run from the project root directory." >&2
    echo "The .devcontainer directory was not found." >&2
    exit 1
fi

docker build -t etl .devcontainer
docker run -it --rm -v "$(pwd)":/home/vscode/etl -u vscode -w /home/vscode/etl etl /bin/bash
