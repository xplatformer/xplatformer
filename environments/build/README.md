# Build Environment for Windows

The build environment can be built from the repository root with the following command:

```bash
docker build --file environments/build/Dockerfile --tag ubuntu-xrdp environments/build/.
docker run --rm --name=xplatformer -d -v "//${PWD}":/workspace -e FRX_XRDP_USER_NAME=jrbeverly -p 33890:3389 ubuntu-xrdp
```

The environment can then be logged into with the credentials `jrbeverly:ChangeMe`.
