#!/bin/bash

echo "Entrez le nom du niveau à génerer : "
read filename

echo "Entrez la longueur du niveau à génerer : "
read length

echo "Donnez le son à utiliser en fond sonore : "
read sound

echo "Donnez le nombre d'asteroides à utiliser : "
read nb_asteroides

ast=0
while [ $ast -lt $nb_asteroides ]
do
    echo "Asteroide $ast : "
    read asteroide
    asteroides[$ast]=$asteroide
    echo "Entrez la fréquence de cet astéroide (1 = beaucoup trop, 1000 = très peu, peut aller plus loin) : "
    read freq
    freqs[$ast]=$freq
    let ast=$ast+1
done

echo "Donnez le nombre de bonus à utiliser : "
read nb_bonus

bon=0
while [ $bon -lt $nb_bonus ]
do
    echo "Bonus $bon : "
    read bonus
    bonuses[$bon]=$bonus
    echo "Entrez la fréquence de ce bonus (1 = beaucoup trop, 1000 = très peu, peut aller plus loin) : "
    read freq
    freqs_bon[$bon]=$freq
    let bon=$bon+1
done

tmpfile=`mktemp`
ast=0
while [ $ast -lt $nb_asteroides ]
do
    freq=${freqs[$ast]}
    pos=0
    while [ $pos -lt $length ]
    do
        put_ast=$(($RANDOM % $freq))
        let pos=$pos+$put_ast
        # TODO: Rendre la vitesse max des astéroides configurable.
        echo "$(($ast + 1)) $pos $(($RANDOM % 600)) $(($RANDOM%5 + 1)) $(($RANDOM%7 - 3))" >> $tmpfile
    done
    let ast=$ast+1
done

tmpfile_bonus=`mktemp`
bon=0
while [ $bon -lt $nb_bonus ]
do
    pos=0
    while [ $pos -lt $length ]
    do
        put_bon=$(($RANDOM % ${freqs_bon[$bon]}))
        let pos=$pos+$put_bon
        # TODO: Rendre la vitesse max des bonus configurable.
        echo "$(($bon+1)) $pos $(($RANDOM % 600)) $(($RANDOM%5 + 1)) $(($RANDOM%5 - 2))" >> $tmpfile_bonus
    done
    let bon=$bon+1
done

(
# OUT: longueur du niveau
echo $(($length+800))
# OUT: bande son du niveau
echo $sound
# OUT: nombre de types d'asteroides
echo $nb_asteroides
# OUT: sprites des asteroides
ast=0
while [ $ast -lt $nb_asteroides ]
do
    echo ${asteroides[$ast]}
    let ast=$ast+1
done
# OUT: nb asteroides
cat $tmpfile | wc -l
# OUT: liste des asteroides
#sort -nk 2 $tmpfile
# The sort above do not work, blank are not considered as separators ! Use this complex version instead.
cat $tmpfile | sed 's/ /;/g' | sort -nk 2 -t ";"  | sed 's/;/ /g'
# OUT: nombre de types de bonus
echo $nb_bonus
# OUT: sprites des asteroides
bon=0
while [ $bon -lt $nb_bonus ]
do
    echo ${bonuses[$bon]}
    let bon=$bon+1
done
# OUT: nb bonus
cat $tmpfile_bonus | wc -l
# OUT: liste des bonus
#sort -nk 2 $tmpfile_bonus
# The sort above do not work on my computer ! Use this complex version instead.
cat $tmpfile_bonus | sed 's/ /;/g' | sort -nk 2 -t ";"  | sed 's/;/ /g'
) > $filename

echo "$filename généré"
