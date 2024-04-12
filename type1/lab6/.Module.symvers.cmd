cmd_/home/minhmdl/kernel/Module.symvers := sed 's/\.ko$$/\.o/' /home/minhmdl/kernel/modules.order | scripts/mod/modpost -m -a  -o /home/minhmdl/kernel/Module.symvers -e -i Module.symvers   -T -
