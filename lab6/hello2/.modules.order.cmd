cmd_/home/minhmdl/lab6/hello2/modules.order := {   echo /home/minhmdl/lab6/hello2/hello2.ko; :; } | awk '!x[$$0]++' - > /home/minhmdl/lab6/hello2/modules.order
