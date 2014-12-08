
renderer
========

> CEF based application to render pages for SEO and Karma unit tests.


Resources
---------

 * [Precompiled releases](https://github.com/ernestoalejo/renderer/releases)
 * [Karma plugin](https://github.com/ernestoalejo/karma-renderer-launcher)
 * [Wrapper for NodeJS](https://github.com/ernestoalejo/cef-renderer)



Docker image for development
----------------------------

Build the image:

```shell
cmd/docker/build.sh
```

Run the image:

```shell
cmd/docker/run.sh
```


Build & run dev app
-------------------

Once we're inside the docker image we can build the binaries:

```shell
scons
```

Test the karma app:

```shell
bin/karma -flagfile flags/karma
```

Or test the seo app:

```shell
cmd/run/test-seo.sh
```


Prepare the packages
--------------------

```shell
cmd/pack/linux64.sh VERSION
```

where *VERSION* uses semver.

Then the package should be uploaded to a new release of Github.
