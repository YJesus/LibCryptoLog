use Crypt::RSA;
MIME::Base64;
use strict;

my $public = new Crypt::RSA::Key::Public( Filename => "/usr/local/etc/key.public", );

my $rse = new Crypt::RSA;

my $todecode = @ARGV[0];

my $message = MIME::Base64::decode_base64(@ARGV[0]);

chomp($message);

my $c = $rse->encrypt( Message => $message, Key => $public, ); 

my $encoded = MIME::Base64::encode_base64($c);

my $response = "BEGINCRYPTO\n".$encoded."ENDCRYPTO\n";

print $response;
