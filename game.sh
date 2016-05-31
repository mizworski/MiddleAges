#!/bin/bash -x

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
    X1=${2%,*}
    Y1=${2#*,}
    shift
    ;;
    -p2)
    X2=${2%,*}
    Y2=${2#*,}
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

if [ $X1 -eq 0 ] && [ X2 -eq 0 ]; then
    X1=$[ ( $RANDOM % N )  + 1 ]
    Y1=$[ ( $RANDOM % (N - 3) )  + 1 ]
    X2=$[ X1 + 8 + ( $RANDOM % (N - 8) ) ]
    Y2=$[ ( $RANDOM % (N - 3) )  + 1 ]
elif [ $X1 -eq 0 ]; then
    X1=$[ X2 + 8 + ( $RANDOM % (N - 8) ) ]
    Y1=$[ ( $RANDOM % (N - 3) )  + 1 ]
elif [ $X2 -eq 0 ]; then
    X2=$[ X1 + 8 + ( $RANDOM % (N - 8) ) ]
    Y2=$[ ( $RANDOM % (N - 3) )  + 1 ]
fi

PIPE=$(mktemp -u)
mkfifo $PIPE
exec 3<>$PIPE
rm $PIPE

PIPE=$(mktemp -u)
mkfifo $PIPE
exec 4<>$PIPE
rm $PIPE

PIPE=$(mktemp -u)
mkfifo $PIPE
exec 5<>$PIPE
rm $PIPE

PIPE=$(mktemp -u)
mkfifo $PIPE
exec 6<>$PIPE
rm $PIPE

PIPE=$(mktemp -u)
mkfifo $PIPE
exec 7<>$PIPE
rm $PIPE

PIPE=$(mktemp -u)
mkfifo $PIPE
exec 8<>$PIPE
rm $PIPE

if [[ $H1 -eq 1 ]] && [[ $H2 -eq 1 ]]; then
    $PROGRAM -human1 -human2 <&3 >&4 &
elif [[ $H1 -eq 1 ]]; then
    $PROGRAM -human1 <&3 >&4 &
elif [[ $H2 -eq 1 ]]; then
    $PROGRAM -human2 <&3 >&4 &
else
    $PROGRAM <&3 >&4 &
fi

PROGRAM_PID=$!

if [[ H1 -eq 0 ]]
then
    $AI1 <&5 >&6 &
    AI1_PID=$!
fi

if [[ H2 -eq 0 ]]; then
    $AI2 <&7 >&8 &
    AI2_PID=$!
fi

echo INIT ${N} ${K} 1 ${X1} ${Y1} ${X2} ${Y2} >&3
echo INIT ${N} ${K} 1 ${X1} ${Y1} ${X2} ${Y2} >&5
echo INIT ${N} ${K} 2 ${X1} ${Y1} ${X2} ${Y2} >&3
echo INIT ${N} ${K} 2 ${X1} ${Y1} ${X2} ${Y2} >&7

if [[ $H1 -eq 1 ]] && [[ $H2 -eq 1 ]]; then
    while kill -0 $PROGRAM_PID 2> /dev/null; do
        :
    done
elif [[ $H1 -eq 1 ]]; then
    while kill -0 $AI2_PID 2> /dev/null; do
        read a <&4
        while [[ ! $a == "END_TURN" ]]; do
            echo $a >&3
            echo $a >&7
            read a <&4
        done
        echo $a >&7
        echo $a >&3

        read a <&8
        while [[ ! $a == "END_TURN" ]]; do
            echo $a >&3
            read a <&8
        done
        echo $a >&3
        sleep $S
    done
elif [[ $H2 -eq 1 ]]; then
    while kill -0 $AI1_PID 2> /dev/null; do
        read a <&6
        while [[ ! $a == "END_TURN" ]]; do
            read a <&6
            echo $a >&3
        done
        echo $a >&3

        read a <&4
        while [[ ! $a == "END_TURN" ]]; do
            echo $a >&5
            echo $a >&3
            read a <&4
        done
        echo $a >&3
        echo $a >&5
        sleep $S
    done
else
    while kill -0 $AI1_PID 2> /dev/null; do
        read a <&6
        while [[ ! $a == "END_TURN" ]]; do
            echo $a >&3
            echo $a >&7
            read a <&6
        done
        echo $a >&3
        echo $a >&7
        sleep $S

        read a <&8
        while [[ ! $a == "END_TURN" ]]; do
            echo $a >&3
            echo $a >&5
            read a <&8
        done
        echo $a >&3
        echo $a >&5
        sleep $S
    done
fi

if kill -0 $PROGRAM_PID 2> /dev/null; then
    kill $PROGRAM_PID
fi

if kill -0 $AI1_PID 2> /dev/null; then
    kill AI1_PID
fi

if kill -0 $AI2_PID 2> /dev/null; then
    kill AI2_PID
fi
