#!/bin/bash

PROGRAM=bin/thor
WORKSPACE=/tmp/thor.$(id -u)
FAILURES=0

error() {
    echo "$@"
    [ -r $WORKSPACE/test ] && (echo; cat $WORKSPACE/test; echo)
    FAILURES=$((FAILURES + 1))
}

cleanup() {
    STATUS=${1:-$FAILURES}
    rm -fr $WORKSPACE
    exit $STATUS
}

check_md5sums() {
    python3 <<EOF
import sys

stdout_curl = sorted(open('$WORKSPACE/stdout.curl', 'rb').read())
stdout_thor = sorted(open('$WORKSPACE/stdout.thor', 'rb').read())

sys.exit(0 if stdout_curl == stdout_thor else 1)
EOF
}

check_metrics() {
    python3 <<EOF
import sys

bandwidth    = []
elapsed_time = None
for line in open('$WORKSPACE/stderr.thor'):
    if line.startswith('Bandwidth'):
        bandwidth.append(float(line.split()[1]))
    elif line.startswith('Elapsed'):
        elapsed_time = float(line.split()[2])

if len(bandwidth) == $HAMMERS \\
   and ($BANDWIDTH_MIN <= sum(bandwidth)/len(bandwidth) <= $BANDWIDTH_MAX) \\
   and ($ELAPSED_TIME_MIN <= elapsed_time <= $ELAPSED_TIME_MAX):
    sys.exit(0)
else:
    sys.exit(1)
EOF
}

check_hammers() {
    python3 <<EOF
import sys

hammers = sum(1 for line in open('$WORKSPACE/stderr.thor') if 'clone' in line)
sys.exit(0 if hammers == $HAMMERS else 1)
EOF

}

# Tests -----------------------------------------------------------------------

export LD_LIBRARY_PATH=$LD_LIBRRARY_PATH:.

mkdir $WORKSPACE

trap "cleanup" EXIT
trap "cleanup 1" INT TERM

echo "Testing thor..."

if [ ! -x $PROGRAM ]; then
    echo "Failure: $PROGRAM is not executable!"
    exit 1
fi

# Usage -----------------------------------------------------------------------

printf " %-50s ... " "thor"
$PROGRAM &> $WORKSPACE/test
if [ $? -eq 0 ] || ! grep -q -i 'usage' $WORKSPACE/test; then
    error "Failure"
else
    echo "Success"
fi

printf " %-50s ... " "thor -h"
$PROGRAM -h &> $WORKSPACE/test
if [ $? -ne 0 ] || ! grep -q -i 'usage' $WORKSPACE/test; then
    error "Failure"
else
    echo "Success"
fi

printf " %-50s ... " "thor -?"
$PROGRAM -? &> $WORKSPACE/test
if [ $? -eq 0 ] || ! grep -q -i 'usage' $WORKSPACE/test; then
    error "Failure"
else
    echo "Success"
fi

rm -f $WORKSPACE/test

# http://nd.edu ---------------------------------------------------------------

URL=http://nd.edu
BANDWIDTH_MIN=0.00
BANDWIDTH_MAX=0.00
ELAPSED_TIME_MIN=0.04
ELAPSED_TIME_MAX=0.08
HAMMERS=1

printf " %-50s ... " "thor $URL"
strace -e clone $PROGRAM $URL > $WORKSPACE/stdout.thor 2> $WORKSPACE/stderr.thor
if [ $? -eq 0 ] ; then
    error "Failure (Exit Status)"
elif [ -s $WORKSPACE/stdout.thor ] ; then
    error "Failure (Contents)"
elif ! check_hammers; then
    error "Failure (Hammers)"
else
    echo "Success"
fi

sleep 1

printf " %-50s ... " "thor -n $HAMMERS $URL"
strace -e clone $PROGRAM -n $HAMMERS $URL > $WORKSPACE/stdout.thor 2> $WORKSPACE/stderr.thor
if [ $? -eq 0 ] ; then
    error "Failure (Exit Status)"
elif [ -s $WORKSPACE/stdout.thor ] ; then
    error "Failure (Contents)"
elif ! check_hammers; then
    error "Failure (Hammers)"
else
    echo "Success"
fi

# http://example.com -----------------------------------------------------------

URL=http://example.com
BANDWIDTH_MIN=0.02
BANDWIDTH_MAX=0.07
ELAPSED_TIME_MIN=0.01
ELAPSED_TIME_MAX=0.04
HAMMERS=1

printf " %-50s ... " "thor $URL"
curl $URL > $WORKSPACE/stdout.curl 2> /dev/null
strace -e clone $PROGRAM $URL > $WORKSPACE/stdout.thor 2> $WORKSPACE/stderr.thor
if [ $? -ne 0 ] ; then
    error "Failure (Exit Status)"
elif ! check_md5sums; then
    error "Failure (Contents)"
elif ! check_metrics; then
    error "Failure (Metrics)"
elif ! check_hammers; then
    error "Failure (Hammers)"
else
    echo "Success"
fi

sleep 1

HAMMERS=2
printf " %-50s ... " "thor -n $HAMMERS $URL"
rm -f $WORKSPACE/stdout.curl
for i in $(seq $HAMMERS); do
    curl $URL >> $WORKSPACE/stdout.curl 2> /dev/null
done
strace -e clone $PROGRAM -n $HAMMERS $URL > $WORKSPACE/stdout.thor 2> $WORKSPACE/stderr.thor
if [ $? -ne 0 ] ; then
    error "Failure (Exit Status)"
elif ! check_md5sums; then
    error "Failure (Contents)"
elif ! check_metrics; then
    error "Failure (Metrics)"
elif ! check_hammers; then
    error "Failure (Hammers)"
else
    echo "Success"
fi

# http://h4x0r.space:9898/appa.png ---------------------------------------------

URL=http://h4x0r.space:9898/appa.png
BANDWIDTH_MIN=4.0
BANDWIDTH_MAX=6.0
ELAPSED_TIME_MIN=0.2
ELAPSED_TIME_MAX=0.6
HAMMERS=1

printf " %-50s ... " "thor $URL"
curl $URL > $WORKSPACE/stdout.curl 2> /dev/null
strace -e clone $PROGRAM $URL > $WORKSPACE/stdout.thor 2> $WORKSPACE/stderr.thor
if [ $? -ne 0 ] ; then
    error "Failure (Exit Status)"
elif ! check_md5sums; then
    error "Failure (Contents)"
elif ! check_metrics; then
    error "Failure (Metrics)"
elif ! check_hammers; then
    error "Failure (Hammers)"
else
    echo "Success"
fi

sleep 1

HAMMERS=3
BANDWIDTH_MIN=2.0
BANDWIDTH_MAX=6.0
ELAPSED_TIME_MIN=0.25
ELAPSED_TIME_MAX=0.75
printf " %-50s ... " "thor -n $HAMMERS $URL"
rm -f $WORKSPACE/stdout.curl
for i in $(seq $HAMMERS); do
    curl $URL >> $WORKSPACE/stdout.curl 2> /dev/null
done
strace -e clone $PROGRAM -n $HAMMERS $URL > $WORKSPACE/stdout.thor 2> $WORKSPACE/stderr.thor
if [ $? -ne 0 ] ; then
    error "Failure (Exit Status)"
elif ! check_md5sums; then
    error "Failure (Contents)"
elif ! check_metrics; then
    error "Failure (Metrics)"
elif ! check_hammers; then
    error "Failure (Hammers)"
else
    echo "Success"
fi

# h4x0r.space:9898/walden.txt --------------------------------------------------

URL=h4x0r.space:9898/walden.txt
BANDWIDTH_MIN=1.0
BANDWIDTH_MAX=3.0
ELAPSED_TIME_MIN=0.15
ELAPSED_TIME_MAX=0.35
HAMMERS=1

printf " %-50s ... " "thor $URL"
curl $URL > $WORKSPACE/stdout.curl 2> /dev/null
strace -e clone $PROGRAM $URL > $WORKSPACE/stdout.thor 2> $WORKSPACE/stderr.thor
if [ $? -ne 0 ] ; then
    error "Failure (Exit Status)"
elif ! check_md5sums; then
    error "Failure (Contents)"
elif ! check_metrics; then
    error "Failure (Metrics)"
elif ! check_hammers; then
    error "Failure (Hammers)"
else
    echo "Success"
fi

sleep 1

HAMMERS=4
ELAPSED_TIME_MIN=0.15
ELAPSED_TIME_MAX=0.45
printf " %-50s ... " "thor -n $HAMMERS $URL"
rm -f $WORKSPACE/stdout.curl
for i in $(seq $HAMMERS); do
    curl $URL >> $WORKSPACE/stdout.curl 2> /dev/null
done
strace -e clone $PROGRAM -n $HAMMERS $URL > $WORKSPACE/stdout.thor 2> $WORKSPACE/stderr.thor
if [ $? -ne 0 ] ; then
    error "Failure (Exit Status)"
elif ! check_md5sums; then
    error "Failure (Contents)"
elif ! check_metrics; then
    error "Failure (Metrics)"
elif ! check_hammers; then
    error "Failure (Hammers)"
else
    echo "Success"
fi

# Summary ---------------------------------------------------------------------

TESTS=$(($(grep -c Success $0) - 1))

echo
echo "   Score $(echo "scale=4; ($TESTS - $FAILURES) / $TESTS.0 * 7.0" | bc | awk '{ printf "%0.2f\n", $1 }' )"
echo
