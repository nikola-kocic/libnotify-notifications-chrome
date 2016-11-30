#!/usr/bin/perl

#
# Depends on JSON:PP and /usr/bin/notify-send
#

use strict;
use JSON::PP qw(decode_json);

my $buf;
my $text_length;

while (1) {
    sysread STDIN, $buf, 4;

    next if(length($buf) == 0);

    $text_length = unpack('i', $buf);
    sysread STDIN, $buf, $text_length;

    open(BOB, '>>/tmp/bob');
    print BOB $buf;
    close(BOB);

    my $decoded = decode_json($buf);

# icons not supported yet
#    if($decoded->{'iconUrl'}) {
#        system('/usr/bin/notify-send', $decoded->{'title'}, $decoded->{'body'}, '-i ', $decoded->{'iconUrl'});
#    } else {
        system('/usr/bin/notify-send', $decoded->{'title'}, $decoded->{'body'});
#    }

    my $response;
    
    if(!$?) {
        $response = '{"status": "OK"}';
    } else {
        $response = '{"status": "ERROR"}';
    }
    print pack('I', length($response)) . $response;
}
