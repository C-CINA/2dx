/* 
 * @license GNU Public License
 * @author Nikhil Biyani (nikhilbiyani@gmail.com)
 * 
 */

#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "../include/2dx_volume_processing.h"

/*
 * A Universal volume processor. 
 */
int main(int argc, char* argv[]) 
{
    args::Executable exe("A universal 2D crystallography volume processor.", ' ', "1.0" );
    
    //Add arguments  
    exe.add(args::templates::MRCOUT);
    exe.add(args::templates::HKLOUT);
    exe.add(args::templates::NORMALIZE_GREY);
    exe.add(args::templates::SPREAD_FOURIER);
    exe.add(args::templates::FULL_FOURIER);
    exe.add(args::templates::SUBSAMPLE);
    exe.add(args::templates::PSF);
    exe.add(args::templates::INVERTED);
    exe.add(args::templates::EXTENDED);
    exe.add(args::templates::THRESHOLD);
    exe.add(args::templates::MAXAMP);
    exe.add(args::templates::MAXRES);
    exe.add(args::templates::SYMMETRY);
    exe.add(args::templates::GAMMA);
    exe.add(args::templates::NZ);
    exe.add(args::templates::NY);
    exe.add(args::templates::NX);
    
    
    std::vector<args::Arg*> infileArgs = {&args::templates::HKZIN, &args::templates::HKLIN, &args::templates::MRCIN};
    exe.xorAdd(infileArgs);
    
    //Parse the arguments
    exe.parse(argc, argv);
    
    //Get and check the variables
    std::string infile;
    std::string informat;
    if(args::templates::MRCIN.isSet())
    {
        infile = args::templates::MRCIN.getValue();
        informat = volume::utilities::filesystem::FileExtension(infile);
    }
    else if(args::templates::HKLIN.isSet())
    {
        infile = args::templates::HKLIN.getValue();
        informat = "hkl";
    }
    else if(args::templates::HKZIN.isSet())
    {
        infile = args::templates::HKZIN.getValue();
        informat = "hkz";
    }
    
    if(!(args::templates::HKLOUT.isSet()) && !(args::templates::MRCOUT.isSet()))
    {
        std::cerr << "\n\nERROR: Please specify at least one output with hklout or mrcout!\n";
        std::cerr << "\nFor full details type:\n\t" << exe.getProgramName() << " --help \n\n\n";
        exit(1);
    }
    
    if((args::templates::HKLIN.isSet() || args::templates::HKZIN.isSet()) && (!(args::templates::NX.isSet()) || !(args::templates::NY.isSet()) || !(args::templates::NZ.isSet())))
    {
        std::cerr << "\n\nERROR: Please specify nx, ny, nz with the option hklin/hkzin!\n";
        std::cerr << "\nFor full details type:\n\t" << exe.getProgramName() << " --help \n\n\n";
        exit(1);
    }
    
    //Prepare the input
    Volume2dx input((int)args::templates::NX.getValue(), (int)args::templates::NY.getValue(), (int)args::templates::NZ.getValue());
    if(args::templates::GAMMA.isSet()) input.set_gamma_degrees(args::templates::GAMMA.getValue());
    
    input.read_volume(infile, informat);
    
    //Would need to overwrite gamma from mrc header
    if(args::templates::GAMMA.isSet()) input.set_gamma_degrees(args::templates::GAMMA.getValue());
    
    if(args::templates::SPREAD_FOURIER.getValue()) input = input.spread_fourier_data();
    
    if(args::templates::MAXRES.isSet()) input.low_pass(args::templates::MAXRES.getValue());
    if(args::templates::SYMMETRY.isSet())
    {
        input.set_symmetry(args::templates::SYMMETRY.getValue());
        input.symmetrize();
    }
    
    if(args::templates::MAXAMP.isSet()) input.rescale_to_max_amplitude(args::templates::MAXAMP.getValue());
    if(args::templates::THRESHOLD.isSet()) input.apply_density_threshold(args::templates::THRESHOLD.getValue());
    
    if(args::templates::EXTENDED.getValue()) input = input.extended_volume(1,1,0);
    if(args::templates::INVERTED.getValue()) input.invert_hand();
    
    if(args::templates::PSF.getValue())
    {
        std::cout << "Creating PSF\n";
        input = input.zero_phases();
        input.centerize_density_along_xyz();
        input.grey_scale_densities();
    }
    
    if(args::templates::SUBSAMPLE.isSet()) input = input.subsample(args::templates::SUBSAMPLE.getValue());
    
    if(args::templates::FULL_FOURIER.getValue()) input.extend_to_full_fourier();
    
    if(args::templates::NORMALIZE_GREY.getValue()) input.grey_scale_densities();
    
    if(args::templates::HKLOUT.isSet()) input.write_volume(args::templates::HKLOUT.getValue(), "hkl");
    if(args::templates::MRCOUT.isSet()) input.write_volume(args::templates::MRCOUT.getValue());
    
    return 0;
}

