source $VO_CMS_SW_DIR/cmsset_default.sh
export target_dir=/user/dbeghin/Work/MuTauHighMass/HighMassLFVMuTau/SignalRegion_CR100
mkdir $target_dir/Arranged_data
export scratchdir=$TMPDIR
cd $scratchdir
mkdir $target_dir/Arranged_data
hadd -f data_B.root $target_dir/Out_Data_*16B*/*.root
cp *.root $target_dir/Arranged_data/
rm -f *.root