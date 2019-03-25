#!/bin/bash
set -e

[ $# -eq 2 ]
base=1000000000
size=10000
bin="$1"
name="$2"

[ -e $bin ]
[ -e log ] || mkdir log
[ -e log/$name ] || mkdir log/$name

make tester.jar

seq $size | parallel '
    seed=$(('${base}' + {}))
    echo seed = ${seed}
    java -jar tester.jar -exec '${bin}' -seed ${seed} -novis -json > log/'${name}'/${seed}.json 2>/dev/null
    echo
'

cat log/${name}/*.json | jq --slurp '[
    ([ .[] | .details.edgeScore / ([ .edges[] | .[3] ] | add) ] | add / length),
    ([ .[] | .details.routeScore / ([ .routes[] | .[2] ] | add) ] | add / length),
    ([ .[] | select (.score == 0) ] | length),
]'
