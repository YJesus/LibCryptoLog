use Crypt::RSA;
MIME::Base64;

$rsa = new Crypt::RSA;

($public, $private) = $rsa->keygen( Size => 1024, Filename => "key" ) ;

