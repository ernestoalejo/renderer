#!/bin/bash

docker run -v $(pwd)/src:/prerender/src -i -t ernestoalejo/renderer
