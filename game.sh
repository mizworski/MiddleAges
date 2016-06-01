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
        exit 1        # unknown option
    ;;
esac
shift # past argument or value
done



if [ ! $X1 -eq 0 ] && [ ! $X2 -eq 0 ]; then
    if [  $X1 - $X2  -lt 8 ] && [  $X2 - $X1  -lt 8 ]; then
        if [ $Y1 - $Y2 -lt 8 ] && [ $Y2 - $Y1 -lt 8 ]; then
            exit 1
        fi
    fi
elif [ ! $X1 -eq 0 ]; then
    if [ $N - $X1 -lt 8 ] && [ $X1 -lt 9 ] && [ $N - $Y1 -lt 8 ] && [ $Y1 -lt 9 ]; then
        exit 1
    fi
elif [ ! $X2 -eq 0 ]; then
    if [ $N - $X2 -lt 8 ] && [ $X2 -lt 9 ] && [ $N - $Y2 -lt 8 ] && [ $Y2 -lt 9 ]; then
        exit 1
    fi
fi

#if [ $X1 -eq 0 ] && [ $X2 -eq 0 ]; then
#    X1=$[ ( $RANDOM % $N )  + 1 ]
#    Y1=$[ ( $RANDOM % ($N - 3) )  + 1 ]
#    X2=$[ $X1 + 8 + ( $RANDOM % ($N - 8) ) ]
#    Y2=$[ ( $RANDOM % ($N - 3) )  + 1 ]
#elif [ $X1 -eq 0 ]; then
#    X1=$[ $X2 + 8 + ( $RANDOM % ($N - 8) ) ]
#    Y1=$[ ( $RANDOM % ($N - 3) )  + 1 ]
#elif [ $X2 -eq 0 ]; then
#    X2=$[ $X1 + 8 + ( $RANDOM % ($N - 8) ) ]
#    Y2=$[ ( $RANDOM % ($N - 3) )  + 1 ]
#fi

if [ $X1 -eq 0 ] && [ $X2 -eq 0 ]; then
    A=$[ $RANDOM % 2 ]
    B=$[ $RANDOM % 2 ]
    C=$[ $RANDOM % 2 ]
#    D1=$[ 1 + ( $RANDOM % $N ) ]
#    D2=$[ 8 + ( $RANDOM % ($N - 8) ) ]
    if [ "$A" -eq 0 ] && [ "$N" -gt 11 ]; then
        D1=$[ 8 + ( $RANDOM % ( $N - ( 11 ) ) ) ]
        D2=$[ 1 + ( $RANDOM % $N ) ]
    else
        D1=$[ 1 + ( $RANDOM % ( $N - 4 ) ) ]
        D2=$[ 8 + ( $RANDOM % ( $N - 8 ) ) ]
    fi
    P1=$[ 1 + ( $RANDOM % ( $N - ( $D1 + 3 ) ) ) ]
    P2=$[ $P1 + $D1 ]
    Q1=$[ 1 + ( $RANDOM % ( $N - $D2 ) ) ]
    Q2=$[ $Q1 + $D2 ]

    if [ "$B" -eq 0 ] && [ "$C" -eq 0 ]; then
        X1=$P1
        X2=$P2
        Y1=$Q1
        Y2=$Q2
    fi
    if [ "$B" -eq 1 ] && [ "$C" -eq 0 ]; then
        X1=$P2
        X2=$P1
        Y1=$Q1
        Y2=$Q2
    fi
    if [ "$B" -eq 0 ] && [ "$C" -eq 1 ]; then
        X1=$P1
        X2=$P2
        Y1=$Q2
        Y2=$Q1
    fi
    if [ "$B" -eq 1 ] && [ "$C" -eq 1 ]; then
        X1=$P2
        X2=$P1
        Y1=$Q2
        Y2=$Q1
    fi
fi

#if [ $X1 -eq 0 ]; then
#    if [[ $X2 -gt 8 ]] || [[ $X2 -le $N - 11 ]]; then
#        if [[ $Y2 -gt 8 ]] || [[ $Y2 -le $N - 8 ]]; then
#            A=$[ $RANDOM % 2 ]
#            if [[ $A -eq 0 ]]; then
#
#if [ $X1 -eq 0 ]; then
#    if [[ $X2 -gt 8 ]]; then
#        if [[ $X2 -le $N - 11 ]]; then
#            if [[ $Y2 -gt 8 ]]; then
#                if [[ $Y2 -le $N - 8 ]]; then
#                    A=$[ $RANDOM % 2 ]
#                    if [[ $A -eq 0 ]]; then
#                        B=$[ 1 + $RANDOM % ( $N - (17 + 3) ) ]
#                        if [[ $B -le $X2 - 8 ]]; then
#                            X1=$B
#                        else
#                            X1=$[ $B + 15 ]
#                        fi
#                        Y1=$[ 1 + $RANDOM % N ]
#                    else #A -eq 1
#                        B=$[ 1 + $RANDOM % ( $N - 17 ) ]
#                        if [[ $B -le $Y2 - 8 ]]; then
#                            Y1=$B
#                        else
#                            Y1=$[ $B + 15 ]
#                        fi
#                        X1=$[ 1 + $RANDOM % (N - 3) ]
#                    fi









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
    while [[ -e /proc/$AI2_PID ]]; do
        read -t 1 a <&4
        while [[ ! $a == "END_TURN" ]] && [[ ! $a == "" ]]; do
            if [[ ! $a == "" ]]; then
                echo $a >&7
            fi
#            if [[ -e /proc/$AI2_PID ]]; then
            if [[ ! $a == "" ]]; then
                read -t 1 a <&4
            fi
        done
        if [[ ! $a == "" ]]; then
                echo $a >&7
        fi

        read -t 1 a <&8
        while [[ ! $a == "END_TURN" ]] && [[ ! $a == "" ]]; do
            if [[ ! $a == "" ]]; then
                echo $a >&3
            fi
#            if [[ -e /proc/$AI2_PID ]]; then
            if [[ ! $a == "" ]]; then
                read -t 1 a <&8
            fi
        done
        if [[ ! $a == "" ]]; then
                echo $a >&3
        fi
        sleep $S
    done
elif [[ $H2 -eq 1 ]]; then
    while [[ -e /proc/$AI1_PID ]]; do
        read -t 1 a <&6
        while [[ ! $a == "END_TURN" ]] && [[ -e /proc/$AI1_PID ]] && [[ ! $a == "" ]]; do
            if [[ ! $a == "" ]]; then
                echo $a >&3
            fi
#            if [[ -e /proc/$AI1_PID ]]; then
            if [[ ! $a == "" ]]; then
                read -t 1 a <&6
            fi
        done
        if [[ ! $a == "" ]]; then
                echo $a >&3
        fi


#        if [[ -e /proc/$AI1_PID ]]; then
            read -t 1 a <&4
#        fi
        while [[ ! $a == "END_TURN" ]]  && [[ ! $a == "" ]]; do
            if [[ ! $a == "" ]]; then
                echo $a >&5
            fi
#            if [[ -e /proc/$AI1_PID ]]; then
            if [[ ! $a == "" ]]; then
                read -t 1 a <&4
            fi
        done
        if [[ ! $a == "" ]]; then
                echo $a >&5
        fi
        sleep $S
    done
else
#    while kill -0 $AI1_PID 2> /dev/null || kill -0 $AI2_PID 2> /dev/null; do
    while kill -0 $PROGRAM_PID 2> /dev/null; do
        read -t 1 a <&6
        while [[ ! $a == "END_TURN" ]] && [[ ! $a == "" ]]; do
           if [[ ! $a == "" ]]; then
                echo $a >&3
                echo $a >&7
           fi
#           if [[ -e /proc/$AI1_PID ]]; then
           if [[ ! $a == "" ]]; then
                read -t 1 a <&6
           fi
        done
        if [[ ! $a == "" ]]; then
                echo $a >&3
                echo $a >&7
        fi
        sleep $S

#        if [[ -e /proc/$AI2_PID ]]; then
#        if [[ ! $a == "" ]]; then
        read -t 1 b <&8
#        fi
        while [[ ! $b == "END_TURN" ]]  && [[ ! $b == "" ]]; do
            if [[ ! $b == "" ]]; then
                echo $b >&3
                echo $b >&5
            fi
            #if [[ -e /proc/$AI2_PID ]]; then
            if [[ ! $b == "" ]]; then
                read -t 1 b <&8
            fi
        done
        if [[ ! $b == "" ]]; then
                echo $b >&3
                echo $b >&5
        fi
#        if kill -0 $AI2_PID 2> /dev/null; then
#            echo jeszcze zyje
#        fi
        sleep $S
    done
fi

if kill -0 $PROGRAM_PID 2> /dev/null; then
    kill $PROGRAM_PID
fi

if kill -0 $AI1_PID 2> /dev/null; then
    kill $AI1_PID
fi

if kill -0 $AI2_PID 2> /dev/null; then
    kill $AI2_PID
fi
