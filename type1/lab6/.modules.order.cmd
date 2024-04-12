cmd_/home/minhmdl/kernel/modules.order := {   echo /home/minhmdl/kernel/hello1.ko; :; } | awk '!x[$$0]++' - > /home/minhmdl/kernel/modules.order
