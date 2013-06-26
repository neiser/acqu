#!/usr/bin/perl

use strict;
use warnings;

use IO::Socket;
use Time::HiRes qw(usleep);




my $infile = shift || die "Usage: $0 <infile>\n";
my $max_packets = shift || 'inf';
my $skippackets = shift || 0;
my $sock = IO::Socket::INET->new(
                                 Proto    => 'udp',
                                 PeerPort => 50000,
                                 PeerAddr => '192.168.1.1',
                                ) or die "Could not create socket: $!\n";

open(my $fh, "<$infile") or die "can't open $infile: $!";
binmode $fh;
$/ = \4;                       ## set record size


my $packet_nr = 0;
my $elapsed = 0;
my $eventsize=0;
my $true_eventsize=0;
my $words=0;
my $trbdata='';
while(<$fh>) {
  if($eventsize==$words) {
    if($trbdata ne '' && $eventsize>41) {
      if($skippackets>0) {
        #print "Skipping...\n";
        $skippackets--;
      }
      else {
        #print "Sending it $true_eventsize\n";
        # include only if printing out each packet data in debug mode
        #$elapsed += usleep(200);
        $sock->send(pack('N',,($true_eventsize-5)*4).'aaaa'.$trbdata.('padd' x 6));
        #sleep(1);
        $sock->flush;
        $packet_nr++;
        last if $packet_nr==$max_packets;
      }
    }
    $eventsize = hex join '', reverse unpack 'H2H2H2H2', $_;
    #$true_eventsize=$eventsize;
    #print "Bytes: $eventsize\n";
    $eventsize = $eventsize/4;
    $true_eventsize = $eventsize;
    if($eventsize%2==0) {
      #print "Edited eventsize $eventsize\n";
      $eventsize--; # if $eventsize%2==0;
    }
    #if($packet_nr>0) {
    #  printf("%02d: %08x\n",$words, unpack 'N', $_);
    #  print $skippackets, "Found event, size: ",$eventsize, "\n";
    #}
    $words = 0;
    $trbdata = '';
    next;
  }
  $words++;
  if($words>=8) {
    $trbdata .= $_;
  }
  #if($packet_nr>0 && $words<30) {
  #  printf("%02d: %08x\n",$words, unpack 'N', $_);
  #}
  #last if $words>100;
}

close $fh;

print $packet_nr," ",$elapsed/1e6,"\n";

