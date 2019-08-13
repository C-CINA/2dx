# This script will generate a .star file given a list of input FOCUS image directories
#
set star_file_path = "${1}"
set img_list = "${2}"
set KV = "${3}"
set CS = "${4}"
set phacon = "${5}"
set ampcon = ` echo "scale=3; sqrt( 1 - ${phacon} * ${phacon} )" | bc `
#
#--------------------------------------------------------------------------------------
${proc_2dx}/linblock "StarFileGenerator: to generate a STAR file from selected images."
#--------------------------------------------------------------------------------------
#
#

if ( -e ${star_file_path} ) then
	\rm ${star_file_path}
endif

cat > ${star_file_path} << eot

data_

loop_
_rlnImageName #1
_rlnMicrographName #2
_rlnMagnification #3 
_rlnDetectorPixelSize #4
_rlnDefocusU #5
_rlnDefocusV #6
_rlnDefocusAngle #7
_rlnVoltage #8
_rlnSphericalAberration #9
_rlnAmplitudeContrast #10
_rlnPhaseShift #11
_rlnAngleRot #12
_rlnAngleTilt #13 
_rlnAnglePsi #14 
_rlnOriginX #15 
_rlnOriginY #16
eot

# cat ${img_list}
set tot_ptcls = 0
foreach img ( `cat ${img_list}` )
        # echo ":Working on ${img}"

	set orimic = `grep imagename_original ../${img}/2dx_image.cfg | awk '{print $4}' | tr -d '"' | awk -F \/ '{print $NF}' `
	set len = `echo ${orimic} | wc -c`
	@ len -= 1
	if ( ${len} <= 1 ) then
		set orimic = `grep "set raw_gaincorrectedstack =" ../${img}/2dx_image.cfg | awk '{print $4}' | tr -d '"'` 
	endif
	# echo ${orimic}
	if (${orimic:e} == "") set orimic = ${orimic}.mrc
	set orimicbase = `basename ${orimic} .mrc | sed 's/_raw//g'`
	
	if ( -e ../${img}/${orimicbase}_particles_prep.star) then

		echo ": ../${img}/${orimicbase}_particles_prep.star found"
		# echo ": ${img}"

		# Get the number of preprocessed particles from this image:
		set nptcls = `tail -n +6 ../${img}/${orimicbase}_particles_prep.star | wc -l`
		# echo ":: ${nptcls}"
		# Remove the last empty line generated by relion_preprocess:
		@ nptcls -= 1
		set defocus = `grep "set defocus =" ../${img}/2dx_image.cfg | awk '{print $4}' | tr -d '"'`
		set defU = `echo ${defocus} | awk -F , '{print $1}'`
		set defV = `echo ${defocus} | awk -F , '{print $2}'`
		set ast = `echo ${defocus} | awk -F , '{print $3}'`
		set phaseshift = `grep defocus_phase_shift ../${img}/2dx_image.cfg | awk '{print $4}' | tr -d '"'`
		set stepdigitizer = `grep "set stepdigitizer =" ../${img}/2dx_image.cfg | awk '{print $4}' | tr -d '"'`
		set magnification = `grep "set magnification =" ../${img}/2dx_image.cfg | awk '{print $4}' | tr -d '"'`

		set i = 1
		while ( $i <= ${nptcls} )
			set iprint = `printf %.8d ${i}`
			echo "${iprint}@${PWD}/../${img}/${orimicbase}_particles_prep.mrcs  ${orimic} ${magnification} ${stepdigitizer} ${defU}  ${defV}  ${ast}  ${KV}  ${CS}  ${ampcon}  ${phaseshift}  0.0  0.0  0.0  0.0  0.0" >> ${star_file_path}
			@ i += 1
			@ tot_ptcls += 1
		end

	else
		echo ": ../${img}/${orimicbase}_particles_prep.star not found"
	endif

end
echo ":: "
echo ":: ${tot_ptcls} particles written to ${star_file_path}."
echo ":: "

exit
