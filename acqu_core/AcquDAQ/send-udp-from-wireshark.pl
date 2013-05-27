#!/usr/bin/perl

use strict;
use warnings;

use Net::PcapUtils;
use NetPacket::Ethernet qw(:strip);
use NetPacket::IP;
use NetPacket::UDP;
use IO::Socket;
use Time::HiRes qw(usleep);

my $packet_nr=0;
my $elapsed = 0;
sub process_pkt {
   my($sock, $hdr, $pkt) = @_;

   my $ip_obj = NetPacket::IP->decode(eth_strip($pkt));
   my $udp_obj = NetPacket::UDP->decode($ip_obj->{data});

   $packet_nr++;

   $sock->send($udp_obj->{data});
   # include only if printing out each packet data in debug mode
   #$elapsed += usleep(200);
   #select(undef, undef, undef, 0.01);
}

my $infile = shift || die "Usage: $0 <infile>\n";

my $sock = IO::Socket::INET->new(
                                 Proto    => 'udp',
                                 PeerPort => 50000,
                                 PeerAddr => '192.168.1.1',
                                ) or die "Could not create socket: $!\n";

print Net::PcapUtils::loop(\&process_pkt,
                           (SAVEFILE => $infile,
                            USERDATA => $sock,
                            FILTER=>'udp',
                            DEV => 'eth0'));
print $packet_nr," ",$elapsed/1e6,"\n";

