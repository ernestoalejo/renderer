#!/bin/bash

set -e
set -u

docker build -t ernestoalejo/renderer .
docker run -i -t ernestoalejo/renderer bash
