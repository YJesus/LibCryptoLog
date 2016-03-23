use Crypt::RSA;
MIME::Base64;
use strict ;


my $private = new Crypt::RSA::Key::Private( Filename => "key.private", );

my $rse = new Crypt::RSA;

if (@ARGV < 2) {
  print STDERR "Usage: $0 inputfile outputfile\n";
  exit 1;
}


open FILE, @ARGV[0] or die $!;

open FILEDUMP, ">@ARGV[1]"; 

my $todecip;

while (<FILE>) {
	
	
	my $dodecip = 0;
	
	if (/BEGINCRYPTO/../ENDCRYPTO/) {
		 if ( /ENDCRYPTO/) { $dodecip = 1 }
		
		 if (!/BEGINCRYPTO/ && !/ENDCRYPTO/) { 
		
			$todecip =$todecip.$_;
			
		}
		
	}
	
	else {
		
		print FILEDUMP $_;
		
	}
	
	if ($dodecip == 1) {
		
		my $decoded = MIME::Base64::decode_base64($todecip);
		my $c = $rse->decrypt( Ciphertext => $decoded, Key => $private, ) || die "fatal"; 
		
		print FILEDUMP "$c\n";
		$dodecip = 0 ;
		$todecip  = undef;
	}
}
