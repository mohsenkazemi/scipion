#!/usr/bin/env python
#------------------------------------------------------------------------------------------------
# Protocol for Xmipp-based ml3d classification, according to:
# {please cite} Scheres et al. (2007) Nature Methods, 4, 27-29
#
# Example use:
# ./protocol_ml3d.py
#
# Author: Sjors Scheres, March 2007
#
#------------------------------------------------------------------------------------------------
# {section} Global parameters
#------------------------------------------------------------------------------------------------
# {file} Selfile with the input images:
InSelFile="all_images.sel"
# {file} Initial 3D reference map:
InitialReference="my_ref.vol"
# Working subdirectory:
WorkingDir="ML3D/test1"
# Delete working subdirectory if it already exists?
DoDeleteWorkingDir=False
# {expert} Root directory name for this project:
""" Absolute path to the root directory for this project
"""
ProjectDir="/home2/bioinfo/scheres/work/protocols/G40P"
# {expert} Directory name for logfiles:
LogDir="Logs"
#------------------------------------------------------------------------------------------------
# {section} Correct absolute grey scale of initial reference
#------------------------------------------------------------------------------------------------
# Correct the absolute grey-scale of the initial reference map?
""" The probabilities are based on squared differences, so that the absolute grey scale is important.
"""
DoCorrectGreyScale=False
# {expert} Angular sampling for a quick projection matching to obtain right grey scale
""" As the resolution of the intial reference should be low, this sampling can be relatively crude, e.g. 15
"""
ProjMatchSampling=15
# {expert} Threshold for a quick weighted back-projection to obtain right grey scale
""" As the resolution of the intial reference should be low, this value can be relatively high, e.g. 0.02
"""
WbpThreshold=0.02
#------------------------------------------------------------------------------------------------
# {section} Low-pass filter initial reference?
#------------------------------------------------------------------------------------------------
# Low-pass filter the initial reference?
""" It is highly recommended to low-pass filter your initial reference volume as much as you can.
"""
DoLowPassFilterReference=True
# Resolution of the low-pass filter (in Angstroms):
LowPassFilter=50
# Pixel size (in Angstroms):
PixelSize=5.6
#------------------------------------------------------------------------------------------------
# {section} Seed generation
#------------------------------------------------------------------------------------------------
# Generate unbiased seeds from a single initial reference map?
DoGenerateSeeds=True
# Number of seeds to be generated (and later on used in ml3d-classification):
NumberOfReferences=3
# {expert} {file} Alternatively, provide a selfile with user-defined seeds:
""" Automated (unbiased!) seed generation is highly recommended...
    But you may use this option to provide the seeds from a previous run
    The seeds should already be on the correct absolute greyscale!
"""
SeedsSelfile=""
#------------------------------------------------------------------------------------------------
# {section} ML3D classification
#------------------------------------------------------------------------------------------------
# Perform ml3d classification run?
DoML3DClassification=True
# Angular sampling for ml3d classification:
""" Fine samplings take huge amounts of CPU and memory.
    Therefore, in general, dont use samplings finer than 10 degrees.
"""
AngularSampling=10
# Number of ml3d iterations to perform:
NumberOfIterations=25
# {file} Symmetry description file:
""" See http://xmipp.cnb.uam.es/twiki/bin/view/Xmipp/Symmetry
    for a description of the symmetry file format
    dont give anything, if no symmetry is present
"""
SymmetryFile="/home2/bioinfo/scheres/work/protocols/G40P/6fold.sym"
# {expert} Additional xmipp_ml_refine3d parameters:
""" For a complete description see the manual pages:
    http://xmipp.cnb.csic.es/twiki/bin/view/Xmipp/MLrefine3D
"""
ExtraParamsMLrefine3D=""
#------------------------------------------------------------------------------------------------
# {section} Parallelization issues
#------------------------------------------------------------------------------------------------
# Use multiple processors in parallel?
DoParallel=True
# Number of processors to use:
MyNumberOfCPUs=5
# {file} A list of all available CPUs (the MPI-machinefile):
""" Depending on your system, your standard script to launch MPI-jobs may require this
    if your queueing system using an environment variable, give it here (with the leading $, e.g. $PBS_NODEFILE
"""
MyMachineFile="/home2/bioinfo/scheres/machines.dat"
#------------------------------------------------------------------------------------------------
# {expert} Analysis of results
""" This script serves only for GUI-assisted visualization of the results
"""
AnalysisScript="visualize_ml3d.py"
#------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------
# {end-of-header} USUALLY YOU DO NOT NEED TO MODIFY ANYTHING BELOW THIS LINE ...
#------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------
#
class ML3D_class:

    #init variables
    def __init__(self,
                 InSelFile,
                 WorkingDir,
                 DoDeleteWorkingDir,
                 ProjectDir,
                 LogDir,
                 DoCorrectGreyScale,
                 ProjMatchSampling,
                 WbpThreshold,
                 DoLowPassFilterReference,
                 LowPassFilter,
                 PixelSize,
                 DoGenerateSeeds,
                 NumberOfReferences,
                 InitialReference,
                 DoML3DClassification,
                 AngularSampling,
                 NumberOfIterations,
                 SymmetryFile,
                 ExtraParamsMLrefine3D,
                 SeedsSelfile,
                 DoParallel,
                 MyNumberOfCPUs,
                 MyMachineFile):
	     
        import os,sys,shutil
        scriptdir=os.path.split(os.path.dirname(os.popen('which xmipp_protocols','r').read()))[0]+'/protocols'
        sys.path.append(scriptdir) # add default search path
        import log,selfile

        self.WorkingDir=WorkingDir
        self.ProjectDir=ProjectDir
        self.NumberOfReferences=NumberOfReferences
        self.DoGenerateSeeds=DoGenerateSeeds
        self.InitialReference=os.path.abspath(InitialReference)
        if (len(SeedsSelfile)>0):
            self.SeedsSelfile=os.path.abspath(SeedsSelfile)
        else:
            self.SeedsSelfile=SeedsSelfile
        self.AngularSampling=AngularSampling
        self.LowPassFilter=LowPassFilter
        self.PixelSize=PixelSize
        self.NumberOfIterations=NumberOfIterations
        if (len(SymmetryFile)>0):
            self.SymmetryFile=os.path.abspath(SymmetryFile)
        else:
            self.SymmetryFile=SymmetryFile
        self.ExtraParamsMLrefine3D=ExtraParamsMLrefine3D
        self.ProjMatchSampling=ProjMatchSampling
        self.WbpThreshold=WbpThreshold
        self.DoParallel=DoParallel
        self.MyNumberOfCPUs=MyNumberOfCPUs
        if (MyMachineFile[0]=="$"):
            self.MyMachineFile=MyMachineFile
        else:
            self.MyMachineFile=os.path.abspath(MyMachineFile)

        # Setup logging
        self.log=log.init_log_system(self.ProjectDir,
                                     LogDir,
                                     sys.argv[0],
                                     self.WorkingDir)

        # Delete working directory if it exists, make a new one
        if (DoDeleteWorkingDir): 
            if os.path.exists(self.WorkingDir):
                shutil.rmtree(self.WorkingDir)
        if not os.path.exists(self.WorkingDir):
            os.makedirs(self.WorkingDir)

        # Create a selfile with absolute pathname in the WorkingDir
        mysel=selfile.selfile()
        mysel.read(InSelFile)
        newsel=mysel.make_abspath()
        self.InSelFile=os.path.abspath(self.WorkingDir+'/'+InSelFile)
        newsel.write(self.InSelFile)

        # Backup script
        log.make_backup_of_script_file(sys.argv[0],
                                       os.path.abspath(self.WorkingDir))
    
        # Execute MLalign2D in the working directory
        os.chdir(self.WorkingDir)

        self.copy_initial_refs()
        
        if DoCorrectGreyScale:
            self.correct_greyscale()

        if DoLowPassFilterReference:
            self.filter_reference()

        if DoGenerateSeeds:
            self.generate_seeds()

        if DoML3DClassification:
            self.execute_ML3D_classification()
        
        # Return to parent dir
        os.chdir(os.pardir)

    # Make a copy of the initial references:
    def copy_initial_refs(self):
        import os,shutil
        if os.path.exists(self.InitialReference):
            shutil.copy(self.InitialReference,'initial_reference.vol')
        if (self.DoGenerateSeeds==False):
            fh=open(self.SeedsSelfile,'r')
            lines=fh.readlines()
            fh.close()
            i = 0
            newlines=[]
            for line in lines:
                i = i + 1
                oname='initial_seed'+str(i).zfill(5)+'.vol'
                words=line.split()
                if words[0][0]=="/":
                    shutil.copy(words[0],oname)
                else:
                    dirname=os.path.dirname(self.SeedsSelfile)
                    iname=dirname+'/'+words[0]
                    shutil.copy(iname,oname)
                newlines.append(oname+' 1\n')
            fh=open('ml3d_seeds.sel','w')
            fh.writelines(newlines)
            fh.close()

    # Crude correction of grey-scale, by performing a single iteration of projection matching and wbp
    def correct_greyscale(self):
        import os
        import launch_parallel_job
        print '*********************************************************************'
        print '*  Correcting absolute grey scale of initial reference:'

        dirname='CorrectGreyscale/'
        if not os.path.exists(dirname):
            os.makedirs(dirname)

        # A single cycle of projection matching
        basename='corrected_reference'
        params= ' -i '    + str(self.InSelFile) + \
                ' -o '    + dirname+basename + \
                ' -vol '  + str(self.InitialReference) + \
                ' -sam ' + str(ProjMatchSampling)
        if not self.SymmetryFile=="":
            params+= ' -sym '+str(self.SymmetryFile)
        params+=' -dont_modify_header -output_classes -output_refs'
                
        launch_parallel_job.launch_job(self.DoParallel,
                                       "xmipp_angular_projection_matching",
                                       "xmipp_mpi_angular_projection_matching",
                                       params,
                                       self.log,
                                       self.MyNumberOfCPUs,
                                       self.MyMachineFile,
                                       False)

        # Followed by a weighted back-projection reconstruction
        iname=dirname+basename+'_classes.sel'
        outname=basename+'.vol'
        params= ' -i '    + str(iname) + \
                ' -o '    + str(outname) + \
                ' -threshold '+str(self.WbpThreshold) + \
                '  -use_each_image -weight '
        if not self.SymmetryFile=="":
            params+= ' -sym '+str(self.SymmetryFile)
           
        launch_parallel_job.launch_job(self.DoParallel,
                                       "xmipp_reconstruct_wbp",
                                       "xmipp_mpi_reconstruct_wbp",
                                       params,
                                       self.log,
                                       self.MyNumberOfCPUs,
                                       self.MyMachineFile,
                                       False)

        return outname

    # Low-pass filter
    def filter_reference(self):
        import os
        print '*********************************************************************'
        print '*  Low-pass filtering of the initial reference:'


        corrvol='corrected_reference.vol'
        if os.path.exists(corrvol):
            reference=corrvol
        else:
            reference=self.InitialReference
        command='xmipp_fourier_filter -o filtered_reference.vol' + \
                 ' -i ' + reference  + \
                 ' -sampling ' + str(self.PixelSize) + \
                 ' -low_pass ' + str(self.LowPassFilter)

        print '* ',command
        self.log.info(command)
        os.system(command)

    # Splits selfile and performs a single cycle of ML3D-classification for each subset
    def generate_seeds(self):
        import os
        import selfile
        print '*********************************************************************'
        print '*  Generating seeds:' 
        newsel=selfile.selfile()

        # Split selfiles
        command='xmipp_selfile_split -o seeds_split'+ \
                 ' -i ' + str(self.InSelFile) + \
                 ' -n ' + str(self.NumberOfReferences) +' \n'
        print '* ',command
        self.log.info(command)
        os.system(command)

        
        filvol='filtered_reference.vol'
        corrvol='corrected_reference.vol'
        if os.path.exists(filvol):
            reference=filvol
        elif os.path.exists(corrvol):
            reference=corrvol
        else:
            reference='initial_reference.vol'

        # Launch MLrefine3D with output to subdirectories
        for i in range(self.NumberOfReferences):
            inselfile='seeds_split_'+str(i+1)+'.sel'
            dirname='GenerateSeed_'+str(i+1)+'/'
            if not os.path.exists(dirname):
                os.makedirs(dirname)
            outname=dirname+'seeds_split_'+str(i+1)
            self.execute_MLrefine3D(inselfile,
                                    outname,
                                    reference,
                                    self.AngularSampling,
                                    1,
                                    self.SymmetryFile,
                                    self.ExtraParamsMLrefine3D)
            newsel.insert(outname+'_it00001.vol','1')
        newsel.write('ml3d_seeds.sel')


    # Perform the actual classification
    def execute_ML3D_classification(self):
        import os
        import launch_parallel_job
 
        dirname='RunML3D/'
        if not os.path.exists(dirname):
            os.makedirs(dirname)

        self.execute_MLrefine3D(self.InSelFile,
                                dirname+'ml3d',
                                'ml3d_seeds.sel',
                                self.AngularSampling,
                                self.NumberOfIterations,
                                self.SymmetryFile,
                                self.ExtraParamsMLrefine3D)


    # Either for seeds generation or for ML3D-classification
    def execute_MLrefine3D(self,inselfile,outname,volname,sampling,iter,symfile,extraparam):
        import os
        import launch_parallel_job

        print '*********************************************************************'
        print '*  Executing ml_refine3d program :' 
        params= ' -i '    + str(inselfile) + \
                ' -o '    + str(outname) + \
                ' -vol '  + str(volname) + \
                ' -iter ' + str(iter) + \
                ' -ang '  + str(sampling)
        if not symfile=="":
            params+= ' -sym '+str(symfile)
        params+=' '+extraparam

        launch_parallel_job.launch_job(self.DoParallel,
                                       "xmipp_ml_refine3d",
                                       "xmipp_mpi_ml_refine3d",
                                       params,
                                       self.log,
                                       self.MyNumberOfCPUs,
                                       self.MyMachineFile,
                                       False)

    def close(self):
        message='Done!'
        print '*',message
        print '*********************************************************************'

#		
# Main
#     
if __name__ == '__main__':

    ML3D=ML3D_class(InSelFile,
                    WorkingDir,
                    DoDeleteWorkingDir,
                    ProjectDir,
                    LogDir,
                    DoCorrectGreyScale,
                    ProjMatchSampling,
                    WbpThreshold,
                    DoLowPassFilterReference,
                    LowPassFilter,
                    PixelSize,
                    DoGenerateSeeds,
                    NumberOfReferences,
                    InitialReference,
                    DoML3DClassification,
                    AngularSampling,
                    NumberOfIterations,
                    SymmetryFile,
                    ExtraParamsMLrefine3D,
                    SeedsSelfile,
                    DoParallel,
                    MyNumberOfCPUs,
                    MyMachineFile)

    # close 
    ML3D.close()

