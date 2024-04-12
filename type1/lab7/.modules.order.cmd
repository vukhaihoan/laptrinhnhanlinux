cmd_/home/minhmdl/lab7/modules.order := {   echo /home/minhmdl/lab7/main.ko; :; } | awk '!x[$$0]++' - > /home/minhmdl/lab7/modules.order
