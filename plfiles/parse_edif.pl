#!/usr/bin/perl -w
use Data::Dumper;

$PLHOME=$ENV{BITSIZE_HOME}."/plfiles";

$PARSE_CORE_PL=$PLHOME."/parse_core_edif.pl";

require $PARSE_CORE_PL;

#require 'parse_core_edif.pl';


#$output_nets = get_outport_nets( $ARGV[0] );

#print Dumper($output_nets);


sub get_outport_nets{

my $top_file_name  = shift(@_);

@file_data = open_file( $top_file_name );



my $inst_data = get_inst_data( \@file_data , $top_file_name );


my $sig_port_names = get_sig_names( \$inst_data ,\@file_data  );


my $oport_nets = resolve_outport_names(\$inst_data , \$sig_port_names , $top_file_name );

return $oport_nets;
}

sub open_file{

my $file_name = shift(@_);

my @lines;

open(FILE, $file_name ) or die "Can't open $file_name : $!\n";
@lines = <FILE>;
close(FILE);

@lines;

}


sub get_inst_data{

my $lines = shift(@_);

my $top_fname = shift(@_);

my %inst_data;

@insts = grep{/\(instance \w+ \(viewRef/} @$lines;

#print Dumper(@insts);

#foreach (@insts){

#  my @line_data = split( ' ' , $_ );


#  $line_data[5] =~ s/\)//g;

#  @port_data = qx[egrep -A 6 '\\(cell $line_data[5]' $ARGV[0]];

#  @out_port = grep{/OUTPUT/} @port_data;


#  @port_name = split(' ', $out_port[0] );



#  $inst_data{$line_data[1]} = {
#			       'comp_name' => $line_data[5],
#			       'out_port_name' => $port_name[3],
#			       };

#}

my $grep_exp = "";

foreach (@insts){

  my @line_data = split( ' ' , $_ );

   $line_data[5] =~ s/\)//g;

 $grep_exp .= "\\(cell " .$line_data[5]."|";

 $inst_cells{$line_data[1]} = $line_data[5];

}

$grep_exp .= "DONOTMATCH";

@cell_data = qx[egrep -A 6 '$grep_exp' $top_fname];

#print Dumper(@cell_data);

my $cell_name;

for ( my $line_ctr = 0; $line_ctr < $#cell_data ; ){

  my $line = $cell_data[$line_ctr++];

  if ( $line =~ /\(cell (\w+) \(cellType/ ){

  $cell_name = $1;


  }
  elsif ( $line =~ /OUTPUT/ ){

    my @line_data = split( ' ', $line );

    $port_name = $line_data[3];

    print "$cell_name : $port_name \n";

    $cell_port{$cell_name} = $port_name;

  }

}

while( my ($k, $v) = each %inst_cells ){


  #print "$k : $v : $cell_port{$v}\n";

  $inst_data{$k} = {
			       'comp_name' => $v,
			       'out_port_name' =>$cell_port{$v},
			       };


}



return \%inst_data;

}


sub get_sig_names{

my $inst_data = shift(@_);

my $lines = shift(@_);

my %sig_name_rec = ();

#print Dumper( $inst_data );


while( my ($k, $v) = each %{$$inst_data} ){
#print $inst_data{$k}->{comp_name}."\n";



#print Dumper($v);

@port_refs = grep{/portRef .* $k/} @$lines;

my @line_recs;

foreach ( @port_refs ){



  if ( $_ !~ /CLK/ ){

    my @line_data = split(' ', $_ );

    #$line_data[3] =~ s/\)//g;


   if ( $line_data[2] eq $v->{out_port_name} ){
    $ld = "$line_data[2]($line_data[3]";

    push( @line_recs , $ld );
  }
  }



}


 $sig_name_rec{$k} = \@line_recs;

}

return \%sig_name_rec;


}
sub get_cells{

my $lines = shift(@_);

@cell_decs = grep{ /\(cell \w+ \(cellType.*/ } @$lines;

@cells = map{ my @k = split(' ',$_);$k[1];} @cell_decs;

@cells;

}



sub get_sigs{

my $lines_ = shift(@_);
my @lines = @$lines_;

my %sig_data = ();

for ( my $line_ctr = 0; $line_ctr < $#lines ; ){

  $line = $lines[$line_ctr++];

  if ( $line =~ /\(net.*/ ){

    if ( $line !~ /clk.*/) {
      @line_data1 = split( ' ', $line );

      @src_port = split(' ', $lines[$line_ctr++] );
#      @dest_port = split(' ', $lines[$line_ctr++] );

      #print Dumper( @src_port );


      #my @data_array = [ $line_data1[3] , $src_port[2],$src_port[3], $dest_port[2],$dest_port[3] ];

      #$sig_data{$line_data1[2]} = [ $line_data1[3] , $src_port[2],$src_port[3], $dest_port[2],$dest_port[3] ];

      $line_data1[3] =~ s/"|\)$//g;

      $sig_data{$line_data1[2]} = { "rename" => $line_data1[3],
				    };


    }
  }

}



return \%sig_data;
}


sub resolve_outport_names{


my $my_inst_data = shift(@_);

my $my_sig_port_names = shift(@_);

my %outport_nets = ();

my $core_name = shift(@_); #"bitsize_test.edf";

my @file_data = open_file_core( $core_name );


my $grep_exp = "";;

foreach my $inst_name ( keys %$$my_inst_data ){

  #if ( $inst_name =~ /Mul/ ){
  if (( $$my_inst_data->{$inst_name}->{comp_name} =~ /mul/ ) || ( $$my_inst_data->{$inst_name}->{comp_name} =~ /input_rom/ )) {


    my $core_file_name = $$my_inst_data->{$inst_name}->{comp_name}.".edn";

    print "Parsing $core_file_name.. ";
    my $core_sig_data = run_core_parse( $core_file_name );
    print "Done !\n";

   
    my $sig_data_array  = $$my_sig_port_names->{$inst_name};

    foreach (@$sig_data_array){

      my $out_sig_name = $inst_name."/".$core_sig_data->{$_}->{net};
      #print "$_ : $out_sig_name \n";

      #$outport_nets{$inst_name}{$out_sig_name} = $_;

      if ( !$core_sig_data->{$_}->{net} eq "" ){
      $outport_nets{$inst_name}{$core_sig_data->{$_}->{net}} = $_;
    }
 
    }

  }
  else{

    #print " $inst_name : $$my_inst_data->{$inst_name}->{comp_name} : $$my_inst_data->{$inst_name}->{out_port_name} \n";

    if ( $$my_inst_data->{$inst_name}->{out_port_name} !~ /OUTPUT/ ){
    $grep_exp .= "\\(portRef.* $$my_inst_data->{$inst_name}->{out_port_name}.* $inst_name|";
  }
  }

}


$grep_exp .= "ENDMATCH";

#print "$grep_exp\n";

my @grep_result = qx[egrep -B 1 '$grep_exp' $core_name];


#my @grep_result = qx[egrep -B 1 '$grep_exp' bitsize_test.edf];

for ( my $line_ctr = 0; $line_ctr < $#grep_result ; ){

  my $line = $grep_result[$line_ctr++];

  if ( $line =~ /\(net/ ){

    my @line_data1 = split( ' ' , $line );

    $line_data1[3] =~ s/"\)//;
    $line_data1[3] =~ s/"//g;

    my @line_data2 = split( ' ' ,  $grep_result[$line_ctr++] );

    $line_data2[5] =~ s/\)//g;

    $outport_nets{$line_data2[5]}{$line_data1[3]} = $line_data1[2];
  }


}

#print Dumper(%outport_nets);

return \%outport_nets;

}

1;
