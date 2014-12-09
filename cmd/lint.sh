#!/bin/bash

cpplint.py --root=src --filter=-runtime/references $(find src -name \*.h -or -name \*.cc)
