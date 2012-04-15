#
#
# 2dx_genergeMergeMap_SubVolume.com
#
#   -----------------------------------------------------------------------------------
#   ... This is not an independent script. It should only be called from other scripts.
#   -----------------------------------------------------------------------------------
#
#
# This calculates sub-volumes from the raw volume SCRATCH/scratch1.map.
#
# Please note that the raw volume mentioned above still has the wrong handedness. 
# This script therefore needs to flip the Z-axis down, which in example 1 below is done
# wiht the last entry for the ROTA MATRIX, which is (0,0,-1) instead of (0,0,1).
#
# If you edit your own version into one of the templates below, make sure to include the 
# Z-axis flipping somewhere, to get a volume with the correct handedness.
#
#
#
#
#
#############################################################################
${proc_2dx}/linblock "SubVolume generation Script called, with option ${calculate_subvolume}"
#############################################################################
#
if ( ${calculate_subvolume}x == "1x" ) then  
  #
  #############################################################################
  ${proc_2dx}/linblock "maprot - to rotate volue for sub-volume preparation"
  #############################################################################
  #
  set pcos = "0.85090352"
  set psin = "0.52532199"
  set mcos = -${pcos}
  set msin = -${psin}
  #
  \rm -f SCRATCH/scratch1b.map
  ${bin_ccp4}/maprot mapin SCRATCH/scratch1.map wrkout SCRATCH/scratch1b.map << eot
MODE FROM
CELL WORK ${realcell} ${ALAT} 90.0 90.0 ${realang}
GRID WORK ${cellx} ${celly} ${ALAT}
XYZLIM 0 ${cellxm1} 0 ${cellym1} 0 ${ALATm1}
SYMM WORK 1
AVER
ROTA POLAR 0.0 0.0 45.0
TRANS  0.0 0.0 0.0
eot
  #
  #############################################################################
  ${proc_2dx}/linblock "maprot - to flip Z-axis down, for correct handedness"
  #############################################################################
  #
  \rm -f SCRATCH/rot_volume.map
  ${bin_ccp4}/maprot mapin SCRATCH/scratch1b.map wrkout SCRATCH/rot_volume.map << eot
MODE FROM
CELL WORK ${realcell} ${ALAT} 90.0 90.0 ${realang}
GRID WORK ${cellx} ${celly} ${ALAT}
XYZLIM 0 ${cellxm1} 0 ${cellym1} 0 ${ALATm1}
SYMM WORK 1
AVER
ROTA MATRIX 1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 -1.0
TRANS  0.0 0.0 0.0
eot
  #
  # echo "# IMAGE: SCRATCH/rot_volume.map <MAP: rotated 3D Volume>" >> LOGS/${scriptname}.results
  #
  #############################################################################
  ${proc_2dx}/linblock "mapmask - to cut sub-volume"
  #############################################################################
  #   
  # 0.7071 = 1/sqrt(2)
  set limx = "0.70710678"
  #
  \rm -f volume_sub.map
  ${bin_ccp4}/mapmask mapin SCRATCH/rot_volume mapout volume_sub.map << eof
AXIS X,Y,Z
scale factor 1
xyzlim 0.0 ${limx} 0.0 ${limx} -0.5 0.5
END
eof
  #
  echo "# IMAGE-IMPORTANT: volume_sub.map <MAP: Final 3D Sub Volume>" >> LOGS/${scriptname}.results
  #
endif
#
#
#
#
#
#
#############################################################################
#############################################################################
#############################################################################
if ( ${calculate_subvolume}x == "2x" ) then  
  #############################################################################
  #############################################################################
  #############################################################################
  #
  #############################################################################
  ${proc_2dx}/linblock "Not yet implemented"
  #############################################################################
  #
endif
#
#
#
#
#
#
#############################################################################
#############################################################################
#############################################################################
if ( ${calculate_subvolume}x == "3x" ) then  
  #############################################################################
  #############################################################################
  #############################################################################
  #
  #############################################################################
  ${proc_2dx}/linblock "Not yet implemented"
  #############################################################################
  #
endif
#
#
#
#
#
#
#############################################################################
#############################################################################
#############################################################################
if ( ${calculate_subvolume}x == "4x" ) then  
  #############################################################################
  #############################################################################
  #############################################################################
  #
  #############################################################################
  ${proc_2dx}/linblock "Not yet implemented"
  #############################################################################
  #
endif
#
#
#
#
#
#
#############################################################################
#############################################################################
#############################################################################
if ( ${calculate_subvolume}x == "5x" ) then  
  #############################################################################
  #############################################################################
  #############################################################################
  #
  #############################################################################
  ${proc_2dx}/linblock "Not yet implemented"
  #############################################################################
  #
endif
#
#
#
#
#
