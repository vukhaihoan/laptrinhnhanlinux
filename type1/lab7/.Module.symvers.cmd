cmd_/home/minhmdl/lab7/Module.symvers := sed 's/\.ko$$/\.o/' /home/minhmdl/lab7/modules.order | scripts/mod/modpost -m -a  -o /home/minhmdl/lab7/Module.symvers -e -i Module.symvers   -T -
