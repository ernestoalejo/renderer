#!/bin/bash

cpplint.py --root=src $(find src -name \*.h -or -name \*.cc)
