
function stdred() (
    set -o pipefail;
    ("$@" 2>&1>&3 | sed $'s,.*,\e[31m&\e[m,' >&2) 3>&1 \
                  | sed $'s,.*,\e[32m&\e[m,'
)

export GOROOT=/go
export GOPATH=/goprojects

export PATH=$PATH:$GOROOT/bin:$GOPATH/bin
