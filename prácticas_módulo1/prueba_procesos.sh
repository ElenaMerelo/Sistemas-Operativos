#!/bin/bash

max=10

for (( i=1; i<$1; i++ ))
do
    suma=$(($max+$i))
done

echo fin $0 con pid $$, suma vale $suma, i vale $i
