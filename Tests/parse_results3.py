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

    OPTMODES = [ 'uni' , 'nonuni' ];
    

    for line in fh:
        if line[0] != '#':
            fields = line.split()

            #print fields[ field_names.index('A_MAN_BW') - 1]

            opttype = fields[field_names.index('OPTMODE') - 1]

            optmode = fields[field_names.index('OPTTYPE') - 1]
         
            key_name = OPTTYPES[ int( opttype ) ] + '_' + OPTMODES[ int( optmode ) ]

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

    print >>datafile, '%s_%s_uni = [ %s  %s ]' % ( sysdata[ 'atype' ] , sysdata[ 'sysname' ] , data[ 'ad_uni' ][ 'LUTS' ] , data[ 'af_uni' ][ 'LUTS' ] )

    print >>datafile, '%s_%s_nonuni = [ %s  %s ]' % ( sysdata[ 'atype' ] , sysdata[ 'sysname' ]  , data[ 'ad_nonuni' ][ 'LUTS' ] , data[ 'af_nonuni' ][ 'LUTS' ] )


def print_optdata( s , data  ):

    stat_data = data[ s ][ 'stat' ]
    dyn_data = data[ s ][ 'dyn' ]

    print  'opt_%s_%s_uni = [ %s  %s ]' % ( stat_data[ 'a_type' ] , s , stat_data[ 'uni' ] , dyn_data[ 'uni' ] )

    print 'opt_%s_%s_nonuni = [ %s  %s ]' % ( stat_data[ 'a_type' ] , s , stat_data[ 'nonuni' ] , dyn_data[ 'nonuni' ] )


def read_bwopt_file( test_name ):

    opt_data = {}
    
    bwopt_file_name = test_name + '/bwopt.dat'

    fh = open( bwopt_file_name )

    for line in fh:
        fields = line.split( ':' )

            
        if fields[ 4 ].strip() == 'sim' and fields[5].strip() == 'ad':
            op_type = 'dyn'
        else:
            op_type = 'stat'
                   
        opt_data[ op_type ] = { 'a_type' : fields[0].strip() , 'uni' : fields[1] , 'nonuni' : fields[2] }

    return opt_data


def main():

    tests = [ 'matrix2' , 'bspline' , 'dct8' , 'rgb2ycc' ]

    data_pr = {}

    sysdata = {}

    run_no = 'run0'

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

    pfh = open( 'matdata.m' , 'w' )
        
    for t in tests:
        sysdata[ 'sysname' ] = t
        #sysdata[ 'atype' ] = sys.argv[1]
        print_data( sysdata , data_pr[ t ] , pfh )
        #print_optdata( t , bwopt_data )
          


main()
