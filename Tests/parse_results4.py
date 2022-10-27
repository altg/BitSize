#!/usr/bin/python

import string
import sys

def read_from_file( test_name  , run_no ): #main function

    data = {}

    results_file_name = test_name + '/results/' + run_no + '/results.dat'

    fh = open( results_file_name , 'r' )

    start_line = fh.readline()

    field_names = start_line.split()

    
    #print field_names

    OPTTYPES = [ 'af' , 'ad' ];

    
    for line in fh:
        if line[0] != '#':
            fields = line.split()

            #print fields[ field_names.index('A_MAN_BW') - 1]

            

            opttype = fields[field_names.index('OPTTYPE') - 1]

            optbw = fields[field_names.index('OPTFRACBW') - 1]
         
            key_name = optbw + '_' + OPTTYPES[ int( opttype ) ]

            data[ key_name ] = { 'LUTS' : fields[field_names.index('NLUT') - 1] ,
                                 'ALUTS' : fields[field_names.index('NLUTWR') - 1] ,
                                 'SLICES' : fields[field_names.index('NSLICES') - 1] ,
                                 'FFS' : fields[field_names.index('NSFF') - 1] ,
                                 'MULTS' : fields[field_names.index('NMULTS') - 1] , 
                                 'FREQ' : fields[field_names.index('MFREQ') - 1],
                                 'PERIOD' : fields[field_names.index('LATENCY') - 1]}
    fh.close()

    return data

def print_data( sysdata , data , datafile ):

    optbws = {}

    for k in data.keys():
        keyfields = k.split( '_' )
        optbws[ keyfields[0] ] = 0;

    for k in optbws.keys():
        print >>datafile ,'%s_%s_%s_nonuni = [ %s %s ]' %  ( sysdata[ 'atype' ] , sysdata[ 'sysname' ] , k , data[ k + '_ad' ][ 'LUTS' ] , data[ k + '_af' ][ 'LUTS' ] )





def main():

    tests = [ 'matrix2' ]

    data_pr = {}

    sysdata = {}

    run_no = 'run1'

    bwopt_data = {}

    sysdata[ 'atype' ] = sys.argv[1]

    for t in tests:
        print "Reading from %s .. " % ( t ),
        data_pr[ t ] = read_from_file( t , run_no )
        #bwopt_data[ t ] = read_bwopt_file( t )
        #sysdata[ 'atype' ] = bwopt_data[ t ]['stat'][ 'a_type' ]
        print "done"
        
        
  ##  for k, v in data.iteritems():
##        print k , ' : ', v['LUTS'

    pfh = open( 'matdata2.m' , 'w' )
        
    for t in tests:
        sysdata[ 'sysname' ] = t
        #sysdata[ 'atype' ] = sys.argv[1]
        print_data( sysdata , data_pr[ t ] , pfh )
        #print_optdata( t , bwopt_data )
          


main()
