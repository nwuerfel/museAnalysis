who has time for MD?

This is a repo for my trash attempt at making a general analysis 
assistant for muse target simulation analysis

The idea is to write an "analysisManager" which handles common analysis
tasks such as parsing TFiles and reading TTree's and making cuts

The analysisManager will be instantiated by a "master" program such as 
crossSection, which determines which cuts/ branches are needed for this 
particular analysis.

The motivation for this is that most of the work between making, say, 
purity and efficiency calculations, is identical to figuring out cross
sections/ knowing what happened during reconstruction of simulation data.

so, crossSection and purity will both call the analysisManager but use 
its utilities differently


The Analysis Manager:

    
