Simple tool to calculate the fletcher4 checksum as used in ZFS

Usage: fletcher4 [blocksize  [seek]]  

Warning: the tool will output the binary checksums on stdout and textual on stderr

typical usage:  fletcher4 2>&1 >/dev/null   
  to just see human readable output
