
renderer
========

> CEF based application to render pages for SEO and Karma unit tests.


Resources
---------

 * [Precompiled binaries](https://googledrive.com/host/0B-unE3_O4B0uSWFYdEJZa1hEQmc/precompiled)
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

Once we're inside the docker image:

```shell
scons && bin/renderer -flagfile flags/karma
```


Prepare the packages
--------------------

```shell
cmd/pack/linux64.sh 0.1.0
```

Then the package should be uploaded to Drive.
