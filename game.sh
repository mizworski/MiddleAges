#!/bin/bash

# some arguments don't have a corresponding value to go with it such
# as in the --default example).
# note: if this is set to > 0 the /etc/hosts part is not recognized ( may be a bug )
PROGRAM="./sredniowiecze_gui_with_libs.sh"

N=10
K=100
S=1
X1=0
Y1=0
X2=0
Y2=0
H1=1
H2=1
AI1=""
AI2=""

while [[ $# > 1 ]]
do
key="$1"

case ${key} in
    -n)
    N="$2"
    shift
    ;;
    -k)
    K="$2"
    shift
    ;;
    -s)
    S="$2"
    shift
    ;;
    -p1)
    X1="$2"
    Y1="$3"
    shift
    ;;
    -p2)
    X2="$2"
    Y2="$3"
    shift
    ;;
    -ai1)
    AI1="$2"
    H1=0
    shift
    ;;
    -ai2)
    AI2="$2"
    H2=0
    shift
    ;;
    *)
            # unknown option
    ;;
esac
shift # past argument or value
done

# TODO random p1, p2

#echo N = "${N}"
#echo K = "${K}"
#echo S = "${S}"
#echo X1 = "${X1}"
#echo Y1 = "${Y1}"
#echo X2 = "${X2}"
#echo Y2 = "${Y2}"
#echo AI1 = "${AI1}"
#echo AI2 = "${AI2}"
#echo H1 = "${H1}"
#echo H2 = "${H2}"

PIPE=$(mktemp -u)
mkfifo $PIPE
exec 3<>$PIPE
rm $PIPE

PIPE=$(mktemp -u)
mkfifo $PIPE
exec 4<>$PIPE
rm $PIPE

if [ $H1 == 1 ] && [ $H2 == 1 ]; then
    $PROGRAM -human1 -human2 <&3 &
elif [ $H1 == 1 ]; then
    $PROGRAM -human1 <&3 >&4 &
elif [ $H2 == 2 ]; then
    $PROGRAM -human2 <&3 >&4 &
else
    $PROGRAM <&3 >&4 &
fi

PROGRAM_PID=$!

if [ H1 == 0 ]
then
    PIPE=$(mktemp -u)
    mkfifo $PIPE
    exec 5<>$PIPE
    rm $PIPE

    PIPE=$(mktemp -u)
    mkfifo $PIPE
    exec 6<>$PIPE
    rm $PIPE

    $AI1 <&5 >&6 &
    AI1_PID=$!
fi

if [ H2 == 0 ]
then

    PIPE=$(mktemp -u)
    mkfifo $PIPE
    exec 7<>$PIPE
    rm $PIPE

    PIPE=$(mktemp -u)
    mkfifo $PIPE
    exec 8<>$PIPE
    rm $PIPE

    $AI2 <&7 >&8 &
    AI2_PID=$!
fi

echo INIT ${N} ${K} 1 ${X1} ${Y1} ${X2} ${Y2} >&3
echo INIT ${N} ${K} 2 ${X1} ${Y1} ${X2} ${Y2} >&3

if [ $H1 == 1 ] && [ $H2 == 1 ]; then
    continue
elif [ $H1 == 1 ]; then
    while kill -0 $AI2_PID 2> /dev/null; do
        read a <&4
        echo a >&7
        read a <&8
        echo a >&3
        sleep $S
    done
elif [ $H2 == 2 ]; then
    while kill -0 $AI1_PID 2> /dev/null; do
        read a <&4
        echo a >&5
        read a <&8
        echo a >&3 >&5
        sleep $S
    done
else
    while kill -0 $AI1_PID 2> /dev/null; do
        read a <&6
        echo a >&3 >&7
        read a <&8
        echo a >&3 >&5
        sleep $S
    done
fi
