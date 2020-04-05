set -e
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

docker build --file "${DIR}/Dockerfile" --tag ubuntu-xrdp "${DIR}/."
docker run --rm --name=xplatformer -d -v "//${PWD}":/workspace -e FRX_XRDP_USER_NAME=jrbeverly -p 33890:3389 ubuntu-xrdp
