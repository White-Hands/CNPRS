#!/bin/bash
i=1
for((;i<=20;i++))
do
	./time_evaluate < ../a1.param >> log_new.txt
	echo "------ $i --------" >> log_new.txt
done
