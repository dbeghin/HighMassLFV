qsub -q express@cream02 -o DY.stdout -e DY.stderr -l walltime=00:20:00  DY.sh
qsub -q express@cream02 -o TT.stdout -e TT.stderr -l walltime=00:20:00  TT.sh
qsub -q express@cream02 -o VVST.stdout -e VVST.stderr -l walltime=00:20:00  VVST.sh
qsub -q express@cream02 -o Data.stdout -e Data.stderr -l walltime=00:20:00  Data.sh
