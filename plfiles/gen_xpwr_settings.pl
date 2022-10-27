#!/usr/bin/perl

$setting_filename = "bs_xpwr_settings.tcl";


if( !( -e $setting_filename  ))
  {

    open( SFILE , ">$setting_filename" );

    print SFILE (<<INFO);
setPowerUnit $ENV{XPWR_POWER_UNIT};
setCurrentUnit $ENV{XPWR_CURRENT_UNIT};
setFrequencyUnit $ENV{XPWR_FREQ_UNIT};

INFO

    close( SFILE );

  }
