#!/bin/bash

bin/seo-generator -flagfile flags/seo-generator | bin/seo -flagfile flags/seo | bin/seo-reader -flagfile flags/seo-reader
