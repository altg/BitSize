#!/usr/bin/python

import string
import sys

def read_from_file( run_no , data ): #main function

    results_file_name = 'results/' + run_no + '/results.dat'

    fh = open( results_file_name , 'r' )

   

    start_line = fh.readline()

    field_names = start_line.split()

    
    #print field_names

    for line in fh:
        if line[0] != '#':
            fields = line.split()

            #print fields[ field_names.index('A_MAN_BW') - 1]

            A_man_bw = fields[field_names.index('A_MAN_BW') - 1]

            A_exp_bw = fields[field_names.index('A_EXP_BW') - 1]

            key_name = run_no + '_' + A_man_bw + '_' + A_exp_bw

            data[ key_name ] = {   'B_MAN_BW' : B_man_bw ,
                                 'B_EXP_BW' : B_exp_bw ,
                                 'A_MAN_BW' : A_man_bw ,
                                 'A_EXP_BW' : A_exp_bw ,
                                 'LUTS' : fields[field_names.index('NLUT') - 1] ,
                                 'SLICES' : fields[field_names.index('NSLICES') - 1] ,
                                 'MULTS' : fields[field_names.index('NMULTS') - 1] , 
                                 'FREQ' : fields[field_names.index('MFREQ') - 1],
                                 'PERIOD' : fields[field_names.index('LATENCY') - 1]}
    fh.close()
    

def main():

    data = {}

    run_table = [ [ '18' , '6'  ],
                  [ '20' , '6'  ],
                  [ '22' , '6'  ],
                  [ '18' , '8'  ],
                  [ '20' , '8'  ],
                  [ '22' , '8'  ],
                  [ '18' , '10'  ],
                  [ '20' , '10'  ],
                  [ '22' , '10'  ]]


    ctr = 0;
    for dd in run_table:
        read_from_file( dd[0] , dd[1] , 'run' + `ctr`  , data )
        ctr += 1
        
   
    fhout = open( 'matdata.m' , 'w' )

    print >> fhout, 'in_data = [ '

    for k, v in data.iteritems():
        print >> fhout, v['A_MAN_BW' ] , v[ 'A_EXP_BW' ] ,  v['B_MAN_BW' ] , v[ 'B_EXP_BW' ] , 1 , ';'

    print >> fhout, '];'

    print >> fhout, 'out_data = [ '

    for k, v in data.iteritems():
        print >> fhout, v['LUTS' ],';'

    print >> fhout, '];'
    
    
    fhout.close()

    #read_from_file( 20 , 6 , 'results/run1/results.dat' );

    

main()
