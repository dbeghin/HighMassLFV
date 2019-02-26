source $VO_CMS_SW_DIR/cmsset_default.sh
export target_dir=/user/dbeghin/Work/MuTauHighMass/HighMassLFVMuTau/Faketaus_CR101
mkdir $target_dir/Arranged_DY
export scratchdir=$TMPDIR
cd $scratchdir
hadd -f DY_inclusive.root $target_dir/Out_DYJetsToLL_M-50_*/Con*.root 
hadd -f DY_400to500.root $target_dir/Out_DY*400to500*/Con*.root 
hadd -f DY_500to700.root $target_dir/Out_DY*500to700*/Con*.root 
hadd -f DY_700to800.root $target_dir/Out_DY*700to800*/Con*.root 
hadd -f DY_800to1000.root $target_dir/Out_DY*800to1000*/Con*.root 
hadd -f DY_1000to1500.root $target_dir/Out_DY*1000to1500*/Con*.root 
hadd -f DY_1500to2000.root $target_dir/Out_DY*1500to2000*/Con*.root 
hadd -f DY_2000to3000.root $target_dir/Out_DY*2000to3000*/Con*.root 
cp *.root $target_dir/Arranged_DY/
rm -f *.root