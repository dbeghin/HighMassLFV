source $VO_CMS_SW_DIR/cmsset_default.sh
export target_dir=/user/dbeghin/Work/MuTauHighMass/HighMassLFVMuTau/SignalRegion_CR100
mkdir $target_dir/Arranged_data
export scratchdir=$TMPDIR
cd $scratchdir
mkdir $target_dir/Arranged_data
hadd -f data_B.root $target_dir/Out_Data_*16B*/*.root
hadd -f data_C.root $target_dir/Out_Data_*16C*/*.root
hadd -f data_D.root $target_dir/Out_Data_*16D*/*.root
hadd -f data_E.root $target_dir/Out_Data_*16E*/*.root
hadd -f data_F.root $target_dir/Out_Data_*16F*/*.root
hadd -f data_G.root $target_dir/Out_Data_*16G*/*.root
hadd -f data_H.root $target_dir/Out_Data_*16H*/*.root
hadd -f data.root data_*.root 
cp *.root $target_dir/Arranged_data/
rm -f *.root