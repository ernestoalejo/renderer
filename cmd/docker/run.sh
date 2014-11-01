#!/bin/bash

docker run -v $(pwd)/src:/renderer/src -i -t ernestoalejo/renderer
