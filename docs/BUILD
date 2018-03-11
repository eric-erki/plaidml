# Copyright Vertex.AI.

package(default_visibility = ["//visibility:public"])

genrule(
    name = "docs",
    srcs = [
        "conf.py",
        "requirements.txt",
    ] + glob(
        ["**/*.rst"],
        exclude = ["venv/**"],
    ),
    outs = ["docs.zip"],
    cmd = """
        CWD=`pwd`
        cd $$(dirname $(location conf.py))
        virtualenv --clear venv
        VIRTUAL_ENV_DISABLE_PROMPT=1
        if [ -e 'venv/bin/activate' ]; then
          . venv/bin/activate
        elif [ -e 'venv/Scripts/activate' ]; then
          . venv/Scripts/activate
        else
          echo 'Unable to enter virtual python environment'
          exit 1
        fi
        python -m pip install -r requirements.txt
        python -m sphinx -a -E -b html . _build/html
        cd _build/html
        zip -q -r $$CWD/$@ ./
    """,
    local = 1,
)
