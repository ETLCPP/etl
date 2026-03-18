#!/bin/bash

set -e

docker run -it --rm -v .:/home/developer/etl -u developer -w /home/developer/etl etl /bin/bash
