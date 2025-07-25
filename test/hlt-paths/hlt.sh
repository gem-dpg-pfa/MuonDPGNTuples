odir=$1

rm -rf $odir
mkdir $odir
cd $odir

#hltGetConfiguration run:394959 \
hltGetConfiguration /dev/CMSSW_15_0_0/GRun \
   --globaltag 150X_dataRun3_HLT_v1 \
   --data \
   --unprescale \
   --output all \
   --max-events 200 \
   --eras Run3_2025 --l1-emulator uGT --l1 L1Menu_Collisions2025_v1_2_0_xml \
   --input /store/data/Run2025D/EphemeralHLTPhysics0/RAW/v1/000/394/959/00000/02ab3d20-66ba-4372-8f06-5d09e0848408.root \
   > hlt.py

cmsRun hlt.py #&> hlt.log

#--paths *RPC* \

cd -
