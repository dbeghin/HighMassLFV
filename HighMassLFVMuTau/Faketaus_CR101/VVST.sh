source $VO_CMS_SW_DIR/cmsset_default.sh
export target_dir=/user/dbeghin/Work/MuTauHighMass/HighMassLFVMuTau/Faketaus_CR101
mkdir $target_dir/Arranged_WW
export scratchdir=$TMPDIR
cd $scratchdir
mkdir $target_dir/Arranged_WW
hadd -f WW_inclusive.root $target_dir/Out_WWTo2L2Nu_13*/Con*.root 
hadd -f WW_200to600.root $target_dir/Out_WW*200To600*/Con*WW*.root 
hadd -f WW_600to1200.root $target_dir/Out_WW*600To1200*/Con*WW*.root 
hadd -f WW_1200to2500.root $target_dir/Out_WW*1200To2500*/Con*WW*.root 
hadd -f WW_2500toInf.root $target_dir/Out_WW*2500ToInf*/Con*WW*.root 
cp *.root $target_dir/Arranged_WW/
rm -f *.root

mkdir $target_dir/Arranged_ZZ
hadd -f ZZ.root $target_dir/Out_ZZ*/Con*ZZ*.root 
cp *.root $target_dir/Arranged_ZZ/
rm -f *.root


mkdir $target_dir/Arranged_WZ
hadd -f WZ.root $target_dir/Out_WZ*/Con*WZ*.root 
cp *.root $target_dir/Arranged_WZ/
rm -f *.root


mkdir $target_dir/Arranged_ST
hadd -f ST.root $target_dir/Out_ST*/Con*.root 
cp *.root $target_dir/Arranged_ST/
rm -f *.root
