source $VO_CMS_SW_DIR/cmsset_default.sh
export target_dir=/user/dbeghin/Work/MuTauHighMass/HighMassLFVMuTau/Faketaus_CR101
mkdir $target_dir/Arranged_TT
export scratchdir=$TMPDIR
cd $scratchdir
mkdir $target_dir/Arranged_TT
hadd -f TT_inclusive.root $target_dir/Out_TTTo2L2Nu*/Con*.root
hadd -f TT_500to800.root $target_dir/Out_TT*500To800*/Con*.root 
hadd -f TT_800to1200.root $target_dir/Out_TT*800To1200*/Con*.root 
hadd -f TT_1200to1800.root $target_dir/Out_TT*1200To1800*/Con*.root 
hadd -f TT_1800toInf.root $target_dir/Out_TT*1800ToInf*/Con*.root 
cp *.root $target_dir/Arranged_TT/
rm -f *.root