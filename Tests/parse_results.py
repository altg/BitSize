#/usr/bin/python

import string
import sys

def main(): #main function
    fh = open( 'results/run0/results.dat' , 'r' )

    data_uni ={}
    data_nonuni = {}

    start_line = fh.readline()

    field_names = start_line.split()

    for line in fh:
        if line[0] != '#':
            fields = line.split()
            if fields[field_names.index('BWMODE') - 1] == '0':
                data_uni[ fields[field_names.index('OUTPUTBW') - 1] ] = { 'LUTS' : fields[field_names.index('NLUT') - 1] ,
                                                                   'SLICES' : fields[field_names.index('NSLICES') - 1] ,
                                                                   'MULTS' : fields[field_names.index('NMULTS') - 1] , 
                                                                   'FREQ' : fields[field_names.index('MFREQ') - 1],
                                                                   'PERIOD' : fields[field_names.index('LATENCY') - 1]}
            else:
                data_nonuni[ fields[field_names.index('OUTPUTBW') - 1] ] = { 'LUTS' : fields[field_names.index('NLUT') - 1] ,
                                                                   'SLICES' : fields[field_names.index('NSLICES') - 1] ,
                                                                   'MULTS' : fields[field_names.index('NMULTS') - 1] , 
                                                                   'FREQ' : fields[field_names.index('MFREQ') - 1],
                                                                   'PERIOD' : fields[field_names.index('LATENCY') - 1]}


 
    for k, v in data_nonuni.iteritems():
        print k, v['LUTS']

    for k, v in data_uni.iteritems():
        print k, v['LUTS']
    
main()
