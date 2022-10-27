#!/usr/bin/perl -w
#use Data::Dumper;

#$test_file = "mul_25x25.edn";

#$sig_test = run_core_parse( $test_file );

#print Dumper( $sig_test );

sub run_core_parse{

my $core_name = shift(@_);


@file_data = open_file_core( $core_name );

$port_data = get_port_data( \@file_data );

my $sig_inst = find_int_sigs( \@file_data , \$port_data , $core_name  );

return $sig_inst;

}

#print Dumper( $sig_inst );

sub open_file_core{

my @lines;

my $core_fname = shift(@_);

open(FILE, $core_fname ) or die "Can't open $core_fname : $!\n";
@lines = <FILE>;
close(FILE);

@lines;

}


sub get_port_data{

my $lines = shift(@_);

my %port_data = ();

@port_decs = grep{/\(port \( array/} @$lines;

foreach (@port_decs){

  @port_data_rec = split( " " );

  $port_data_rec[10] =~ s/\)//g;

  $port_data{$port_data_rec[5]} = {
				   "p_type" => $port_data_rec[10],
				   "p_size" => $port_data_rec[7],
				    };
}


return \%port_data;

}

sub find_int_sigs{

my $lines = shift(@_);

my $p_data = shift(@_);

my $core_fname = shift(@_);

print "$core_fname\n";

my $p_data2 = ();


my %sig_inst = ();
#my $sig_data = shift(@_);

#while( my ($k, $v) = each %$$p_data ) {


#  foreach (0 .. $v->{p_size} -1 ){

#    $port_name = $k."($_".")";
#    #print $port_name."\n";
#    @decs = qx[grep -A 3 "$port_name" $ARGV[0]];

#    @inst_data = split( ' ', $decs[3] );

#    $p_data2{$port_name}  = $inst_data[3];
#    #print Dumper( @decs );
#  }
#}


foreach ( keys %$$p_data ){

  if ( $_ eq 'o' ){
  $grep_exp = $_."\\([0-9]+\\)";

  push( @decs , qx[egrep -A 3 '$grep_exp' $core_fname] );

  #print Dumper( @decs );
  #print "$grep_exp \n";
}
}

#print Dumper( @decs );

for ( my $line_ctr = 0; $line_ctr < $#decs ; ){

  $line = $decs[$line_ctr++];

  if( $line =~ /\(net/ ){
    @net_data = split(' ' , $line );

    $line_ctr += 1;

    $line2 = $decs[$line_ctr++];

    @net_data2 = split(' ' , $line2 );

    $net_data2[3] =~ s/\)//g;

    $net_ndx_name = $net_data2[2]."(".$net_data2[3].")";

    $line3 = $decs[$line_ctr++];
    @inst_data = split(' ' , $line3 );

    $net_data[3] =~ s/"\)|"//g;
    $inst_data[3] =~ s/\)//g;

    #print "$net_data[3]  $inst_data[3]\n";

    #$sig_inst1{$net_data[3]} = $inst_data[3];

    #$sig_inst1{$net_ndx_name} = $inst_data[3];

   $sig_inst2{$inst_data[3]} = $net_ndx_name;

  }

}
#print Dumper( %p_data2 );

#$grep_exp = 0;

#while( my ($k, $v) = each %sig_inst1 ) {

#$grep_exp = "I \\(instanceRef $v";

#@grep_result = qx[egrep -B 2 '$grep_exp' $core_fname];
 
#@net_data = split( ' ' , $grep_result[0] );

#$sig_inst{$k} = { "net" => $net_data[1],
#		  "inst" => $v,
#		};


#}

my $grep_exp = "";
foreach ( keys %sig_inst2 ){

  $grep_exp .= "I \\(instanceRef $_|";

}

$grep_exp .= "ENDMATCH";

#print $grep_exp."\n";

@grep_result2 = qx[egrep -B 2 '$grep_exp' $core_fname];

for ( my $line_ctr = 0; $line_ctr < $#grep_result2 ; ){

  my $line = $grep_result2[$line_ctr++];

  if ( $line =~ /\(net / ){

    my @net_line = split( ' ' , $line );

    $line_ctr++;

    my @port_line =  split( ' ' , $grep_result2[$line_ctr++] );

    $port_line[3] =~ s/\)//g;

   $sig_inst{$sig_inst2{$port_line[3]}} = { "net" =>  $net_line[1],
		  "inst" =>$port_line[3],
		};

  }


}


return \%sig_inst;

}

1;
