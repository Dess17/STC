f = fopen( 'detector_src_32f.bin', 'r' );
x = fread( f, inf, 'float32' );
fclose( f );
plot( x );