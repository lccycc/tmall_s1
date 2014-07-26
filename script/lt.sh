awk '$4 > 118 && $3==0{a[$1" "$2]+=$5;} END{for(i in a)if(a[i]>3)print i}' data/input/tian.lan |awk '!a[$0]++' >rest && wc -l rest
